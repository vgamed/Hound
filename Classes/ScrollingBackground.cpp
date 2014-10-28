#include "ScrollingBackground.h"
#include "Hound.h"

USING_NS_CC;

ScrollingBackground::ScrollingBackground(void)
{
}


ScrollingBackground::~ScrollingBackground(void)
{
}

Scene* ScrollingBackground::createScene()
{
	auto scene = Scene::create();
	if (scene != nullptr)
	{
		SpriteChain chain;
		Sprite* sprite = nullptr;
		Layer* layer = nullptr;
		
		sprite = Sprite::create("img_bg_1.jpg");
		chain.push_back(sprite);
		layer = ScrollingBackground::create(	chain,
												Vec2(0.0f,1.0f),
												200.0f,
												100.0f);
		if (layer != nullptr)
		{
			scene->addChild(layer);
		}

		chain.clear();
		sprite = Sprite::create("clouds.png");
		chain.push_back(sprite);
		layer = ScrollingBackground::create(	chain,
												Vec2(0.0f,1.0f),
												100.0f);
		if (layer != nullptr)
		{
			scene->addChild(layer);
		}

		// test for Hound movement
		Hound *hound = Hound::create();
		if (hound != nullptr)
		{
			scene->addChild(hound);
		}
		auto size = Director::getInstance()->getVisibleSize();
		auto origin = Director::getInstance()->getVisibleOrigin();
		hound->setPosition(Vec2(origin.x+size.width/2, origin.y+hound->getBoundingBox().size.height));
	}
	
	return scene;
}

ScrollingBackground * ScrollingBackground::create(	SpriteChain &chain,
													Vec2 &scrollingDir,
													float scrollingSpeed,
													float startOffset,
													unsigned long repeatingNum,
													bool isScale)
{
	// there is no way to use scrolling background without any sprites
	CC_ASSERT(chain.size()>0);
	
	// only axis based directions are supported: X, -X, Y, -Y
	// No direction is also a bad parameter!
	CC_ASSERT(	((abs(scrollingDir.x)>FLT_EPSILON)&&(abs(scrollingDir.y)<=FLT_EPSILON))||
				((abs(scrollingDir.y)>FLT_EPSILON)&&(abs(scrollingDir.x)<=FLT_EPSILON)));

    ScrollingBackground *pRet = new ScrollingBackground();
	if (pRet && pRet->init(chain, scrollingDir, scrollingSpeed, startOffset, repeatingNum, isScale))
    {
        pRet->autorelease();
        return pRet;
    }
	else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool ScrollingBackground::init(	SpriteChain &chain,
								Vec2 &scrollingDir,
								float scrollingSpeed,
								float startOffset,
								unsigned long repeatingNum,
								bool isScale)
{
    if (!Layer::init())
    {
        return false;
    }
    
	// initialize member variables
	m_scrollingDirection = scrollingDir;
	m_startOffset = startOffset;
	m_scrollingSpeed = scrollingSpeed;
	m_repeatingNum = repeatingNum;
	m_spriteChain = chain;
	m_isScale = isScale;

	m_isPaused = false;
	m_repeatingCount = 0;
	m_indexOfNextFront = 0;
	m_swapIndicator.x = m_swapIndicator.y = FLT_MAX;

	// normalize the scrolling direction
	m_scrollingDirection.normalize();

	// clone the only sprite to make a swapp-able chain 
	// when there is only 1 sprite in the given chain
	if (m_spriteChain.size() == 1)
	{
		
		Sprite* clone = Sprite::createWithTexture(m_spriteChain[0]->getTexture());
		m_spriteChain.push_back(clone);
	}

	// add each sprite in the chain as a child of the layer,
	// then make them invisible.
	SpriteChain::iterator it;
	for (it=m_spriteChain.begin(); it!=m_spriteChain.end(); ++it)
	{
		(*it)->setAnchorPoint(Vec2(0.5f,0.5f));
		(*it)->setVisible(false);
		this->addChild(*it);
	}

	return true;
}

void ScrollingBackground::onEnterTransitionDidFinish(void)
{
	Size visibleSize = this->getParent()->getContentSize();
	Vec2 origin(0.0f, 0.0f);

	// scale all sprites in the chain to the size of parent
	SpriteChain::iterator it;
	for (it=m_spriteChain.begin(); it!=m_spriteChain.end(); ++it)
	{
		Size bb = (*it)->getBoundingBox().size;
		(*it)->setScaleX(visibleSize.width/bb.width);
		(*it)->setScaleY(visibleSize.height/bb.height);
	}

	// make the first sprite in the chain as the front sprite
	// also prepare the back sprite
	m_frontSprite = m_spriteChain[0];
	m_indexOfNextFront = 1;
	m_backSprite = m_spriteChain[m_indexOfNextFront];

	// set the starting position for front and back sprite
	auto bbFront = m_frontSprite->getBoundingBox();
	auto bbBack = m_backSprite->getBoundingBox();
	do {
		if (m_scrollingDirection.y > FLT_EPSILON) // +Y direction
		{
			m_frontSprite->setPosition(Vec2(origin.x+visibleSize.width/2, 
				origin.y+bbFront.size.height/2-m_startOffset));
			m_backSprite->setPosition(Vec2(origin.x+visibleSize.width/2, 
				origin.y+bbFront.size.height+bbBack.size.height/2-m_startOffset));
			m_swapIndicator = Vec2(origin.x+visibleSize.width/2, origin.y);
			break;
		}
		if (m_scrollingDirection.y < -FLT_EPSILON) // -Y direction
		{
			m_frontSprite->setPosition(Vec2(origin.x+visibleSize.width/2, 
				origin.y+visibleSize.height-bbFront.size.height/2+m_startOffset));
			m_backSprite->setPosition(Vec2(origin.x+visibleSize.width/2, 
				origin.y+visibleSize.height-bbFront.size.height-bbBack.size.height/2+m_startOffset));
			m_swapIndicator = Vec2(origin.x+visibleSize.width/2, origin.y+visibleSize.height);
			break;
		}
		if (m_scrollingDirection.x > FLT_EPSILON) // +X direction
		{
			m_frontSprite->setPosition(Vec2(origin.x+bbFront.size.width/2-m_startOffset, 
				origin.y+visibleSize.height/2));
			m_backSprite->setPosition(Vec2(origin.x+bbFront.size.width+bbBack.size.width/2-m_startOffset, 
				origin.y+visibleSize.height/2));
			m_swapIndicator = Vec2(origin.x, origin.y+visibleSize.height/2);
			break;
		}
		if (m_scrollingDirection.x < -FLT_EPSILON) // -X direction
		{
			m_frontSprite->setPosition(Vec2(origin.x+visibleSize.width-bbFront.size.width/2+m_startOffset, 
				origin.y+visibleSize.height/2));
			m_backSprite->setPosition(Vec2(origin.x+visibleSize.width-bbFront.size.width-bbBack.size.width/2+m_startOffset, 
				origin.y+visibleSize.height/2));
			m_swapIndicator = Vec2(origin.x+visibleSize.width, origin.y+visibleSize.height/2);
			break;
		}
	} while(0);

	m_frontSprite->setVisible(true);
	m_backSprite->setVisible(true);

	// start updating
	this->scheduleUpdate();
}

void ScrollingBackground::update(float dt)
{
	if (m_isPaused)
		return;

	Vec2 pos;
	Vec2 trans = m_scrollingDirection * m_scrollingSpeed * dt;

	// update the position of the back and front position
	pos = m_backSprite->getPosition() - trans;
	m_backSprite->setPosition(pos);

	pos = m_frontSprite->getPosition() - trans;
	m_frontSprite->setPosition(pos);

	// m_repeatingNum == ULONG_MAX is equivalent to infinite
	if (m_repeatingCount>m_repeatingNum)
		return;

	do {
		if (m_scrollingDirection.y > FLT_EPSILON) // +Y direction
		{
			pos.y += m_frontSprite->getBoundingBox().size.height / 2;
			if (pos.y <= m_swapIndicator.y)
			{
				swapSpriteChain();
				// associate the new back sprite to the new front sprite
				pos = m_frontSprite->getPosition();
				pos.y += (m_frontSprite->getBoundingBox().size.height+m_backSprite->getBoundingBox().size.height)/2;
				m_backSprite->setPosition(pos);

				m_repeatingCount++;
			}
			break;
		}
		if (m_scrollingDirection.y < -FLT_EPSILON) // -Y direction
		{
			pos.y -= m_frontSprite->getBoundingBox().size.height / 2;
			if (pos.y >= m_swapIndicator.y)
			{
				swapSpriteChain();
				// associate the new back sprite to the new front sprite
				pos = m_frontSprite->getPosition();
				pos.y -= (m_frontSprite->getBoundingBox().size.height+m_backSprite->getBoundingBox().size.height)/2;
				m_backSprite->setPosition(pos);

				m_repeatingCount++;
			}
			break;
		}
		if (m_scrollingDirection.x > FLT_EPSILON) // +X direction
		{
			pos.x += m_frontSprite->getBoundingBox().size.width / 2;
			if (pos.x <= m_swapIndicator.x)
			{
				swapSpriteChain();
				// associate the new back sprite to the new front sprite
				pos = m_frontSprite->getPosition();
				pos.x += (m_frontSprite->getBoundingBox().size.width+m_backSprite->getBoundingBox().size.width)/2;
				m_backSprite->setPosition(pos);

				m_repeatingCount++;
			}
			break;
		}
		if (m_scrollingDirection.x < -FLT_EPSILON) // -X direction
		{
			pos.x -= m_frontSprite->getBoundingBox().size.width / 2;
			if (pos.x >= m_swapIndicator.x)
			{
				swapSpriteChain();
				// associate the new back sprite to the new front sprite
				pos = m_frontSprite->getPosition();
				pos.x -= (m_frontSprite->getBoundingBox().size.width+m_backSprite->getBoundingBox().size.width)/2;
				m_backSprite->setPosition(pos);

				m_repeatingCount++;
			}
			break;
		}
	} while(0);
}

void ScrollingBackground::swapSpriteChain(void)
{
	m_frontSprite = m_backSprite;
	m_indexOfNextFront++;
	if (m_indexOfNextFront == m_spriteChain.size())
	{
		m_indexOfNextFront = 0;
	}
	m_backSprite =  m_spriteChain[m_indexOfNextFront];
}