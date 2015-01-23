#include "EntityStateFactory.h"

EntityStateFactory::EntityStateFactory(void)
{
}


EntityStateFactory::~EntityStateFactory(void)
{
}

EntityState* EntityStateFactory::create(const StateInfo &info)
{
	switch(info.type)
	{
	case (int)STATE_TYPE::ENTRY:
		return new EntityEntryState(info);
	case (int)STATE_TYPE::BATTLE_PHASE:
		return new EntityBattlePhaseState(info);
	case (int)STATE_TYPE::TRANSFORM:
		return nullptr;
	case (int)STATE_TYPE::LEAVE:
		return new EntityLeaveState(info);
	case (int)STATE_TYPE::DEAD:
		return new EntityDeadState(info);
	case (int)STATE_TYPE::BATTLE_END:
		return new EntityBattleEndState(info);
	case (int)STATE_TYPE::MOVE:
		return new EntityMoveState(info);
	default:
		return nullptr;
	}
}
