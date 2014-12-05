#ifndef __HOUND_PROJECTILE_H__
#define __HOUND_PROJECTILE_H__

#include "../Common.h"

class Projectile : public cocos2d::Sprite
{
public:
	virtual void update(float dt);

	virtual float getDamage(void) const { return m_damage; }
	virtual int getProjectileType(void) { return m_projectileType; }

	virtual const Circle& getBoundingCircle(void) const 
	{
		return m_boundingCircle; 
	}

protected:
	Projectile(void);
	virtual ~Projectile(void);

	virtual bool init(const BarrelInfo &info, const cocos2d::Vec2 &direction, 
		float damage, float speed, bool from_hound);

	int				m_projectileType;
	cocos2d::Vec2	m_direction;
	float			m_speed;
	float			m_damage;

	Circle			m_boundingCircle;

	bool			m_isFromHound;
};

#endif //__HOUND_PROJECTILE_H__
