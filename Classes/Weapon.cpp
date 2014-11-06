#include "Weapon.h"
#include "Battlefield.h"
#include "Hound.h"
#include "Projectile/Bullet.h"
#include "Projectile/Laser.h"
#include "Projectile/Missile.h"

USING_NS_CC;

Weapon::Weapon(void)
{
}


Weapon::~Weapon(void)
{
}

Weapon* Weapon::create(const WeaponInfo &info, bool is_hound)
{
	auto weapon = new Weapon();
	if (weapon && weapon->init(info))
	{
		weapon->m_isHound = is_hound;
		weapon->autorelease();
		return weapon;
	}
	CC_SAFE_DELETE(weapon);
	return nullptr;
}

bool Weapon::init(const WeaponInfo &info)
{
	Texture2D *texture = 
		Director::getInstance()->getTextureCache()->getTextureForKey(info.texture_name);
	if (!initWithTexture(texture))
	{
		return false;
	}

	m_level = info.level;
	m_type = info.type;

	m_firingCounter = 0.0f;
	m_firingInterval = info.firing_interval;
	m_projectileDamage = info.projectile_damage;
	m_projectileSpeed = info.projectile_speed;

	// initialize barrells
	Barrel barrel;
	barrel.projectile_startpoint = Vec2(0.0f,0.0f); // make this customizable?
	for (BarrelInfo binfo : info.barrells)
	{
		barrel.info = binfo;
		barrel.direction  = Vec2::UNIT_Y.rotateByAngle(Vec2::ZERO, -CC_DEGREES_TO_RADIANS(barrel.info.rotate_angle));
		switch(barrel.info.type)
		{
		case BARREL_TYPE::BULLET:
			barrel.projectile_creator = Bullet::create;
			break;
		case BARREL_TYPE::LASER:
			barrel.projectile_creator = Laser::create;
			break;
		case BARREL_TYPE::MISSILE:
			barrel.projectile_creator = Missile::create;
			break;
		default:
			break;
		}
		m_barrells.push_back(barrel);
	}

	return true;
}

void Weapon::update(float dt)
{
	m_firingCounter += dt;
	if( m_firingCounter >= m_firingInterval )
	{
		fire();
		m_firingCounter = 0.0f;
	}
}

void Weapon::fire(void)
{
	CC_ASSERT(getParent()!=nullptr && getParent()->getParent()!=nullptr);

	Battlefield *bf = (Battlefield*)(getParent()->getParent());

	for (Barrel &b : m_barrells)
	{
		Projectile *proj = b.projectile_creator(b.info, b.direction, 
			m_projectileDamage, m_projectileSpeed, m_isHound);
		if (proj != nullptr)
		{
			bf->addActiveProjectile(proj); // bullet should be child of Battlefield
			proj->setRotation(b.info.rotate_angle);
			proj->setPosition(b.projectile_startpoint);
		}
	}
}

void Weapon::updateProjectileStartPoints(void)
{
	CC_ASSERT(getParent()!=nullptr && getParent()->getParent()!=nullptr);

	Node *fighter = getParent();
	Node *battlefield = fighter->getParent();

	for (Barrel &b : m_barrells)
	{
		// projectile start point needs to be updated after the hound moved
		b.projectile_startpoint = getPosition();
		b.projectile_startpoint = fighter->convertToWorldSpace(b.projectile_startpoint);
		b.projectile_startpoint = battlefield->convertToNodeSpace(b.projectile_startpoint);
	}
}
