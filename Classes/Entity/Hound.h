#ifndef __HOUND_H__
#define __HOUND_H__

#include "Common.h"
#include "Entity.h"
#include "Wingman.h"

class Hound : public Entity
{
public:
	static Hound* create(const HoundInfo &hdi, const LevelInfo &lli);

	void update(float dt);

	//bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	//void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	
	void doDamage(float damage);

	CC_SYNTHESIZE(int, m_touchID, TouchID);

protected:
	Hound(void);
	~Hound(void);

	bool init(const HoundInfo &hdi, const LevelInfo &lli);

private:
	int				m_bodyType;
	int				m_bodyLevel;

	//Armor*			m_armor;
	//Engine*			m_engine;

	Wingman *m_wingmanLeft;
	Wingman *m_wingmanRight;
};

#endif

