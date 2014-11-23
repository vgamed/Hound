#include "Projectile.h"
#include "../Hound.h"
#include "../Battlefield.h"
#include "../Enemy/Enemy.h"

USING_NS_CC;

Projectile::Projectile(void)
{
}


Projectile::~Projectile(void)
{
}

bool Projectile::init(const BarrelInfo &info, const cocos2d::Vec2 &direction, 
		float damage, float speed, bool from_hound)
{
	SpriteFrame *frame = 
		SpriteFrameCache::getInstance()->getSpriteFrameByName(info.projectile_effect_name);
	if (!initWithSpriteFrame(frame))
	{
		return false;
	}

	m_projectileType = info.projectile_type;
	m_direction = direction;
	m_speed = speed;
	m_damage = damage;

	m_direction.normalize();
	m_isFromHound = from_hound;
	m_isFromHound ? setLocalZOrder(ZORDER_HOUND_PROJECTILE) :
					setLocalZOrder(ZORDER_ENEMY_PROJECTILE);
	setScale(info.projectile_scale_xy);

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
		data.who = this;
		if (m_isFromHound)
		{
			data.type = COLLISION_TYPE::PROJECTILE_TO_ENEMY;
			for (Enemy *enemy : bf->getActiveEnemies())
			{
				//if (enemy->getBoundingCircle().intersectsRect(getBoundingBox()))
				if (enemy->getBoundingCircle().intersectsCircle(getBoundingCircle()))
				{
					data.whom.push_back(enemy);
				}
			}
		}
		else
		{
			data.type = COLLISION_TYPE::PROJECTILE_TO_HOUND;
			if (bf->getHound()->getBoundingCircle().intersectsCircle(getBoundingCircle()))
			{
				data.whom.push_back(bf->getHound());
			}
		}

		if (data.whom.size() > 0)
		{

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