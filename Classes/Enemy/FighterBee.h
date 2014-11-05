#ifndef __HOUND_FIGHTER_BEE_H__
#define __HOUND_FIGHTER_BEE_H__

#include "Enemy.h"

class FighterBee :
	public Enemy
{
public:
	static Enemy* create(const EnemyInfo &info);
	void update(float dt);

protected:
	FighterBee(void);
	~FighterBee(void);
};

#endif //__HOUND_FIGHTER_BEE_H__