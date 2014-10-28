#ifndef __HOUND_H__
#define __HOUND_H__

#include "cocos2d.h"
#include "Wingman.h"

class Hound :
	public cocos2d::Sprite
{
public:
	static Hound* create(void);

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);

	static const int TAG;

protected:
	Hound(void);
	~Hound(void);

    bool initWithTexture(cocos2d::Texture2D *texture, const cocos2d::Rect& rect, bool rotated) override;

private:
	Wingman *m_wingmanLeft;
	Wingman *m_wingmanRight;

	cocos2d::Vec2 m_movingOffset;
};

#endif

