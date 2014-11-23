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

	bool ret = false;
	if (texture == nullptr)
		ret = Sprite::init();
	else
		ret = Sprite::initWithTexture(texture);

	if (!ret)
	{
		return false;
	}

	m_level = info.level;
	m_type = info.type;

	m_timeCounter = 0.0f;
	m_timeOffsetFiringStart = info.time_offset_firing_start;
	m_timeOffsetFiringStop = info.time_offset_firing_stop;

	m_damage = info.damage;
	m_speed = info.speed;

	// initialize barrells
	Barrel barrel;
	barrel.firing_counter = 0.0f;
	for (BarrelInfo binfo : info.barrells)
	{
		barrel.info = binfo;
		barrel.direction  = Vec2::UNIT_Y.rotateByAngle(Vec2::ZERO, 
			-CC_DEGREES_TO_RADIANS(info.rotate_angle+barrel.info.rotate_angle));
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
	CC_ASSERT(m_timeOffsetFiringStart<=m_timeOffsetFiringStop);

	if (m_timeCounter < m_timeOffsetFiringStop)
	{
		m_timeCounter += dt;
		if( m_timeCounter >= m_timeOffsetFiringStart )
		{
			fire(dt);
		}
	}
}

void Weapon::fire(float dt)
{
	CC_ASSERT(getParent()!=nullptr && getParent()->getParent()!=nullptr);

	Battlefield *bf = (Battlefield*)(getParent()->getParent());
	Vec2 start_pos = bf->convertToNodeSpace(getParent()->convertToWorldSpace(getPosition()));
	for (Barrel &b : m_barrells)
	{
		b.firing_counter += dt;
		if (b.firing_counter >= b.info.firing_interval)
		{
			Projectile *proj = b.projectile_creator(b.info, b.direction, 
				m_damage+b.info.projectile_damage, 
				m_speed+b.info.projectile_speed, m_isHound);
			if (proj != nullptr)
			{
				bf->addActiveProjectile(proj); // bullet should be child of Battlefield
				proj->setRotation(getRotation()+b.info.rotate_angle);
				proj->setPosition(start_pos);
			}
			
			b.firing_counter = 0.0f;
		}
	}
}
