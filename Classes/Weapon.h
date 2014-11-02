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

	void updateProjectileStartPoints(void);
	void fire(void);

protected:
	typedef std::function<Projectile* ( const std::string&, 
										const cocos2d::Vec2&, 
										float, float)>	PROJECTILE_CREATOR;
	struct Barrel
	{
		BARREL_TYPE		type;
		cocos2d::Vec2	direction;
		std::string		effect_name;
		cocos2d::Vec2	projectile_startpoint;
		PROJECTILE_CREATOR projectile_creator;
	};

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

	std::vector<Barrel> m_barrells;
};

#endif //__HOUND_WEAPON_H__