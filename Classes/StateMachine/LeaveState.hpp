#ifndef __HOUND_LEAVE_STATE_H__
#define __HOUND_LEAVE_STATE_H__

#include "cocos2d.h"
#include "StateMachine.hpp"

template <typename T>
class LeaveState : public State<T>
{
public:
	typedef std::vector<cocos2d::Vec2> WAYPOINTS;
	LeaveState(void) {}
	~LeaveState(void) {}

	void enter( T* t );
	void exec( T* t, float dt );
	void exit( T* t );

private:
	cocos2d::Vec2	m_startPostion;
	WAYPOINTS		m_waypoints;
	float			m_rotationZ;
	float			m_speed;
};

template <typename T> void LeaveState<T>::enter( T* t )
{
}

template <typename T> void LeaveState<T>::exec( T* t, float dt )
{
}

template <typename T> void LeaveState<T>::exit( T* t )
{
}

#endif //__HOUND_LEAVE_STATE_H__

