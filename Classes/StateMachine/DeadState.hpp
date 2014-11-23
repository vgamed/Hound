#ifndef __HOUND_DEAD_STATE_H__
#define __HOUND_DEAD_STATE_H__

#include "cocos2d.h"
#include "StateMachine.hpp"

template <typename T, STATE_MACHINE_EVENT finishEvent = STATE_MACHINE_EVENT::NONE>
class DeadState : public State<T>
{
public:
	DeadState(void) {}
	~DeadState(void) {}

	void enter(T &t);
	void exec(T &t, float dt);
	void exit(T &t);
};

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void DeadState<T, finishEvent>::enter(T &t)
{
}

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void DeadState<T, finishEvent>::exec(T &t, float dt)
{
}

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void DeadState<T, finishEvent>::exit(T &t)
{
}

#endif //__HOUND_DEAD_STATE_H__

