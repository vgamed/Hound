#ifndef __HOUND_ENTRY_STATE_H__
#define __HOUND_ENTRY_STATE_H__

#include "cocos2d.h"
#include "MoveState.hpp"

template <typename T>
class EntryState : public MoveState<T>
{
public:
	EntryState(const StateInfo &info)
		: MoveState<T>(info)
	{}
	~EntryState(void) {}

	void enter( T &t );
	void exit( T &t );
};

template <typename T> void EntryState<T>::enter( T &t )
{
	t.setInvincible(true);
	MoveState<T>::enter(t);
}

template <typename T> void EntryState<T>::exit( T &t )
{
	t.setInvincible(false);
	MoveState<T>::exit(t);
}

#endif //__HOUND_ENTRY_STATE_H__

