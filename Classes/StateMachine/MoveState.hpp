#ifndef __HOUND_MOVE_STATE_H__
#define __HOUND_MOVE_STATE_H__

#include "../Common.h"
#include "StateMachine.hpp"

template <typename T>
class MoveState : public State<T>
{
public:
	MoveState() {}
	MoveState(const MOVEMENTS &movements);
	~MoveState(void) {}

	void enter(T &t);
	void exec(T &t, float dt);
	void exit(T &t);

	bool isMoveFinished(void) { return m_moveFinished; }

private:
	typedef std::function<bool (T &t, float dt)> MOVEMENT_DO_FUNC;

	void initMovement(T &t);
	void initDisplacement(T &t);
	void initRotation(T &t);
	void initStay(T &t);

	bool doDisplacement(T &t, float dt);
	bool doRotation(T &t, float dt);
	bool doStay(T &t, float dt);

	MOVEMENTS		m_movements;
	std::size_t		m_curIndexMovement;

	MOVEMENT_DO_FUNC	m_movementDoFunc;

	cocos2d::Vec2	m_displmtDir;
	float			m_rotateDir;
	float			m_stayTimer;

	bool			m_isMoveFinished;
};

template <typename T> MoveState<T>::MoveState(const MOVEMENTS &movements)
	: m_movements(movements)
	, m_curIndexMovement(0)
	, m_movementDoFunc(nullptr)
	, m_displmtDir(cocos2d::Vec2::ZERO)
	, m_rotateDir(0.0f)
	, m_stayTimer(0.0f)
	, m_isMoveFinished(false)
{
}

template <typename T> void MoveState<T>::enter(T &t)
{
	CC_ASSERT(m_movements.size() > 0);

	m_curIndexMovement = 0;
	
	initMovement(t);

	m_isMoveFinished = false; // start the movement
}

template <typename T> void MoveState<T>::exec(T &t, float dt)
{
	if (m_movementDoFunc(t, dt))
	{
		m_curIndexMovement++;
		if (m_curIndexMovement < m_movements.size())
		{
			initMovement(t);
		}
		else
		{
			m_isMoveFinished = true;
		}
	}
}

template <typename T> void MoveState<T>::exit(T &t)
{
}

template <typename T> void MoveState<T>::initMovement(T &t)
{
	switch (m_movements[m_curIndexMovement].type)
	{
	case MOVEMENT_TYPE::DISPLACEMENT:
		initDisplacement(t);
		m_movementDoFunc = CC_CALLBACK_2(MoveState<T>::doDisplacement, this);
		break;
	case MOVEMENT_TYPE::ROTATION:
		initRotation(t);
		m_movementDoFunc = CC_CALLBACK_2(MoveState<T>::doRotation, this);
		break;
	case MOVEMENT_TYPE::STAY:
		initStay(t);
		m_movementDoFunc = CC_CALLBACK_2(MoveState<T>::doStay, this);
		break;
	default:
		m_movementDoFunc = nullptr;
		break;
	}
}

template <typename T> void MoveState<T>::initDisplacement(T &t)
{
	//// settle down the start position
	//if (m_movements.size() > 1)
	//{
	//	t->setPosition(m_movements[m_curIndexMovement].target_position);
	//	m_direction = (	m_movements[m_curIndexMovement+1].target_position-
	//					m_movements[m_curIndexMovement].target_position).normalize();

	//	// if the no_rotate of next waypoint is true, 
	//	// set the rotation of the sprite to 0.0f
	//	if (m_movements[m_curIndexMovement+1].no_rotate)
	//	{
	//		//t->setRotation(0.0f);
	//	}
	//	else // set rotation the sprite to the direction
	//	{
	//		float angle = m_direction.getAngle(cocos2d::Vec2::UNIT_Y);
	//		t->setRotation(CC_RADIANS_TO_DEGREES(angle));
	//	}
	//}
	//else // a static sprite
	//{
	//	t->setPosition(m_movements[m_curIndexMovement].target_position);
	//	m_direction = cocos2d::Vec2::ZERO;
	//	//t->setRotation(0.0f);
	//}

}

template <typename T> void MoveState<T>::initRotation(T &t)
{
}

template <typename T> void MoveState<T>::initStay(T &t)
{
}

template <typename T> bool MoveState<T>::doDisplacement(T &t, float dt)
{
//	t->setPosition(t->getPosition()+m_direction*m_movements[m_curIndexMovement+1].move_speed*dt);
	return false;
}

template <typename T> bool MoveState<T>::doRotation(T &t, float dt)
{
	return false;
}

template <typename T> bool MoveState<T>::doStay(T &t, float dt)
{
	return false;
}

#endif //__HOUND_MOVE_STATE_H__

