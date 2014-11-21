#ifndef __HOUND_ENEMY_FIGHTER_H__
#define __HOUND_ENEMY_FIGHTER_H__

#include "../Common.h"
#include "../StateMachine/MoveState.hpp"

class Enemy;

typedef StateMachine<Enemy> EnemyStateMachine;
typedef _StateTransition<Enemy> EnemyStateTransit;
typedef State<Enemy> EnemyState;
typedef std::map<int, EnemyState*> EnemyStateMap;
typedef MoveState<Enemy> EnemyMoveState;

class Enemy :
	public cocos2d::Sprite
{
public:
	virtual void update(float dt);

	virtual ENEMY_TYPE getType(void) 
	{ return m_type; }

	virtual const Circle& getBoundingCircle(void) 
	{
		m_boundingCircle.center = getPosition();
		return m_boundingCircle; 
	}

	virtual EnemyStateMachine& getStateMachine(void)
	{ return m_stateMachine; }

	virtual void doDamage(float damage);

	void setInvincible(bool invincible)
	{ m_invincible = invincible; }
	
	bool isDead(void) 
	{ return (m_curLife<=0.0f); }

protected:
	Enemy(void);
	~Enemy(void);

	virtual bool init(const EnemyInfo &info);

	ENEMY_TYPE m_type;

	float	m_curLife;
	float	m_maxLife;
	float	m_armor;
	bool	m_invincible;

	Circle	m_boundingCircle;

	EnemyStateMachine m_stateMachine;
};

#endif //__HOUND_ENEMY_FIGHTER_H__