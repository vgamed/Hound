#ifndef __HOUND_BULLET_EXPLOSION_H__
#define __HOUND_BULLET_EXPLOSION_H__

#include "SFXExplosion.h"

class BulletExplosion :
	public SFXExplosion
{
public:
	static SFXExplosion* createWithSpriteFrames(const std::string &sf_name, int start_id, int number, 
		float last, float scale);
	static SFXExplosion* createWithAnimation(const std::string &anim_name, float last, float scale);


protected:
	BulletExplosion(void);
	~BulletExplosion(void);
};

#endif //__HOUND_BULLET_EXPLOSION_H__