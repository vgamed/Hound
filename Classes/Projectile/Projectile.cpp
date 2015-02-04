#include "Projectile.h"
#include "Hound.h"
#include "Battlefield.h"
#include "Enemy.h"

USING_NS_CC;

Projectile::Projectile(void)
	: m_projectileType((int)PROJECTILE_TYPE::NONE)
	, m_damage(0.0f)
	, m_init_direction(Vec2::ZERO)
	, m_runtime_direction(Vec2::ZERO)
	, m_init_acceleration(0.0f)
	, m_runtime_acceleration(0.0f)
	, m_init_speed(0.0f)
	, m_runtime_speed(0.0f)
	, m_target(nullptr)

{
}


Projectile::~Projectile(void)
{
	if (m_target != nullptr)
	{
		m_target->release();
	}
}

bool Projectile::init(const BarrelInfo &info, 
					  const cocos2d::Vec2 &direction, 
					  Entity *target)
{
	do{
		SpriteFrame *frame = 
			SpriteFrameCache::getInstance()->getSpriteFrameByName(info.projectile_asset_name);
		if ((frame!=nullptr) && (initWithSpriteFrame(frame)))
		{
			break;
		}
		Texture2D *texture = 
			Director::getInstance()->getTextureCache()->getTextureForKey(info.projectile_asset_name);
		if ((texture==nullptr) || (!initWithTexture(texture)))
		{
			return false;
		}
	}while(0);

	m_projectileType = info.projectile_type;
	m_damage = info.projectile_final_damage;

	m_steering_speed = 0.0f;
	m_steering_acceleration = info.projectile_final_steering_accel;

	m_init_speed = info.projectile_final_speed;
	m_init_acceleration = info.projectile_final_acceleration;
	m_init_direction = direction;
	m_init_direction.normalize();

	m_runtime_speed = m_init_speed;
	m_runtime_acceleration = m_init_acceleration;
	m_runtime_direction = m_init_direction;

	if (target != nullptr)
	{
		m_target = target;
		target->retain(); //keep the target from autoRelease() when this projectile is active
		switch (m_target->getType())
		{
		case ENTITY_TYPE::ENEMY:
			setLocalZOrder(ZORDER_HOUND_PROJECTILE);
			break;
		case ENTITY_TYPE::HOUND:
			setLocalZOrder(ZORDER_ENEMY_PROJECTILE);
			break;
		default:
			break;
		}
	}
					
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

	if ((m_target != nullptr) && getBoundingBox().intersectsRect(bf->getBoundingBox()))
	{	// detect collision with objects in battlefield
		CollisionData data;
		data.who = this;
		if (m_target->getType() == ENTITY_TYPE::ENEMY)
		{
			data.type = COLLISION_TYPE::PROJECTILE_TO_ENEMY;
			for (auto enemy : bf->getActiveEnemies())
			{
				//if (enemy->getBoundingCircle().intersectsRect(getBoundingBox()))
				if ((enemy != nullptr) && !enemy->isDead() && 
					enemy->getBoundingCircle().intersectsCircle(getBoundingCircle()))
				{
					data.whom.push_back(enemy);
				}
			}
		}
		else
		{
			data.type = COLLISION_TYPE::PROJECTILE_TO_HOUND;
			if ((bf->getHound() != nullptr) && !bf->getHound()->isDead() &&
				bf->getHound()->getBoundingCircle().intersectsCircle(getBoundingCircle()))
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