#ifndef __HOUND_LEAVE_STATE_H__
#define __HOUND_LEAVE_STATE_H__

#include "cocos2d.h"
#include "MoveState.hpp"

template <typename T, STATE_MACHINE_EVENT finishEvent = STATE_MACHINE_EVENT::NONE>
class LeaveState : public MoveState<T>
{
public:
	LeaveState(const StateInfo &info)
		: MoveState<T>(info)
	{}
	~LeaveState(void) {}

	void enter(T &t);
	void exec(T &t, float dt);
	void exit(T &t);
};

template <typename T, STATE_MACHINE_EVENT finishEvent>
void LeaveState<T, finishEvent>::enter(T &t)
{
	t.setInvincible(true);

	Movement move;
	move.type = MOVEMENT_TYPE::DISPLACEMENT;
	move.target_position = cocos2d::Vec2(t.getPosition().x, -t.getBoundingBox().size.height);
	move.move_param.displmt.facing_dir = true;
	move.move_param.displmt.speed = 1000.0f;
	MoveState<T>::addMovement(move);

	MoveState<T>::enter(t);
}

template <typename T, STATE_MACHINE_EVENT finishEvent>
void LeaveState<T, finishEvent>::exec(T &t, float dt )
{
	MoveState<T>::exec(t, dt);

	if (MoveState<T>::isDone())
	{
		t.getStateMachine().triggerEvent((int)finishEvent);
	}
}

template <typename T, STATE_MACHINE_EVENT finishEvent>
void LeaveState<T, finishEvent>::exit(T &t)
{
	t.setInvincible(false);

	MoveState<T>::exit(t);
}

#endif //__HOUND_LEAVE_STATE_H__

