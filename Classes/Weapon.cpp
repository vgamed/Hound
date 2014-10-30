#include "Weapon.h"

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

	//m_firingInterval = info.firing_interval;
	m_firingCounter = 0.0f;
	//m_bulletDamage = info.bullet_damage;
	//m_bulletSpeed = info.bullet_speed;

	// initialize barrells
	//m_barrells...

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
	std::vector<Barrel*>::iterator it;
	for (it=m_barrells.begin(); it!=m_barrells.end(); it++)
	{
		(*it)->fire(m_bulletDamage, m_bulletSpeed);
	}
}