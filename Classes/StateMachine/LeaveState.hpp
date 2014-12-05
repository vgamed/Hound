#ifndef __HOUND_LEAVE_STATE_H__
#define __HOUND_LEAVE_STATE_H__

#include "cocos2d.h"
#include "MoveState.hpp"

USING_NS_CC;

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

private:
	void calcTargetPosition(const T &t, Vec2 &target);
};

template <typename T, STATE_MACHINE_EVENT finishEvent>
void LeaveState<T, finishEvent>::enter(T &t)
{
	t.setInvincible(true);

	Movement move;
	move.type = (int)MOVEMENT_TYPE::DISPLACEMENT;
	calcTargetPosition(t, move.target_position);
	
	CC_ASSERT(move.target_position != Vec2::ZERO);

	move.displmt_auto_facing = t.isLeaveAutoFacing();
	move.speed = t.getLeaveSpeed();
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

template <typename T, STATE_MACHINE_EVENT finishEvent>
void LeaveState<T, finishEvent>::calcTargetPosition(const T &t, Vec2 &target)
{
	//target = Vec2(t.getPosition().x, -t.getBoundingBox().size.height);
	target = Vec2::ZERO;

	Rect border = t.getParent()->getBoundingBox();
	float length = border.origin.getDistance(Vec2(border.getMaxX(), border.getMaxY()));
	Vec2 dir = Vec2::UNIT_Y.rotateByAngle(Vec2::ZERO, CC_DEGREES_TO_RADIANS(t.getRotation()));

	Vec2 A, B, C, D;
	A = t.getPosition();
	B = A + dir * length;
	
	do{
		// test intersect with the bottom line of border
		C = Vec2(border.getMinX(),border.getMinY());
		D = Vec2(border.getMaxX(),border.getMinY());
		target = Vec2::getIntersectPoint(A, B, C, D);
		if (target != Vec2::ZERO)
			break;
		// test intersect with the top line of border
		C = Vec2(border.getMinX(),border.getMaxY());
		D = Vec2(border.getMaxX(),border.getMaxY());
		target = Vec2::getIntersectPoint(A, B, C, D);
		if (target != Vec2::ZERO)
			break;
		// test intersect with the left line of border
		C = Vec2(border.getMinX(),border.getMinY());
		D = Vec2(border.getMinX(),border.getMaxY());
		target = Vec2::getIntersectPoint(A, B, C, D);
		if (target != Vec2::ZERO)
			break;
		// test intersect with the right line of border
		C = Vec2(border.getMaxX(),border.getMinY());
		D = Vec2(border.getMaxX(),border.getMaxY());
		target = Vec2::getIntersectPoint(A, B, C, D);
		if (target != Vec2::ZERO)
			break;
	}while(0);
}

#endif //__HOUND_LEAVE_STATE_H__

