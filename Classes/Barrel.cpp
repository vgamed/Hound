#include "Barrel.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Laser.h"
#include "Missile.h"

USING_NS_CC;

Barrel::Barrel(Weapon* weapon, BARREL_TYPE type, 
		const cocos2d::Vec2 &direction,
		const std::string &effect_name)
	: m_direction(direction)
	, m_type(type)
	, m_effectName(effect_name)
	, m_weaponAssociated(weapon)
{
	// init projectile creator
	switch(m_type)
	{
	case BARREL_BULLET:
		m_projectileCreator = Bullet::create;
		break;
	case BARREL_LASER:
		m_projectileCreator = Laser::create;
		break;
	case BARREL_MISSILE:
		m_projectileCreator = Missile::create;
		break;
	}
}

void Barrel::updateProjectileStartPoint(void)
{
	CC_ASSERT((m_weaponAssociated!=nullptr)&&(m_weaponAssociated->getParent()!=nullptr));

	// init projectile start point
	Layer* layer = m_weaponAssociated->getBattlefieldLayer();
	m_projectileStartPoint = m_weaponAssociated->getPosition();
	m_projectileStartPoint = m_weaponAssociated->getParent()->convertToWorldSpace(m_projectileStartPoint);
	m_projectileStartPoint = layer->convertToNodeSpace(m_projectileStartPoint);
}

Barrel::~Barrel(void)
{
}

void Barrel::fire(float damage, float speed)
{
	Projectile *proj = m_projectileCreator(m_effectName, m_direction, damage, speed);
	if (proj != nullptr)
	{
		m_weaponAssociated->getBattlefieldLayer()->addChild(proj);
		proj->setPosition(m_projectileStartPoint);
	}
}

void Barrel::removeProjectile(const Projectile *proj)
{
}
