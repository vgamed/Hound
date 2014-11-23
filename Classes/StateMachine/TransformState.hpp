#ifndef __HOUND_TRANSFORM_STATE_H__
#define __HOUND_TRANSFORM_STATE_H__

#include "cocos2d.h"
#include "MoveState.hpp"

template <typename T, STATE_MACHINE_EVENT finishEvent = STATE_MACHINE_EVENT::NONE>
class TransformState : public MoveState<T>
{
public:
	TransformState(void) {}
	~TransformState(void) {}

	void enter(T &t);
	void exec(T &t, float dt);
	void exit(T &t);
};

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void TransformState<T, finishEvent>::enter(T &t)
{
}

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void TransformState<T, finishEvent>::exec(T &t, float dt)
{
}

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void TransformState<T, finishEvent>::exit(T &t)
{
}

#endif //__HOUND_TRANSFORM_STATE_H__

