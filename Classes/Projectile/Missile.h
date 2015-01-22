#ifndef __HOUND_MISSILE_H__
#define __HOUND_MISSILE_H__

#include "Projectile.h"

class Missile :	public Projectile
{
public:
	static Projectile* create(const BarrelInfo &info, 
		const cocos2d::Vec2 &direction, bool from_hound);

	void update(float dt);

protected:
	Missile(void);
	virtual ~Missile(void);
};

#endif //__HOUND_MISSILE_H__
