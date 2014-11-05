#ifndef __HOUND_ENEMY_FIGHTER_H__
#define __HOUND_ENEMY_FIGHTER_H__

#include "../Common.h"

class Enemy :
	public cocos2d::Sprite
{
public:
	virtual void update(float dt);

protected:
	Enemy(void);
	~Enemy(void);

	virtual bool init(const EnemyInfo &info);
};

#endif //__HOUND_ENEMY_FIGHTER_H__