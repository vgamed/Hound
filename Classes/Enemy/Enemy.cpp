#include "Enemy.h"

USING_NS_CC;

Enemy::Enemy(void)
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

	this->setScale(info.scale_xy);
	this->setLocalZOrder(ZORDER_ENEMY_FIGHTER);

	// init weapons
	// ...

	scheduleUpdate();

	return true;
}

void Enemy::update(float dt)
{
	static float timer = 0.0f;
	timer += dt;
	if (timer > 10.0f)
	{
		EventCustom event(EVENT_CUSTOM_DEBUG);
		DebugData data;
		data.command = DEBUG_COMMAND::ENEMY_SELF_DESTROY;
		data.target = this;
		event.setUserData(&data);
		_eventDispatcher->dispatchEvent(&event);
		timer = 0.0f;
	}
}