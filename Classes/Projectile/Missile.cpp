#include "Missile.h"

USING_NS_CC;

Missile::Missile(void)
{
}


Missile::~Missile(void)
{
}

Projectile* Missile::create(const BarrelInfo &info, 
							const cocos2d::Vec2 &direction, 
							Entity *target)
{
	auto ret = new Missile();
	if (ret!=nullptr && !ret->init(info, direction, target))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Missile::update(float dt)
{
	// movement update
	//...

	// collision detect
	Projectile::update(dt);
}