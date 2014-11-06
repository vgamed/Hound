#ifndef __HOUND_ENEMY_FIGHTER_H__
#define __HOUND_ENEMY_FIGHTER_H__

#include "../Common.h"

class Enemy :
	public cocos2d::Sprite
{
public:
	virtual void update(float dt);

	virtual ENEMY_TYPE getType(void) { return m_type; }

	virtual const Circle& getBoundingCircle(void) const 
	{
		return m_boundingCircle; 
	}

protected:
	Enemy(void);
	~Enemy(void);

	virtual bool init(const EnemyInfo &info);

	ENEMY_TYPE m_type;
	float	m_timerLeave;
	Circle	m_boundingCircle;
};

#endif //__HOUND_ENEMY_FIGHTER_H__