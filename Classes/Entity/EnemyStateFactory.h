#ifndef __HOUND_ENEMY_STATE_FACTORY_H__
#define __HOUND_ENEMY_STATE_FACTORY_H__

#include "Enemy.h"

class EnemyStateFactory
{
public:
	static EnemyState* create(const StateInfo &info);

private:
	EnemyStateFactory(void);
	~EnemyStateFactory(void);
};

#endif //__HOUND_ENEMY_STATE_FACTORY_H__