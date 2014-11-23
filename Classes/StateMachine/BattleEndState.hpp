#ifndef __HOUND_BATTLE_END_STATE_H__
#define __HOUND_BATTLE_END_STATE_H__

#include "cocos2d.h"
#include "StateMachine.hpp"

template <typename T, STATE_MACHINE_EVENT finishEvent = STATE_MACHINE_EVENT::NONE>
class BattleEndState : public State<T>
{
public:
	BattleEndState(void) {}
	~BattleEndState(void) {}

	void enter(T &t);
	void exec(T &t, float dt);
	void exit(T &t);

private:
	cocos2d::Vec2	m_startPostion;
	WAYPOINTS		m_waypoints;
	float			m_rotationZ;
	float			m_speed;
};

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void BattleEndState<T, finishEvent>::enter(T &t)
{
}

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void BattleEndState<T, finishEvent>::exec(T &t, float dt)
{
}

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void BattleEndState<T, finishEvent>::exit(T &t)
{
}

#endif //__HOUND_BATTLE_END_STATE_H__

