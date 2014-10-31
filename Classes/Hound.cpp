#include "Hound.h"

USING_NS_CC;

const int Hound::TAG = 1001;

Hound::Hound(void)
{
}


Hound::~Hound(void)
{
}

Hound* Hound::create(const HoundInfo &hdi)
{
    Hound *hound = new (std::nothrow) Hound();
	if (hound && hound->init(hdi))
    {
        hound->autorelease();
        return hound;
    }
    CC_SAFE_DELETE(hound);
    return nullptr;
}

bool Hound::init(const HoundInfo &hdi)
{
	Texture2D *texture = 
		Director::getInstance()->getTextureCache()->getTextureForKey(hdi.body_texture_name);
	if (!initWithTexture(texture))
	{
		return false;
	}

	this->setTag(TAG);
	this->setName("Hound");
	this->setScale(0.7f);

	// initialize wingmen
	//...

	// initialize weapons
	std::vector<WeaponInfo>::const_iterator it;
	for (it=hdi.weapons.begin(); it!=hdi.weapons.end(); it++)
	{
		WeaponInfo const * info = (WeaponInfo const*)(&(*it));
		Weapon* weapon = nullptr;
		weapon = Weapon::create(*info);
		if (weapon != nullptr)
		{
			m_weapons.push_back(weapon);
			addChild(weapon);
			weapon->setPosition(info->dock_position);
		}
	}
	
	// register touch listeners
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Hound::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Hound::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void Hound::update(float dt)
{
	// update hound states
	//...

	// update weapons
	std::vector<Weapon*>::iterator it;
	for (it=m_weapons.begin(); it!=m_weapons.end(); ++it)
	{
		(*it)->update(dt);
	}
}

void Hound::configWeapons(void)
{
	std::vector<Weapon*>::iterator it;
	for (it=m_weapons.begin(); it!=m_weapons.end(); ++it)
	{
		(*it)->configBarrells();
	}
}

bool Hound::onTouchBegan(Touch *touch, Event *event)
{
	auto target = static_cast<Hound *>(event->getCurrentTarget());
	if (target == nullptr)
		return false;

	// remove the following part if you wanna control like À×öªÕ½»ú
	Rect bb = target->getBoundingBox();
	Size s = target->getContentSize();
	if (!bb.containsPoint(touch->getLocation()))
		return false;

	m_movingOffset = target->getPosition() - touch->getLocation();

	return true;
}

void Hound::onTouchMoved(Touch *touch, Event *event)
{
	auto target = static_cast<Hound *>(event->getCurrentTarget());
	if (target == nullptr)
		return;
	
	target->setPosition(touch->getLocation()+m_movingOffset);
	configWeapons();
}
