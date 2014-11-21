#include "FighterBee.h"
#include "EnemyStateFactory.h"

USING_NS_CC;

FighterBee::FighterBee(void)
{
	m_stateMap.clear();
}


FighterBee::~FighterBee(void)
{
	for (auto state : m_stateMap)
	{
		if (state.second != nullptr)
			delete state.second;
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

	for (const auto &state_info : info.state_infoes)
	{
		auto state = EnemyStateFactory::create(state_info);
		m_stateMap.insert(std::make_pair(state->getId(), state));
	}

	for (const auto &state_map_info : info.state_map_infoes)
	{
		EnemyStateTransit trans;
		trans.event = (int)(state_map_info.event);
		auto it = m_stateMap.find(state_map_info.from);
		if (it == m_stateMap.end())
		{
			trans.from = nullptr;
		}
		else
		{
			trans.from = it->second;
			CC_ASSERT(trans.from->getId() == state_map_info.from);
		}

		it = m_stateMap.find(state_map_info.to);
		CC_ASSERT(it != m_stateMap.end());
		trans.to = it->second;
		CC_ASSERT(trans.to->getId() == state_map_info.to);
		
		m_stateMachine.addStateTransition(trans);
	}
	
	return true;
}