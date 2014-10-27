#ifndef __SCROLLING_BACKGROUND_H__
#define __SCROLLING_BACKGROUND_H__

#include "cocos2d.h"

class ScrollingBackground :
	public cocos2d::Layer
{
public:
	typedef std::vector<cocos2d::Sprite *> SpriteChain;

    static cocos2d::Scene* createScene(void); //for debugging purpose, will be removed when publishing

	void update(float dt);

	static ScrollingBackground* create(	SpriteChain &chain,
										cocos2d::Vec2 &scrollingDir = cocos2d::Vec2(0.0f,1.0f),
										float scrollingSpeed = 200.0f,
										float startOffset = 0.0f,
										unsigned long repeatingNum = ULONG_MAX,
										bool isScale = true);
	void pauseScrolling()
	{
		m_isPaused = true;
	}
	
	void resumeScrolling()
	{
		m_isPaused = false;
	}

	void resetScrolling()
	{
		m_isPaused = false;
		m_repeatingCount = 0;
	}

	void onEnterTransitionDidFinish(void);

	CC_SYNTHESIZE(float, m_scrollingSpeed, ScrollingSpeed);
	CC_SYNTHESIZE(cocos2d::Vec2, m_scrollingDirection, ScrollingDirection);

private:
	ScrollingBackground(void);
	~ScrollingBackground(void);
	bool init(	SpriteChain &chain,
				cocos2d::Vec2 &scrollingDir,
				float scrollingSpeed,
				float startOffset,
				unsigned long repeatingNum,
				bool isScale);

	void swapSpriteChain(void);

	float			m_startOffset;
	unsigned long	m_repeatingNum;
	unsigned long	m_repeatingCount;
	bool			m_isScale;
	bool			m_isPaused;

	cocos2d::Sprite *m_frontSprite;
	cocos2d::Sprite *m_backSprite;
	SpriteChain m_spriteChain;
	SpriteChain::size_type m_indexOfNextFront;

	cocos2d::Vec2 m_swapIndicator;
};

#endif //__SCROLLING_BACKGROUND_H__