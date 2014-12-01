#ifndef __HOUND_ENEMY_FIGHTER_H__
#define __HOUND_ENEMY_FIGHTER_H__

#include "../Common.h"
#include "Weapon.h"
#include "../StateMachine/EntryState.hpp"
#include "../StateMachine/BattlePhaseState.hpp"
#include "../StateMachine/TransformState.hpp"
#include "../StateMachine/LeaveState.hpp"
#include "../StateMachine/DeadState.hpp"
#include "../StateMachine/BattleEndState.hpp"

class Enemy;

typedef StateMachine<Enemy> EnemyStateMachine;
typedef _StateTransition<Enemy> EnemyStateTransit;
typedef State<Enemy> EnemyState;
typedef std::map<int, EnemyState*> EnemyStateMap;
typedef EntryState<Enemy, STATE_MACHINE_EVENT::ENTRY_FINISHED> EnemyEntryState;
typedef BattlePhaseState<Enemy, STATE_MACHINE_EVENT::BATTLE_PHASE_FINISHED, STATE_MACHINE_EVENT::AI_DEAD> EnemyBattlePhaseState;
typedef TransformState<Enemy, STATE_MACHINE_EVENT::TRANSFORM_FINISHED> EnemyTransformState;
typedef LeaveState<Enemy, STATE_MACHINE_EVENT::LEAVE_FINISHED> EnemyLeaveState;
typedef DeadState<Enemy> EnemyDeadState;
typedef BattleEndState<Enemy> EnemyBattleEndState;
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

	virtual const std::vector<Weapon*>& getWeapons(void)
	{ return m_weapons;	}

	virtual EnemyStateMachine& getStateMachine(void)
	{ return m_stateMachine; }

	virtual void doDamage(float damage);

	void setInvincible(bool invincible)
	{ m_invincible = invincible; }
	
	float getCurLife(void) const
	{ return m_curLife; }

	const cocos2d::Vec2& getEntryFrom(void) const
	{ return m_entryFrom; }

	const cocos2d::Vec2& getEntryTo(void) const
	{ return m_entryTo; }

	float getEntrySpeed(void) const
	{ return m_entrySpeed; }

	bool isEntryAutoFacing(void) const
	{ return m_entryAutoFacing; }

	float getLeaveSpeed(void) const
	{ return m_leaveSpeed; }

	bool isLeaveAutoFacing(void) const
	{ return m_leaveAutoFacing; }

protected:
	Enemy(void);
	~Enemy(void);

	virtual bool init(const EnemyInfo &info);

	ENEMY_TYPE m_type;

	cocos2d::Vec2	m_entryFrom;
	cocos2d::Vec2	m_entryTo;
	float			m_entrySpeed;
	bool			m_entryAutoFacing;

	float			m_leaveSpeed;
	bool			m_leaveAutoFacing;

	float	m_curLife;
	float	m_maxLife;
	float	m_armor;
	
	bool	m_invincible; // true - this enemy can not be hurt

	Circle	m_boundingCircle;

	std::vector<Weapon*>	m_weapons;

	EnemyStateMap m_stateMap;
	EnemyStateMachine m_stateMachine;
};

#endif //__HOUND_ENEMY_FIGHTER_H__