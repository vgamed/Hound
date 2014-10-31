#ifndef __HOUND_BULLET_H__
#define __HOUND_BULLET_H__

#include "Common.h"
#include "Projectile.h"

class Bullet : public Projectile
{
public:
	static Projectile* create(const std::string effect, 
		const cocos2d::Vec2 &direction,
		float damage, float speed);

	void update(float dt) override;

protected:
	Bullet(void);
	virtual ~Bullet(void);
};

#endif //__HOUND_BULLET_H__
