#include "Missile.h"

USING_NS_CC;

Missile::Missile(void)
{
}


Missile::~Missile(void)
{
}

Projectile* Missile::create(const std::string effect, 
		const cocos2d::Vec2 &direction,
		float damage, float speed)
{
	auto ret = new Missile();
	if (ret!=nullptr && !ret->init(effect, direction, speed, damage))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Missile::update(float dt)
{
}