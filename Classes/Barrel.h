#ifndef __HOUND_BARREL_H__
#define __HOUND_BARREL_H__

#include "Common.h"

class Weapon;
class Projectile;

class Barrel
{
public:
	Barrel(Weapon* weapon, BARREL_TYPE type, 
		const cocos2d::Vec2 &direction,
		const std::string &effect_name);
	~Barrel(void);

	void updateProjectileStartPoint();

	void fire(float damage, float speed);
	void removeProjectile(const Projectile *proj);

private:
	BARREL_TYPE		m_type;
	cocos2d::Vec2	m_direction;
	Weapon *		m_weaponAssociated;
	std::string		m_effectName;

	cocos2d::Vec2	m_projectileStartPoint;
	
	typedef std::function<Projectile* ( const std::string&, 
										const cocos2d::Vec2&, 
										float, float)>	PROJECTILE_CREATOR;

	PROJECTILE_CREATOR m_projectileCreator;
	std::vector<Projectile*> m_activeProjectiles;
};

#endif //__HOUND_BARREL_H__