#ifndef __HOUND_SFX_FACTORY_H__
#define __HOUND_SFX_FACTORY_H__

#include "SFXExplosion.h"

class SFXFactory
{
public:
	static SFXExplosion* createProjectileSFX(PROJECTILE_TYPE type);
	static SFXExplosion* createEnemyExplosionSFX(ENEMY_TYPE type);

private:
	SFXFactory(void);
	~SFXFactory(void);
};

#endif //__HOUND_SFX_FACTORY_H__