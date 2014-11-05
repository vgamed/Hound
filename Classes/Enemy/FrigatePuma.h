#ifndef __HOUND_FRIGATE_PUMA_H__
#define __HOUND_FRIGATE_PUMA_H__

#include "Enemy.h"

class FrigatePuma :
	public Enemy
{
public:
	static Enemy* create(const EnemyInfo &info);
	void update(float dt);

protected:
	FrigatePuma(void);
	~FrigatePuma(void);
};

#endif //__HOUND_FRIGATE_PUMA_H__