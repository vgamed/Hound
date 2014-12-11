#include "Weapon.h"
#include "DrawNodeShape.h"
#include "Battlefield.h"
#include "Hound.h"
#include "Bullet.h"
#include "Laser.h"
#include "Missile.h"

#define WEAPON_DEBUG 1

USING_NS_CC;

Weapon::Weapon(void)
	: m_id(-1)
	, m_type(0)
	, m_autoAim(false)
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

	m_id = info.id;
	m_level = info.level;
	m_type = info.type;
	m_autoAim = info.auto_aim;

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
		barrel.direction  = Vec2::UNIT_Y;
		switch(barrel.info.type)
		{
		case (int)BARREL_TYPE::BULLET:
			barrel.projectile_creator = Bullet::create;
			break;
		case (int)BARREL_TYPE::LASER:
			barrel.projectile_creator = Laser::create;
			break;
		case (int)BARREL_TYPE::MISSILE:
			barrel.projectile_creator = Missile::create;
			break;
		default:
			break;
		}
		m_barrells.push_back(barrel);
	}

#if WEAPON_DEBUG
	// display a dot at the weapon position for debugging
	if (texture == nullptr)
	{
		auto shape = DrawNodeShape::create(DrawNodeShape::SHAPE_TYPE::ARROW, 
			getPosition(), 3.0f, Color4F(1.0f,0.0f,0.0f,1.0f));
		addChild(shape);
	}
#endif //WEAPON_DEBUG

	return true;
}

void Weapon::update(float dt)
{
	if (m_autoAim) // hound weapon will not aim automatically
	{
		aimHound();
	}

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

void Weapon::aimHound(void)
{
	CC_ASSERT(getParent()!=nullptr && getParent()->getParent()!=nullptr);
	auto bf = dynamic_cast<Battlefield*>(getParent()->getParent());
	auto hound = bf->getHound();
	if (hound != nullptr)
	{
		Vec2 dir = bf->convertToWorldSpace(hound->getPosition()) - getParent()->convertToWorldSpace(getPosition());
		setRotation(CC_RADIANS_TO_DEGREES(dir.getAngle(Vec2::UNIT_Y))-getParent()->getRotation());
	}
}

void Weapon::fire(float dt)
{
	CC_ASSERT(getParent()!=nullptr && getParent()->getParent()!=nullptr);

	Battlefield *bf = (Battlefield*)(getParent()->getParent());
	Vec2 start_pos = bf->convertToNodeSpace(getParent()->convertToWorldSpace(getPosition()));
	for (Barrel &barrel : m_barrells)
	{
		barrel.firing_counter += dt;
		if (barrel.firing_counter >= barrel.info.firing_interval)
		{
			float angle = getParent()->getRotation() + getRotation() + barrel.info.rotate_angle;
			barrel.direction = Vec2::UNIT_Y.rotateByAngle(Vec2::ZERO, -CC_DEGREES_TO_RADIANS(angle));
			Projectile *proj = barrel.projectile_creator(barrel.info, 
														barrel.direction, 
														m_damage + barrel.info.projectile_damage, 
														m_speed + barrel.info.projectile_speed, 
														m_isHound);
			if (proj != nullptr)
			{
				bf->addActiveProjectile(proj); // bullet should be child of Battlefield
				proj->setRotation(angle);
				proj->setPosition(start_pos);
			}
			
			barrel.firing_counter = 0.0f;
		}
	}
}
