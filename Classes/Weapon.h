#ifndef __HOUND_WEAPON_H__
#define __HOUND_WEAPON_H__

#include "Common.h"
#include "Barrel.h"

class Weapon :
	public cocos2d::Sprite
{
public:
	static Weapon* create(const WeaponInfo &info);

	void update(float dt);
	void configBarrells(void);

	void fire(void);

	cocos2d::Layer* getBattlefieldLayer(void)
	{
		return (cocos2d::Layer*)(getParent()->getParent());
	}

protected:
	Weapon(void);
	~Weapon(void);

	bool init(const WeaponInfo &info);

private:
	unsigned int	m_level;
	WEAPON_TYPE		m_type;

	float			m_firingInterval;
	float			m_firingCounter;

	float			m_bulletDamage;
	float			m_bulletSpeed;

	std::vector<Barrel*> m_barrells;
};

#endif //__HOUND_WEAPON_H__