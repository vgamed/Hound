#include "Hound.h"

USING_NS_CC;

const int Hound::TAG = 1001;

Hound::Hound(void)
{
}


Hound::~Hound(void)
{
}

Hound* Hound::create(void)
{
    Hound *hound = new (std::nothrow) Hound();
    if (hound && hound->initWithFile("mplane.png"))
    {
        hound->autorelease();
        return hound;
    }
    CC_SAFE_DELETE(hound);
    return nullptr;
}

bool Hound::initWithTexture(Texture2D *texture, const Rect& rect, bool rotated)
{
	if (!Sprite::initWithTexture(texture, rect, rotated))
	{
		return false;
	}

	this->setTag(TAG);
	this->setName("Hound");
	this->setScale(0.7f);
	
	// initialize wingmen
	//...
	
	// register touch listeners
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Hound::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Hound::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

bool Hound::onTouchBegan(Touch *touch, Event *event)
{
	auto target = static_cast<Hound *>(event->getCurrentTarget());
	if (target == nullptr)
		return false;

	m_movingOffset = target->getPosition() - touch->getLocation();

	return true;
}

void Hound::onTouchMoved(Touch *touch, Event *event)
{
	auto target = static_cast<Hound *>(event->getCurrentTarget());
	target->setPosition(touch->getLocation()+m_movingOffset);
}
