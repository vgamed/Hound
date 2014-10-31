#ifndef __HOUND_PROJECTILE_H__
#define __HOUND_PROJECTILE_H__

#include "cocos2d.h"

class Projectile :
	public cocos2d::Sprite
{
public:
	virtual void update(float dt) = 0;

protected:
	Projectile(void);
	virtual ~Projectile(void);

	virtual bool init(const std::string &effect, const cocos2d::Vec2 &direction,
		float damage, float speed);

protected:
	cocos2d::Vec2	m_direction;
	float			m_speed;
	float			m_damage;
};

#endif //__HOUND_PROJECTILE_H__
