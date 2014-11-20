#include "FighterBee.h"

USING_NS_CC;

FighterBee::FighterBee(void)
{
	m_states.clear();
}


FighterBee::~FighterBee(void)
{
	for (auto state : m_states)
	{
		if (state != nullptr)
			delete state;
	}
}

Enemy* FighterBee::create(const EnemyInfo &info)
{
	auto ret = new FighterBee();
	if ((ret!=nullptr)&&(ret->init(info)))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool FighterBee::init(const EnemyInfo &info)
{
	if (!Enemy::init(info))
		return false;

	for (const auto &state_info : info.states)
	{
		auto state = new EnemyMoveState(state_info);
		m_states.push_back(state);
	}

	if (m_states.size() > 0)
	{
		EnemyStateTransit trans;
		trans.event = (int)(STATE_MACHINE_EVENT::START);
		trans.from = nullptr;
		trans.to = m_states[0];
		m_stateMachine.addStateTransition(trans);
	}
	
	return true;
}

void FighterBee::update(float dt)
{
	Enemy::update(dt);
}
