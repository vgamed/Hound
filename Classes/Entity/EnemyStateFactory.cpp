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
	case (int)STATE_TYPE::ENTRY:
		return new EnemyEntryState(info);
	case (int)STATE_TYPE::BATTLE_PHASE:
		return new EnemyBattlePhaseState(info);
	case (int)STATE_TYPE::TRANSFORM:
		return nullptr;
	case (int)STATE_TYPE::LEAVE:
		return new EnemyLeaveState(info);
	case (int)STATE_TYPE::DEAD:
		return new EnemyDeadState(info);
	case (int)STATE_TYPE::BATTLE_END:
		return new EnemyBattleEndState(info);
	case (int)STATE_TYPE::MOVE:
		return new EnemyMoveState(info);
	default:
		return nullptr;
	}
}
