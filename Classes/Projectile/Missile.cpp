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
	if (ret!=nullptr && ret->init(info, direction, target))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Missile::update(float dt)
{
	// update runtime speed and steering speed
	m_runtime_speed += m_runtime_acceleration * dt;
	m_steering_speed += m_steering_acceleration * dt;

	// calculate runtime delta displacement and steering delta displacement
	float step = m_runtime_speed * dt;
	float steering_step = m_steering_speed * dt;

	// calculate targeting direction
	Vec2 target_dir = Vec2::ZERO;
	if ((m_target != nullptr) && !m_target->isDead())
	{
		target_dir = m_target->getPosition() - this->getPosition();
		target_dir.normalize();
	}

	// calculate displacement
	m_runtime_direction = target_dir * steering_step + m_runtime_direction * step;
	setPosition(getPosition() + m_runtime_direction);

	// calculate rotation
	m_runtime_direction.normalize();
	setRotation(CC_RADIANS_TO_DEGREES(m_runtime_direction.getAngle(Vec2::UNIT_Y)));

	// collision detect
	Projectile::update(dt);
}