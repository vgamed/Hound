#ifndef __HOUND_ENEMY_FACTORY_H__
#define __HOUND_ENEMY_FACTORY_H__

#include "Enemy.h"

class EnemyFactory
{
public:
	static Enemy* create(const EnemyInfo &info);

private:
	EnemyFactory(void);
	~EnemyFactory(void);
};

#endif //__HOUND_ENEMY_FACTORY_H__