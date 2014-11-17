#ifndef __HOUND_FIGHTER_BEE_H__
#define __HOUND_FIGHTER_BEE_H__

#include "Enemy.h"
#include "../StateMachine/MoveState.hpp"

class FighterBee :
	public Enemy
{
public:
	static Enemy* create(const EnemyInfo &info);
	void update(float dt);

protected:
	FighterBee(void);
	~FighterBee(void);

private:
	typedef std::vector<State<FighterBee>> FIGHTBEE_STATES;
	
	FIGHTBEE_STATES m_states;
	StateMachine<FighterBee> m_stateMachine;

	MoveState<FighterBee> state;
};

#endif //__HOUND_FIGHTER_BEE_H__