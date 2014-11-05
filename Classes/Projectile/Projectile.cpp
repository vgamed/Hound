#include "Projectile.h"
#include "../Battlefield.h"
#include "../Enemy/Enemy.h"

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

	Size sz = getBoundingBox().size;
	m_boundingCircle.radius = MIN(sz.width, sz.height)/2;
	m_boundingCircle.center = getPosition();

	scheduleUpdate();

	return true;
}

void Projectile::update(float dt)
{
	// update bounding circle position
	m_boundingCircle.center = getPosition();

	// do collision detect
	auto bf = dynamic_cast<Battlefield*>(getParent());
	CC_ASSERT(bf != nullptr); // orphan projectile is absolutely something should never happen

	if (getBoundingBox().intersectsRect(bf->getBoundingBox()))
	{	// detect collision with objects in battlefield
		CollisionData data;
		for (Enemy *enemy : bf->getActiveEnemies())
		{
			//if (enemy->getBoundingCircle().intersectsRect(getBoundingBox()))
			if (enemy->getBoundingCircle().intersectsCircle(getBoundingCircle()))
			{
				data.whom.push_back(enemy);
			}
		}

		if (data.whom.size() > 0)
		{
			data.type = COLLISION_TYPE::PROJECTILE_TO_ENEMY;
			data.who = this;

			EventCustom event(EVENT_CUSTOM_COLLISION);
			event.setUserData(&data);

			_eventDispatcher->dispatchEvent(&event);
		}
	}
	else
	{	// detect collision with objects in battlefield
		CollisionData data;
		data.type = COLLISION_TYPE::PROJECTILE_TO_FIELD;
		data.who = this;
		data.whom.push_back(bf);

		EventCustom event(EVENT_CUSTOM_COLLISION);
		event.setUserData(&data);

		_eventDispatcher->dispatchEvent(&event);
	}
}