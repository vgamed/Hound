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

	setTag(TAG);
	setName("Hound");
	setScale(hdi.scale_xy);
	setLocalZOrder(ZORDER_HOUND);

	m_boundingCircle.radius = hdi.bounding_circle_radius;

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
	//auto listener = EventListenerTouchOneByOne::create();
	//listener->setSwallowTouches(true);
	//listener->onTouchBegan = CC_CALLBACK_2(Hound::onTouchBegan, this);
	//listener->onTouchMoved = CC_CALLBACK_2(Hound::onTouchMoved, this);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

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

//bool Hound::onTouchBegan(Touch *touch, Event *event)
//{
//	m_movingOffset = getPosition() - touch->getLocation();
//	return true;
//}
//
//void Hound::onTouchMoved(Touch *touch, Event *event)
//{
//	setPosition(touch->getLocation()+m_movingOffset);
//}
