#ifndef __HOUND_LASER_H__
#define __HOUND_LASER_H__

#include "Projectile.h"

class Laser : public Projectile
{
public:
	static Projectile* create(const std::string effect, 
		const cocos2d::Vec2 &direction,
		float scale, float damage, 
		float speed, bool from_hound);

	void update(float dt);

protected:
	Laser(void);
	virtual ~Laser(void);
};

#endif //__HOUND_LASER_H__
