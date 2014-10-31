#include "Weapon.h"

USING_NS_CC;

Weapon::Weapon(void)
{
}


Weapon::~Weapon(void)
{
	std::vector<Barrel*>::iterator it;
	for (it=m_barrells.begin(); it!=m_barrells.end(); ++it)
	{
		delete (*it);
	}
	m_barrells.clear();
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
	std::vector<BarrelInfo>::const_iterator cit;
	for (cit=info.barrells.begin(); cit!=info.barrells.end(); ++cit)
	{
		Barrel *barrel = new Barrel(this, (*cit).type, (*cit).direction, (*cit).effect_name);
		m_barrells.push_back(barrel);
	}

	return true;
}

void Weapon::configBarrells(void)
{
	std::vector<Barrel*>::iterator it;
	for (it=m_barrells.begin(); it!=m_barrells.end(); it++)
	{
		(*it)->updateProjectileStartPoint();
	}
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