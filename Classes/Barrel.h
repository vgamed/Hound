#ifndef __HOUND_BARREL_H__
#define __HOUND_BARREL_H__

#include "Projectile.h"

class Barrel
{
public:
	Barrel(void);
	~Barrel(void);

	void fire(float damage, float speed);

private:
	int		m_type; // currently consistent with weapon type, reserved for future potential usage
	
	cocos2d::Vec2 m_direction;
};

#endif //__HOUND_BARREL_H__