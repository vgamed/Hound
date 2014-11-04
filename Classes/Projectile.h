#ifndef __HOUND_PROJECTILE_H__
#define __HOUND_PROJECTILE_H__

#include "Common.h"

class Projectile : public cocos2d::Sprite
{
public:
	virtual void update(float dt) = 0;

protected:
	Projectile(void);
	virtual ~Projectile(void);

	virtual bool init(const std::string effect, 
		const cocos2d::Vec2 &direction,
		float scale, float damage, 
		float speed, bool from_hound);

protected:
	cocos2d::Vec2	m_direction;
	float			m_speed;
	float			m_damage;

	bool			m_isFromHound;
};

#endif //__HOUND_PROJECTILE_H__
