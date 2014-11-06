#include "SFXExplosion.h"

USING_NS_CC;

SFXExplosion::SFXExplosion(void)
{
}


SFXExplosion::~SFXExplosion(void)
{
}

bool SFXExplosion::initWithSpriteFrames(const std::string &sf_name, int start_id, int number, 
									 float last, float scale)
{
	if (!init())
	{
		return false;
	}

	setScale(scale);

	auto anim = Animation::create();
	char name[50] = "\0";
	for (int i=0; i<number; ++i)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		sprintf_s(name, 50, "%s%03d.png", sf_name.c_str(), start_id+i);
#else
		sprintf(name, "%s%03d.png", sf_name.c_str(), start_id+i);
#endif
		anim->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
	}
	anim->setDelayPerUnit(last/number);
	anim->setRestoreOriginalFrame(true);
	
	auto callback_action = CallFuncN::create(CC_CALLBACK_1(SFXExplosion::onAnimEnd, this));
	auto anim_action = Animate::create(anim);
	runAction(Sequence::create(anim_action, callback_action, nullptr));

	return true;
}

bool SFXExplosion::initWithAnimation(const std::string &anim_name, float last, float scale)
{
	if (!init())
	{
		return false;
	}

	setScale(scale);

	auto anim = AnimationCache::getInstance()->getAnimation(anim_name);
	auto action = Animate::create(anim);
	runAction(action);

	return true;
}

void SFXExplosion::onAnimEnd(cocos2d::Node *sender)
{
	CC_ASSERT((sender==this) && (sender->getParent()!=nullptr));

	sender->getParent()->removeChild(sender);
}
