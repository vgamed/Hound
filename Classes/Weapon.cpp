#include "Weapon.h"
#include "Battlefield.h"
#include "Hound.h"
#include "Bullet.h"
#include "Laser.h"
#include "Missile.h"

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
	m_bulletDamage = info.bullet_damage;
	m_bulletSpeed = info.bullet_speed;

	// initialize barrells
	Barrel barrel;
	barrel.projectile_startpoint = Vec2(0.0f,0.0f); // make this customizable?
	for (BarrelInfo binfo : info.barrells)
	{
		barrel.type = binfo.type;
		barrel.rotate_angle = binfo.rotate_angle;
		barrel.direction  = Vec2::UNIT_Y.rotateByAngle(Vec2::ZERO, -CC_DEGREES_TO_RADIANS(barrel.rotate_angle));
		barrel.effect_name = binfo.effect_name;
		barrel.projectile_scale_xy = binfo.projectile_scale_xy; 
		switch(barrel.type)
		{
		case BARREL_BULLET:
			barrel.projectile_creator = Bullet::create;
			break;
		case BARREL_LASER:
			barrel.projectile_creator = Laser::create;
			break;
		case BARREL_MISSILE:
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
		Projectile *proj = b.projectile_creator(b.effect_name, b.direction, 
			b.projectile_scale_xy, m_bulletDamage, m_bulletSpeed, m_isHound);
		if (proj != nullptr)
		{
			bf->addProjectile(proj); // bullet should be child of Battlefield
			proj->setRotation(b.rotate_angle);
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
