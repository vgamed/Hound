#include "Laser.h"

USING_NS_CC;

Laser::Laser(void)
{
}


Laser::~Laser(void)
{
}

Projectile* Laser::create(const std::string effect, 
		const cocos2d::Vec2 &direction,
		float damage, float speed)
{
	auto ret = new Laser();
	if (ret!=nullptr && !ret->init(effect, direction, 0.0f, damage)) //speed doesn't matter for laser
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Laser::update(float dt)
{
}