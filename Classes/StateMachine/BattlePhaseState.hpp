#ifndef __HOUND_BATTLE_PHASE_STATE_H__
#define __HOUND_BATTLE_PHASE_STATE_H__

#include "cocos2d.h"
#include "MoveState.hpp"

template <typename T, STATE_MACHINE_EVENT finishEvent = STATE_MACHINE_EVENT::NONE>
class BattlePhaseState : public MoveState<T>
{
public:
	BattlePhaseState(const StateInfo &info)
		: MoveState<T>(info)
		, m_weaponGroup(info.weapons)
		, m_lifeThreshold(info.life_threshold)
	{}
	~BattlePhaseState(void) {}

	void enter(T &t);
	void exec(T &t, float dt);
	void exit(T &t);

private:
	WEAPON_GROUP			m_weaponGroup;
	float					m_lifeThreshold;

	std::vector<Weapon*>	m_weapons;
};

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void BattlePhaseState<T, finishEvent>::enter(T &t)
{
	auto total_weapons = t.getWeapons();
	for (const auto id : m_weaponGroup)
		for (auto weapon : total_weapons)
	{
		if (id == weapon->getId())
		{
			m_weapons.push_back(weapon);
		}
	}
	
	MoveState<T>::enter(t);
}

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void BattlePhaseState<T, finishEvent>::exec(T &t, float dt)
{
	for (auto weapon : m_weapons)
	{
		weapon->update(dt);
	}

	MoveState<T>::exec(t, dt);

	if ((MoveState<T>::isMoveFinished())||
		(t.getCurLife() <= m_lifeThreshold))
	{
		t.getStateMachine().triggerEvent((int)finishEvent);
	}
}

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void BattlePhaseState<T, finishEvent>::exit(T &t)
{
	MoveState<T>::exit(t);
}

#endif //__HOUND_BATTLE_PHASE_STATE_H__

