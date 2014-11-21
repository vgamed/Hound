#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

// State definition
template <typename T>
class State
{
public:
	virtual ~State(void) {}

	virtual void enter(T &t){}
	virtual void exec(T &t, float dt){}
	virtual void exit(T &t){}

	int getId(void) { return m_id; }
	void setId(int id) { m_id = id; }

	int getType(void) { return m_type; }
	void setType(int type) { m_type = type; }

protected:
	State(void) {}
	State& operator = (const State &other);

private:
	int m_id;
	int m_type;
};

// define the information structure of transition among states
template <typename T>
struct _StateTransition
{
	int	event;
	State<T> *from;
	State<T> *to;
};

// StateMachine definition
template <typename T>
class StateMachine
{
public:
	typedef _StateTransition<T> StateTransition;
	typedef std::multimap<int, StateTransition> StateMultimap;

	StateMachine(T &owner)
		: m_rOwner(owner)
		, m_pPreState(nullptr)
		, m_pCurState(nullptr)
	{}

	bool addStateTransition(const StateTransition &trans);

	void triggerEvent(int event);

	void update(float dt);

	State<T> &getCurrentState(void)
	{ return *m_pCurState; }

	State<T> &getPreviousState(void)
	{ return *m_pPreState; }

	bool isInState(State<T> &state) 
	{ return (m_pCurState==&state); }

private:
	void changeState(State<T> &state);

	void setCurrentState(State<T> &state) 
	{
		m_pCurState = &state; 
		m_pCurState->enter(m_rOwner);
	}

	void setPreviousState(State<T> &state)
	{
		m_pPreState = &state; 
	}

	void revertToPreviousState(void)
	{
		changeState(*m_pPreState);
	}

private:
	T &m_rOwner; // owner of the state machine
	
	State<T> *m_pPreState; // pointer to the previous state
	State<T> *m_pCurState; // pointer to the current state

	StateMultimap m_stateMap;
	StateMultimap m_stateMapAny;
};

template <typename T> 
bool StateMachine<T>::addStateTransition(const StateTransition &trans)
{
	if (trans.from == nullptr)
	{	// this transition doesn't care about the from state
		// thus take the high priority
		m_stateMapAny.insert(std::make_pair(trans.event, trans));
	}
	else
	{
		m_stateMap.insert(std::make_pair(trans.event, trans));
	}
	return true;
}

template <typename T> 
void StateMachine<T>::triggerEvent(int event)
{
	if (m_stateMapAny.size() > 0)
	{
		auto range = m_stateMapAny.equal_range(event);
		changeState(*(range.first->second.to));
	}
	if (m_stateMap.size() > 0)
	{
		auto range = m_stateMapAny.equal_range(event);
		for (auto it=range.first; it!=range.second; ++it)
		{
			if (it->second.from == m_pCurState)
			{
				changeState(*(it->second.to));
				break;
			}
		}
	}
}

template <typename T> 
void StateMachine<T>::update(float dt) 
{
	if( m_pCurState != nullptr)
		m_pCurState->exec(m_rOwner, dt);
}

template <typename T> 
void StateMachine<T>::changeState(State<T> &state)
{
	if (m_pCurState != nullptr)
	{
		m_pCurState->exit(m_rOwner);
		setPreviousState(*m_pCurState);
	}
	setCurrentState(state);
}

#endif //__STATE_MACHINE_H__

