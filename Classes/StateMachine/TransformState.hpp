#ifndef __HOUND_TRANSFORM_STATE_H__
#define __HOUND_TRANSFORM_STATE_H__

#include "cocos2d.h"
#include "StateMachine.hpp"

template <typename T>
class TransformState: public State<T>
{
public:
	typedef std::vector<cocos2d::Vec2> WAYPOINTS;
	TransformState(void) {}
	~TransformState(void) {}

	void enter( T* t );
	void exec( T* t, float dt );
	void exit( T* t );

private:
	cocos2d::Vec2	m_startPostion;
	WAYPOINTS		m_waypoints;
	float			m_rotationZ;
	float			m_speed;
};

template <typename T> void TransformState<T>::enter( T* t )
{
}

template <typename T> void TransformState<T>::exec( T* t, float dt )
{
}

template <typename T> void TransformState<T>::exit( T* t )
{
}

#endif //__HOUND_TRANSFORM_STATE_H__

