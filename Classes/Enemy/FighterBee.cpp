#include "FighterBee.h"

USING_NS_CC;

FighterBee::FighterBee(void)
{
}


FighterBee::~FighterBee(void)
{
}

Enemy* FighterBee::create(const EnemyInfo &info)
{
	auto ret = new FighterBee();
	if ((ret!=nullptr)&&(ret->init(info)))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void FighterBee::update(float dt)
{
	Enemy::update(dt);
}
