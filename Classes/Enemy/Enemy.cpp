#include "Enemy.h"

USING_NS_CC;

Enemy::Enemy(void)
	: m_stateMachine(*this)
	, m_invincible(false)
{
}


Enemy::~Enemy(void)
{
}

bool Enemy::init(const EnemyInfo &info)
{
	Texture2D *texture = 
		Director::getInstance()->getTextureCache()->getTextureForKey(info.body_texture_name);
	if (!initWithTexture(texture))
	{
		return false;
	}

	m_type = info.type;
	m_curLife = m_maxLife = info.max_life;
	m_armor = info.armor;

	setScale(info.scale_xy);
	setLocalZOrder(ZORDER_ENEMY_FIGHTER);

	m_boundingCircle.radius = info.bounding_circle_radius;

	// init state transit for state machine
	//...

	// init weapons
	//...

	scheduleUpdate();

	return true;
}

void Enemy::update(float dt)
{
	m_stateMachine.update(dt);
}

void Enemy::doDamage(float damage)
{
	if (m_invincible)
		return;

	m_curLife -= damage*(1-m_armor/1000.0f);
}
