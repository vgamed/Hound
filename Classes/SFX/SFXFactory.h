#ifndef __HOUND_SFX_FACTORY_H__
#define __HOUND_SFX_FACTORY_H__

#include "SFXExplosion.h"

class SFXFactory
{
public:
	static SFXExplosion* createProjectileSFX(int type);
	static SFXExplosion* createEnemyExplosionSFX(int type);
	static SFXExplosion* createHoundExplosionSFX(void);

private:
	SFXFactory(void);
	~SFXFactory(void);
};

#endif //__HOUND_SFX_FACTORY_H__