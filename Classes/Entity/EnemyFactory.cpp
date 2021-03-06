#include "EnemyFactory.h"
#include "FighterBee.h"
#include "FrigatePuma.h"
#include "CarrierTiger.h"

EnemyFactory::EnemyFactory(void)
{
}


EnemyFactory::~EnemyFactory(void)
{
}

Enemy* EnemyFactory::create(const EnemyInfo &info)
{
	switch(info.type)
	{
	case (int)ENEMY_TYPE::FIGHTER_BEE:
		return FighterBee::create(info);
	case (int)ENEMY_TYPE::FRIGATE_PUMA:
		return FrigatePuma::create(info);
	case (int)ENEMY_TYPE::CARRIER_TIGER:
		return CarrierTiger::create(info);
	default:
		return nullptr;
	}
}
