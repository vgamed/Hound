#include "BulletExplosion.h"

USING_NS_CC;

BulletExplosion::BulletExplosion(void)
{
}


BulletExplosion::~BulletExplosion(void)
{
}

SFXExplosion* BulletExplosion::createWithSpriteFrames(const std::string &sf_name, int start_id, int number, 
											 float last, float scale)
{
	auto ret = new BulletExplosion();
	if ((ret!=nullptr)&&(ret->initWithSpriteFrames(sf_name, start_id, number, last, scale)))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

SFXExplosion* BulletExplosion::createWithAnimation(const std::string &anim_name, float last, float scale)
{
	auto ret = new BulletExplosion();
	if ((ret!=nullptr)&&(ret->initWithAnimation(anim_name, last, scale)))
	{
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

