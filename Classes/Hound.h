#ifndef __HOUND_H__
#define __HOUND_H__

#include "Common.h"
#include "Wingman.h"
#include "Weapon.h"

#include "StateMachine/EntryState.hpp"
#include "StateMachine/BattlePhaseState.hpp"
#include "StateMachine/TransformState.hpp"
#include "StateMachine/LeaveState.hpp"
#include "StateMachine/DeadState.hpp"
#include "StateMachine/BattleEndState.hpp"

class Hound;

typedef StateMachine<Hound> HoundStateMachine;
typedef _StateTransition<Hound> HoundStateTransit;
typedef State<Hound> HoundState;
typedef std::map<int, HoundState*> HoundStateMap;
typedef EntryState<Hound, STATE_MACHINE_EVENT::ENTRY_FINISHED> HoundEntryState;
typedef BattlePhaseState<Hound, STATE_MACHINE_EVENT::BATTLE_PHASE_FINISHED, STATE_MACHINE_EVENT::HOUND_DEAD> HoundBattlePhaseState;
typedef TransformState<Hound, STATE_MACHINE_EVENT::TRANSFORM_FINISHED> HoundTransformState;
typedef LeaveState<Hound, STATE_MACHINE_EVENT::LEAVE_FINISHED> HoundLeaveState;
typedef DeadState<Hound> HoundDeadState;
typedef BattleEndState<Hound> HoundBattleEndState;
typedef MoveState<Hound> HoundMoveState;

class Hound : public cocos2d::Sprite
{
public:
	static Hound* create(const HoundInfo &hdi, const LevelInfo &lli);

	void update(float dt);

	//bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	//void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	
	void doDamage(float damage);

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
	{ return m_weapons;	}

	virtual HoundStateMachine& getStateMachine(void)
	{ return m_stateMachine; }

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

	static const int TAG;
	
	CC_SYNTHESIZE(int, m_touchID, TouchID);

protected:
	Hound(void);
	~Hound(void);

	bool init(const HoundInfo &hdi, const LevelInfo &lli);

private:
	int				m_bodyType;
	int				m_bodyLevel;

	//Armor*			m_armor;
	//Engine*			m_engine;

	cocos2d::Vec2	m_entryFrom;
	cocos2d::Vec2	m_entryTo;
	float			m_entrySpeed;
	bool			m_entryAutoFacing;

	float			m_leaveSpeed;
	bool			m_leaveAutoFacing;

	float	m_curLife;
	float	m_maxLife;
	float	m_armor;
	bool	m_invincible; // true - Hound can not be hurt

	Circle	m_boundingCircle;

	std::vector<Weapon*>	m_weapons;

	Wingman *m_wingmanLeft;
	Wingman *m_wingmanRight;

	HoundStateMap m_stateMap;
	HoundStateMachine m_stateMachine;
};

#endif

