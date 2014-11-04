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
		float scale, float damage, 
		float speed, bool from_hound)
{
	auto ret = new Bullet();
	if (ret!=nullptr && ret->init(effect, direction, scale, damage, speed, from_hound))
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