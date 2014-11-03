#include "Weapon.h"
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

Weapon* Weapon::create(const WeaponInfo &info)
{
	auto weapon = new Weapon();
	if (weapon && weapon->init(info))
	{
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
	//m_firingInterval = info.firing_interval;
	//m_bulletDamage = info.bullet_damage;
	//m_bulletSpeed = info.bullet_speed;
	m_firingInterval = 0.1f; //second
	m_bulletSpeed = 1000.0f; //per second
	m_bulletDamage = 10.0f;

	// initialize barrells
	Barrel barrel;
	barrel.projectile_startpoint = Vec2(0.0f,0.0f);
	std::vector<BarrelInfo>::const_iterator cit;
	for (cit=info.barrells.begin(); cit!=info.barrells.end(); ++cit)
	{
		barrel.type = (*cit).type;
		barrel.rotate_angle = (*cit).rotate_angle;
		barrel.direction  = Vec2::UNIT_Y.rotateByAngle(Vec2::ZERO, -CC_DEGREES_TO_RADIANS(barrel.rotate_angle));
		barrel.effect_name = (*cit).effect_name;
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
	std::vector<Barrel>::iterator it;
	for (it=m_barrells.begin(); it!=m_barrells.end(); it++)
	{
		Barrel *b = &(*it);
		Projectile *proj = b->projectile_creator(b->effect_name, b->direction, m_bulletDamage, m_bulletSpeed);
		if (proj != nullptr)
		{
			getParent()->getParent()->addChild(proj); // bullet should be child of Battlefield
			proj->setRotation(b->rotate_angle);
			proj->setPosition(b->projectile_startpoint);
		}
	}
}

void Weapon::updateProjectileStartPoints(void)
{
	CC_ASSERT(this->getParent()!=nullptr);

	std::vector<Barrel>::iterator it;
	for (it=m_barrells.begin(); it!=m_barrells.end(); it++)
	{
		// projectile start point needs to be updated after the hound moved
		Barrel *p = &(*it);
		p->projectile_startpoint = getPosition();
		p->projectile_startpoint = getParent()->convertToWorldSpace(p->projectile_startpoint);
		p->projectile_startpoint = getParent()->getParent()->convertToNodeSpace(p->projectile_startpoint);
	}
}
