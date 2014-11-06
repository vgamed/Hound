#include "SFXFactory.h"
#include "BulletExplosion.h"

SFXFactory::SFXFactory(void)
{
}


SFXFactory::~SFXFactory(void)
{
}

SFXExplosion* SFXFactory::createProjectileSFX(PROJECTILE_TYPE type)
{
	switch(type)
	{
	case PROJECTILE_TYPE::BULLET_NORMAL:
		return BulletExplosion::createWithSpriteFrames("a_", 1, 8, 0.5f, 0.8f);
	case PROJECTILE_TYPE::LASER_NORMAL:
		return nullptr;
	case PROJECTILE_TYPE::MISSILE_NORMAL:
		return nullptr;
	default:
		return nullptr;
	}
}

SFXExplosion* SFXFactory::createEnemyExplosionSFX(ENEMY_TYPE type)
{
	switch(type)
	{
	case ENEMY_TYPE::FIGHTER_BEE:
		return BulletExplosion::createWithSpriteFrames("d_00", 1, 9, 1.0f, 1.0f);
	case ENEMY_TYPE::FRIGATE_PUMA:
		return BulletExplosion::createWithSpriteFrames("d_00", 1, 9, 1.0f, 1.2f);
	case ENEMY_TYPE::CARRIER_TIGER:
		return BulletExplosion::createWithSpriteFrames("d_00", 1, 9, 1.0f, 2.0f);
	default:
		return nullptr;
	}
}
