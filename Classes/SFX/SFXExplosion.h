#ifndef __HOUND_SFX_EXPLOSION_H__
#define __HOUND_SFX_EXPLOSION_H__

#include "../Common.h"

class SFXExplosion : public cocos2d::Sprite
{
public:
	virtual void onAnimEnd(cocos2d::Node *sender);

protected:
	SFXExplosion(void);
	virtual ~SFXExplosion(void);

	bool initWithSpriteFrames(const std::string &sf_name, int start_id, int number,
		float last, float scale);
	bool initWithAnimation(const std::string &anim_name, float last, float scale);
};

#endif //__HOUND_SFX_EXPLOSION_H__
