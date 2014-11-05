#include "Projectile.h"

USING_NS_CC;

Projectile::Projectile(void)
{
}


Projectile::~Projectile(void)
{
}

bool Projectile::init(const std::string effect, 
		const cocos2d::Vec2 &direction,
		float scale, float damage, 
		float speed, bool from_hound)
{
	SpriteFrame *frame = 
		SpriteFrameCache::getInstance()->getSpriteFrameByName(effect);
	if (!initWithSpriteFrame(frame))
	{
		return false;
	}

	m_direction = direction;
	m_speed = speed;
	m_damage = damage;

	m_direction.normalize();
	m_isFromHound = from_hound;
	m_isFromHound ? setLocalZOrder(ZORDER_HOUND_PROJECTILE) :
					setLocalZOrder(ZORDER_ENEMY_PROJECTILE);
	setScale(scale);

	scheduleUpdate();

	return true;
}
