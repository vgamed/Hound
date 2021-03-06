#ifndef __HOUND_ENTITY_H__
#define __HOUND_ENTITY_H__

#include "Common.h"
#include "Weapon.h"

#include "StateMachine/EntryState.hpp"
#include "StateMachine/BattlePhaseState.hpp"
#include "StateMachine/TransformState.hpp"
#include "StateMachine/LeaveState.hpp"
#include "StateMachine/DeadState.hpp"
#include "StateMachine/BattleEndState.hpp"

class Entity;

typedef StateMachine<Entity> EntityStateMachine;
typedef _StateTransition<Entity> EntityStateTransit;
typedef State<Entity> EntityState;
typedef std::map<int, EntityState*> EntityStateMap;
typedef EntryState<Entity, STATE_MACHINE_EVENT::ENTRY_FINISHED> EntityEntryState;
typedef BattlePhaseState<Entity, STATE_MACHINE_EVENT::BATTLE_PHASE_FINISHED, STATE_MACHINE_EVENT::DESTROYED> EntityBattlePhaseState;
typedef TransformState<Entity, STATE_MACHINE_EVENT::TRANSFORM_FINISHED> EntityTransformState;
typedef LeaveState<Entity, STATE_MACHINE_EVENT::LEAVE_FINISHED> EntityLeaveState;
typedef DeadState<Entity> EntityDeadState;
typedef BattleEndState<Entity> EntityBattleEndState;
typedef MoveState<Entity> EntityMoveState;

class Entity : public cocos2d::Sprite
{
public:
	// pure virtual functions
	virtual void update(float dt) = 0;
	virtual void doDamage(float damage) = 0;
	virtual void selectTarget(void) = 0;

	// getters and setters
	Entity* getCurrentTarget(void) const
	{
		return m_curTarget;
	}

	ENTITY_TYPE getType(void) const
	{
		return m_type;
	}

	const Circle& getBoundingCircle(void) 
	{
		m_boundingCircle.center = getPosition();
		return m_boundingCircle; 
	}
	
	float getBoundingCircleRadius(void) const
	{
		return m_boundingCircle.radius;
	}

	virtual const std::vector<Weapon*>& getWeapons(void)
	{
		return m_weapons;	
	}

	virtual EntityStateMachine& getStateMachine(void)
	{
		return m_stateMachine; 
	}

	void setInvincible(bool invincible)
	{ 
		m_invincible = invincible; 
	}

	float getCurLife(void) const
	{
		return m_curLife; 
	}

	const cocos2d::Vec2& getEntryFrom(void) const
	{
		return m_entryFrom; 
	}

	const cocos2d::Vec2& getEntryTo(void) const
	{
		return m_entryTo; 
	}

	float getEntrySpeed(void) const
	{
		return m_entrySpeed; 
	}

	bool isEntryAutoFacing(void) const
	{
		return m_entryAutoFacing; 
	}

	float getLeaveSpeed(void) const
	{
		return m_leaveSpeed; 
	}

	bool isLeaveAutoFacing(void) const
	{
		return m_leaveAutoFacing; 
	}

	bool isDead(void) const
	{
		return m_stateMachine.isInState((int)STATE_TYPE::DEAD);
	}

	bool isGone(void) const
	{
		return (m_stateMachine.isInState((int)STATE_TYPE::LEAVE) &&
				m_stateMachine.getCurrentState().isDone());
	}

protected:
	Entity(void)
		: m_type(ENTITY_TYPE::NONE)
		, m_stateMachine(*this)
		, m_invincible(false)
		, m_curTarget(nullptr)
	{
		m_stateMap.clear();
	}

	Entity(ENTITY_TYPE type)
		: m_type(type)
		, m_stateMachine(*this)
		, m_invincible(false)
		, m_curTarget(nullptr)
	{
		m_stateMap.clear();
	}

	Entity(int type)
		: m_type((ENTITY_TYPE)type)
		, m_stateMachine(*this)
		, m_invincible(false)
		, m_curTarget(nullptr)
	{
		m_stateMap.clear();
	}

	~Entity(void) 
	{
		for (auto state : m_stateMap)
		{
			if (state.second != nullptr)
				delete state.second;
		}
	}

	ENTITY_TYPE m_type;

	cocos2d::Vec2	m_entryFrom;
	cocos2d::Vec2	m_entryTo;
	float			m_entrySpeed;
	bool			m_entryAutoFacing;

	float			m_leaveSpeed;
	bool			m_leaveAutoFacing;

	float	m_curLife;
	float	m_maxLife;
	float	m_armor;
	bool	m_invincible; // true - Entity can not be hurt

	Circle	m_boundingCircle;

	std::vector<Weapon*>	m_weapons;

	EntityStateMap m_stateMap;
	EntityStateMachine m_stateMachine;

	Entity	*m_curTarget;
};

#endif //__HOUND_ENTITY_H__
