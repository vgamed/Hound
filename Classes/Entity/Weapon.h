#ifndef __HOUND_WEAPON_H__
#define __HOUND_WEAPON_H__

#include "Common.h"

class Entity;
class Projectile;

class Weapon :
	public cocos2d::Sprite
{
public:
	typedef std::function<Projectile* ( const BarrelInfo&, 
										const cocos2d::Vec2&, 
										Entity*)>	PROJECTILE_CREATOR;

	struct Barrel
	{
		BarrelInfo		info;
		cocos2d::Vec2	direction;
		float			firing_counter;
		PROJECTILE_CREATOR projectile_creator;
	};

	static Weapon* create(const WeaponInfo &info);

	void reset(void)
	{ m_timeCounter = 0; }

	void update(float dt);

	void onEnter(void);

	void fire(float dt);

	int getId(void) const
	{ return m_id; }

	int getType(void) const
	{ return m_type; }

protected:
	Weapon(void);
	~Weapon(void);

	bool init(const WeaponInfo &info);

	void autoAimTarget(void);

private:
	int				m_id;
	unsigned int	m_level;
	int				m_type;
	bool			m_autoAim;

	float			m_timeOffsetFiringStart; //second
	float			m_timeOffsetFiringStop; //second
	float			m_timeCounter;

	float			m_damage;
	float			m_speed;

	std::vector<Barrel> m_barrells;

	Entity*			m_entityParent;
};

#endif //__HOUND_WEAPON_H__