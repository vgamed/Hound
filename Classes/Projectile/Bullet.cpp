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
						   Entity *target)
{
	auto ret = new Bullet();
	if (ret!=nullptr && ret->init(info, direction, target))
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
	m_runtime_speed += m_runtime_acceleration * dt;
	setPosition(getPosition() + m_runtime_speed*dt*m_runtime_direction);

	// collision detect
	Projectile::update(dt);
}