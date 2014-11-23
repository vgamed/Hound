#include "EnemyStateFactory.h"

EnemyStateFactory::EnemyStateFactory(void)
{
}


EnemyStateFactory::~EnemyStateFactory(void)
{
}

EnemyState* EnemyStateFactory::create(const StateInfo &info)
{
	switch(info.type)
	{
	case STATE_TYPE::ENTRY:
		return new EnemyEntryState(info);
	case STATE_TYPE::BATTLE_PHASE:
		return nullptr;
	case STATE_TYPE::TRANSFORM:
		return nullptr;
	case STATE_TYPE::LEAVE:
		return new EnemyLeaveState(info);
	case STATE_TYPE::DEAD:
		return nullptr;
	case STATE_TYPE::HOUND_DEAD:
		return nullptr;
	case STATE_TYPE::MOVE:
		return new EnemyMoveState(info);
	default:
		return nullptr;
	}
}
