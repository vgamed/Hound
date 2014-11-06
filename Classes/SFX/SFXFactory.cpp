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
	// temporary implementation
	return BulletExplosion::createWithSpriteFrames("d_00", 1, 9, 1.0f, 1.0f);
}
