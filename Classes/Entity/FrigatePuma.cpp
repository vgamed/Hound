#include "FrigatePuma.h"

USING_NS_CC;

FrigatePuma::FrigatePuma(void)
{
}


FrigatePuma::~FrigatePuma(void)
{
}

Enemy* FrigatePuma::create(const EnemyInfo &info)
{
	auto ret = new FrigatePuma();
	if ((ret!=nullptr)&&(ret->init(info)))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void FrigatePuma::update(float dt)
{
	Enemy::update(dt);
}
