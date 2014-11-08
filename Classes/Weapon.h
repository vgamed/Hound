#ifndef __HOUND_WEAPON_H__
#define __HOUND_WEAPON_H__

#include "Common.h"

class Projectile;

class Weapon :
	public cocos2d::Sprite
{
public:
	typedef std::function<Projectile* ( const BarrelInfo&, 
										const cocos2d::Vec2&, 
										float, float, bool)>	PROJECTILE_CREATOR;

	struct Barrel
	{
		BarrelInfo		info;
		cocos2d::Vec2	direction;
		PROJECTILE_CREATOR projectile_creator;
	};

	static Weapon* create(const WeaponInfo &info, bool is_hound=true);

	void update(float dt);

	void fire(void);

protected:
	Weapon(void);
	~Weapon(void);

	bool init(const WeaponInfo &info);

private:
	unsigned int	m_level;
	WEAPON_TYPE		m_type;

	float			m_firingInterval;
	float			m_firingCounter;

	float			m_projectileDamage;
	float			m_projectileSpeed;

	std::vector<Barrel> m_barrells;

	bool			m_isHound;
};

#endif //__HOUND_WEAPON_H__