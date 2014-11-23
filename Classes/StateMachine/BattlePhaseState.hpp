#ifndef __HOUND_BATTLE_PHASE_STATE_H__
#define __HOUND_BATTLE_PHASE_STATE_H__

#include "cocos2d.h"
#include "MoveState.hpp"

template <typename T, STATE_MACHINE_EVENT finishEvent = STATE_MACHINE_EVENT::NONE>
class BattlePhaseState : public MoveState<T>
{
public:
	BattlePhaseState(void) {}
	~BattlePhaseState(void) {}

	void enter(T &t);
	void exec(T &t, float dt);
	void exit(T &t);
};

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void BattlePhaseState<T, finishEvent>::enter(T &t)
{
}

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void BattlePhaseState<T, finishEvent>::exec(T &t, float dt)
{
}

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void BattlePhaseState<T, finishEvent>::exit(T &t)
{
}

#endif //__HOUND_BATTLE_PHASE_STATE_H__

