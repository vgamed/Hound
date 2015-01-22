#include "Projectile.h"
#include "Laser.h"

USING_NS_CC;

Laser::Laser(void)
{
}


Laser::~Laser(void)
{
}

Projectile* Laser::create(const BarrelInfo &info, 
						  const cocos2d::Vec2 &direction, 
						  bool from_hound)
{
	auto ret = new Laser();
	//speed is treated as damage interval for Laser type barrells
	if (ret!=nullptr && !ret->init(info, direction, from_hound))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Laser::update(float dt)
{
	// movement update
	//...

	// collision detect
	Projectile::update(dt);
}