#ifndef __HOUND_MOVE_STATE_H__
#define __HOUND_MOVE_STATE_H__

#include "Common.h"
#include "StateMachine.hpp"

template <typename T>
class MoveState : public State<T>
{
public:
	MoveState(const StateInfo &info);
	virtual ~MoveState(void) {}

	virtual void enter(T &t);
	virtual void exec(T &t, float dt);
	virtual void exit(T &t);

	void addMovement(const Movement &move);

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
	bool			m_repeat;

	std::size_t		m_curIndexMovement;

	MOVEMENT_DO_FUNC	m_movementDoFunc;

	cocos2d::Vec2	m_displmtDir;
	float			m_rotateDir;
	float			m_stayTimer;
};

template <typename T> 
MoveState<T>::MoveState(const StateInfo &info)
	: m_movements(info.movements)
	, m_repeat(info.repeat_movements)
	, m_curIndexMovement(0)
	, m_movementDoFunc(nullptr)
	, m_displmtDir(cocos2d::Vec2::ZERO)
	, m_rotateDir(0.0f)
	, m_stayTimer(0.0f)
{
	State<T>::m_id = info.id;
	State<T>::m_type = (int)(info.type);
}

template <typename T>
void MoveState<T>::enter(T &t)
{
	m_curIndexMovement = 0;
	
	if (m_movements.size() > 0)
	{
		initMovement(t);
	}

	State<T>::m_done = false; // start the movement
}

template <typename T>
void MoveState<T>::exec(T &t, float dt)
{
	if (m_movements.size() <= 0)
		return;

	if (!State<T>::m_done && 
		(m_movementDoFunc != nullptr) && 
		m_movementDoFunc(t, dt))
	{
		m_curIndexMovement++;
		if (m_curIndexMovement < m_movements.size())
		{
			initMovement(t);
		}
		else if (m_repeat)
		{
			m_curIndexMovement = 0;
			initMovement(t);
		}
		else
		{
			State<T>::m_done = true;
		}
	}
}

template <typename T>
void MoveState<T>::exit(T &t)
{
}

template <typename T>
void MoveState<T>::addMovement(const Movement &move)
{
	CC_ASSERT((move.type > (int)MOVEMENT_TYPE::NONE)&&
		(move.type < (int)MOVEMENT_TYPE::MAX_VALUE));
	m_movements.push_back(move);
}

template <typename T>
void MoveState<T>::initMovement(T &t)
{
	switch (m_movements[m_curIndexMovement].type)
	{
	case (int)MOVEMENT_TYPE::DISPLACEMENT:
		initDisplacement(t);
		m_movementDoFunc = CC_CALLBACK_2(MoveState<T>::doDisplacement, this);
		break;
	case (int)MOVEMENT_TYPE::ROTATION:
		initRotation(t);
		m_movementDoFunc = CC_CALLBACK_2(MoveState<T>::doRotation, this);
		break;
	case (int)MOVEMENT_TYPE::STAY:
		initStay(t);
		m_movementDoFunc = CC_CALLBACK_2(MoveState<T>::doStay, this);
		break;
	default:
		m_movementDoFunc = nullptr;
		break;
	}
}

template <typename T>
void MoveState<T>::initDisplacement(T &t)
{
	CC_ASSERT(m_movements[m_curIndexMovement].type == (int)MOVEMENT_TYPE::DISPLACEMENT);

	if (m_movements[m_curIndexMovement].speed <= 0)
	{
		// it's a instant dispalcement
		t.setPosition(m_movements[m_curIndexMovement].target_position);
	}
	else
	{
		// prepare for the sequent displacements
		m_displmtDir = m_movements[m_curIndexMovement].target_position - t.getPosition();
		m_displmtDir.normalize();
		if (m_movements[m_curIndexMovement].displmt_auto_facing)
		{
			t.setRotation(CC_RADIANS_TO_DEGREES(m_displmtDir.getAngle(cocos2d::Vec2::UNIT_Y)));
		}
	}
}

template <typename T>
void MoveState<T>::initRotation(T &t)
{
	CC_ASSERT(m_movements[m_curIndexMovement].type == (int)MOVEMENT_TYPE::ROTATION);

	if (m_movements[m_curIndexMovement].jump)
		t.setPosition(m_movements[m_curIndexMovement].target_position);

	if (m_movements[m_curIndexMovement].speed < 0)
	{
		// it's a instant rotation
		t.setRotation(m_movements[m_curIndexMovement].target_angle);
	}
	else
	{
		// prepare for the sequent rotation
		m_rotateDir = m_movements[m_curIndexMovement].target_angle;
		m_rotateDir = m_rotateDir/abs(m_rotateDir);
	}
}

template <typename T>
void MoveState<T>::initStay(T &t)
{
	CC_ASSERT(m_movements[m_curIndexMovement].type == (int)MOVEMENT_TYPE::STAY);

	if (m_movements[m_curIndexMovement].jump)
	{
		t.setPosition(m_movements[m_curIndexMovement].target_position);
		t.setRotation(m_movements[m_curIndexMovement].target_angle);
	}
	
	m_stayTimer = 0.0f;
}

template <typename T>
bool MoveState<T>::doDisplacement(T &t, float dt)
{
	float step = m_movements[m_curIndexMovement].speed*dt;
	float dist = m_movements[m_curIndexMovement].target_position.distance(t.getPosition());

	t.setPosition(t.getPosition() + step * m_displmtDir);

	if (step > dist)
	{
		return true; //the current movement is finished
	}

	return false;
}

template <typename T> bool MoveState<T>::doRotation(T &t, float dt)
{
	float step = m_movements[m_curIndexMovement].speed * dt;
	float dist = m_movements[m_curIndexMovement].target_angle - t.getRotation();

	t.setRotation(t.getRotation() + step*m_rotateDir);

	if (step > dist)
	{
		return true; //the current movement is finished
	}

	return false;
}

template <typename T>
bool MoveState<T>::doStay(T &t, float dt)
{
	m_stayTimer += dt;
	if (m_stayTimer >= m_movements[m_curIndexMovement].stay_period)
	{
		return true; //the current movement is finished
	}

	return false;
}

#endif //__HOUND_MOVE_STATE_H__

