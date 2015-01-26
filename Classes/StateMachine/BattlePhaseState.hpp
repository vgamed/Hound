#ifndef __HOUND_BATTLE_PHASE_STATE_H__
#define __HOUND_BATTLE_PHASE_STATE_H__

#include "cocos2d.h"
#include "MoveState.hpp"

template <typename T, 
	STATE_MACHINE_EVENT finishEvent = STATE_MACHINE_EVENT::NONE, 
	STATE_MACHINE_EVENT deadEvent = STATE_MACHINE_EVENT::NONE> 
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

template <typename T, STATE_MACHINE_EVENT finishEvent, STATE_MACHINE_EVENT deadEvent> 
void BattlePhaseState<T, finishEvent, deadEvent>::enter(T &t)
{
	auto total_weapons = t.getWeapons();
	for (const auto id : m_weaponGroup)
		for (auto weapon : total_weapons)
	{
		if (id == weapon->getId())
		{
			weapon->reset();
			m_weapons.push_back(weapon);
			break;
		}
	}
	
	MoveState<T>::enter(t);
}

template <typename T, STATE_MACHINE_EVENT finishEvent, STATE_MACHINE_EVENT deadEvent> 
void BattlePhaseState<T, finishEvent, deadEvent>::exec(T &t, float dt)
{
	t.selectTarget();

	for (auto weapon : m_weapons)
	{
		weapon->update(dt);
	}

	MoveState<T>::exec(t, dt);

	// if life is below 0, trigger the dead event
	if (t.getCurLife() <= 0.0f)
	{
		t.getStateMachine().triggerEvent((int)deadEvent);
		return;
	}

	// if movement is finished, or life is below the predefined threshold, trigger the finish event
	if ((MoveState<T>::isDone())||
		(t.getCurLife() <= m_lifeThreshold))
	{
		t.getStateMachine().triggerEvent((int)finishEvent);
	}
}

template <typename T, STATE_MACHINE_EVENT finishEvent, STATE_MACHINE_EVENT deadEvent> 
void BattlePhaseState<T, finishEvent, deadEvent>::exit(T &t)
{
	MoveState<T>::exit(t);
}

#endif //__HOUND_BATTLE_PHASE_STATE_H__

