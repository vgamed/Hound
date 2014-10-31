#include "Projectile.h"

USING_NS_CC;

Projectile::Projectile(void)
{
}


Projectile::~Projectile(void)
{
}

bool Projectile::init(const std::string &effect, const cocos2d::Vec2 &direction,
	float damage, float speed)
{
	SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(effect);
	if (!initWithSpriteFrame(frame))
	{
		return false;
	}

	m_direction = direction;
	m_speed = speed;
	m_damage = damage;

	m_direction.normalize();
	setLocalZOrder(-100);
	setScale(0.4f);

	Rect bb = getBoundingBox();
	Size s = getContentSize();

	scheduleUpdate();

	return true;
}
