#ifndef __HOUND_DEAD_STATE_H__
#define __HOUND_DEAD_STATE_H__

#include "cocos2d.h"
#include "StateMachine.hpp"

template <typename T>
class DeadState : public State<T>
{
public:
	DeadState(const StateInfo &info)
	{
		State<T>::m_id = info.id;
		State<T>::m_type = (int)(info.type);
	}

	~DeadState(void) 
	{}

	void enter(T &t)
	{ State<T>::m_done = true; }

	void exec(T &t, float dt)
	{}

	void exit(T &t)
	{}
};

#endif //__HOUND_DEAD_STATE_H__

