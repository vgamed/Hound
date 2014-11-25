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
		float			firing_counter;
		PROJECTILE_CREATOR projectile_creator;
	};

	static Weapon* create(const WeaponInfo &info, bool is_hound=true);

	void reset(void)
	{ m_timeCounter = 0; }

	void update(float dt);

	void fire(float dt);

	int getId(void) const
	{ return m_id; }

	WEAPON_TYPE getType(void) const
	{ return m_type; }

protected:
	Weapon(void);
	~Weapon(void);

	bool init(const WeaponInfo &info);

	void aim(void);

private:
	int				m_id;
	unsigned int	m_level;
	WEAPON_TYPE		m_type;
	bool			m_autoAim;

	float			m_timeOffsetFiringStart; //second
	float			m_timeOffsetFiringStop; //second
	float			m_timeCounter;

	float			m_damage;
	float			m_speed;

	std::vector<Barrel> m_barrells;

	bool			m_isHound;
};

#endif //__HOUND_WEAPON_H__