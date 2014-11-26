#ifndef __HOUND_ENTRY_STATE_H__
#define __HOUND_ENTRY_STATE_H__

#include "cocos2d.h"
#include "MoveState.hpp"

template <typename T, STATE_MACHINE_EVENT finishEvent = STATE_MACHINE_EVENT::NONE>
class EntryState : public MoveState<T>
{
public:
	EntryState(const StateInfo &info)
		: MoveState<T>(info)
	{}
	~EntryState(void) {}

	void enter(T &t);
	void exec(T &t, float dt);
	void exit(T &t);
};

template <typename T, STATE_MACHINE_EVENT finishEvent> 
void EntryState<T, finishEvent>::enter(T &t)
{
	t.setInvincible(true);

	MoveState<T>::enter(t);
}

template <typename T, STATE_MACHINE_EVENT finishEvent>
void EntryState<T, finishEvent>::exec(T &t, float dt)
{
	MoveState<T>::exec(t, dt);

	if (State<T>::isDone())
	{
		t.getStateMachine().triggerEvent((int)finishEvent);
	}
}

template <typename T, STATE_MACHINE_EVENT finishEvent>
void EntryState<T, finishEvent>::exit(T &t)
{
	t.setInvincible(false);

	MoveState<T>::exit(t);
}

#endif //__HOUND_ENTRY_STATE_H__

