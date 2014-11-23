#ifndef __HOUND_H__
#define __HOUND_H__

#include "Common.h"
#include "Wingman.h"
#include "Weapon.h"

class Hound : public cocos2d::Sprite
{
public:
	static Hound* create(const HoundInfo &hdi);

	void update(float dt);

	//bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	//void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	
	void doDamage(float damage);

	const Circle& getBoundingCircle(void) 
	{
		m_boundingCircle.center = getPosition();
		return m_boundingCircle; 
	}
	
	float getBoundingCircleRadius(void) const
	{
		return m_boundingCircle.radius;
	}

	static const int TAG;
	
	CC_SYNTHESIZE(int, m_touchID, TouchID);

protected:
	Hound(void);
	~Hound(void);

	bool init(const HoundInfo &hdi);

private:
	BODY_TYPE			m_bodyType;
	int					m_bodyLevel;

	//Armor*			m_armor;
	//Engine*			m_engine;
	float	m_curLife;
	float	m_maxLife;
	float	m_armor;

	std::vector<Weapon*>	m_weapons;

	Wingman *m_wingmanLeft;
	Wingman *m_wingmanRight;

	cocos2d::Vec2 m_movingOffset; //for record touch position offset from the hound
	Circle	m_boundingCircle;
};

#endif

