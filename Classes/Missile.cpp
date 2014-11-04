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
		float scale, float damage, 
		float speed, bool from_hound)
{
	auto ret = new Missile();
	if (ret!=nullptr && !ret->init(effect, direction, scale, damage, speed, from_hound))
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