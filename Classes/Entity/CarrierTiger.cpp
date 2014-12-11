#include "CarrierTiger.h"

USING_NS_CC;

CarrierTiger::CarrierTiger(void)
{
}


CarrierTiger::~CarrierTiger(void)
{
}

Enemy* CarrierTiger::create(const EnemyInfo &info)
{
	auto ret = new CarrierTiger();
	if ((ret!=nullptr)&&(ret->init(info)))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void CarrierTiger::update(float dt)
{
	Enemy::update(dt);
}
