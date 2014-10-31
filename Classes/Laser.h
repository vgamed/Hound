#ifndef __HOUND_LASER_H__
#define __HOUND_LASER_H__

#include "Common.h"
#include "Projectile.h"

class Laser :
	public Projectile
{
public:
	static Projectile* create(const std::string effect, 
		const cocos2d::Vec2 &direction,
		float damage, float speed);

	void update(float dt);

protected:
	Laser(void);
	virtual ~Laser(void);
};

#endif //__HOUND_LASER_H__
