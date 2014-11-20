#ifndef __HOUND_ENEMY_FIGHTER_H__
#define __HOUND_ENEMY_FIGHTER_H__

#include "../Common.h"
#include "../StateMachine/MoveState.hpp"

class Enemy :
	public cocos2d::Sprite
{
public:
	virtual void update(float dt);

	virtual ENEMY_TYPE getType(void) { return m_type; }

	virtual const Circle& getBoundingCircle(void) 
	{
		m_boundingCircle.center = getPosition();
		return m_boundingCircle; 
	}

	virtual StateMachine<Enemy>& getStateMachine(void)
	{ return m_stateMachine; }

	virtual void doDamage(float damage);

	bool isDead(void) { return (m_curLife<=0.0f); }

protected:
	Enemy(void);
	~Enemy(void);

	virtual bool init(const EnemyInfo &info);

	ENEMY_TYPE m_type;

	float	m_curLife;
	float	m_maxLife;
	float	m_armor;

	Circle	m_boundingCircle;

	StateMachine<Enemy> m_stateMachine;
};

typedef StateMachine<Enemy> EnemyStateMachine;
typedef StateTransit<Enemy> EnemyStateTransit;
typedef State<Enemy> EnemyState;
typedef MoveState<Enemy> EnemyMoveState;

#endif //__HOUND_ENEMY_FIGHTER_H__