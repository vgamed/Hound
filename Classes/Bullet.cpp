#include "Bullet.h"

USING_NS_CC;

Bullet::Bullet(void)
{
}


Bullet::~Bullet(void)
{
}

Projectile* Bullet::create(const std::string effect, 
		const cocos2d::Vec2 &direction,
		float damage, float speed)
{
	auto ret = new Bullet();
	if (ret!=nullptr && ret->init(effect, direction, damage, speed))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Bullet::update(float dt)
{
	// movement update
	setPosition(getPosition()+m_speed*dt*m_direction);
}