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

bool FighterBee::init(const EnemyInfo &info)
{
	if (!Enemy::init(info))
		return false;
	
	return true;
}