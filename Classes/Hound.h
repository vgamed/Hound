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
	void configWeapons(void);

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);

	static const int TAG;

protected:
	Hound(void);
	~Hound(void);

	bool init(const HoundInfo &hdi);

private:
	BODY_TYPE			m_bodyType;
	int					m_bodyLevel;

	//Armor*			m_armor;
	//Engine*			m_engine;

	std::vector<Weapon*>	m_weapons;

	Wingman *m_wingmanLeft;
	Wingman *m_wingmanRight;

	cocos2d::Vec2 m_movingOffset;
};

#endif

