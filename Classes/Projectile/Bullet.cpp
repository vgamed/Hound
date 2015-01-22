#include "Bullet.h"

USING_NS_CC;

Bullet::Bullet(void)
{
}


Bullet::~Bullet(void)
{
}

Projectile* Bullet::create(const BarrelInfo &info, 
						   const cocos2d::Vec2 &direction, 
						   bool from_hound)
{
	auto ret = new Bullet();
	if (ret!=nullptr && ret->init(info, direction, from_hound))
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

	// collision detect
	Projectile::update(dt);
}