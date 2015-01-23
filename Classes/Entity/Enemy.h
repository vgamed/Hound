#ifndef __HOUND_ENEMY_FIGHTER_H__
#define __HOUND_ENEMY_FIGHTER_H__

#include "../Common.h"
#include "Entity.h"

class Enemy : public Entity
{
public:
	virtual void update(float dt);

	virtual int getEnemyType(void) 
	{ return m_typeEnemy; }

	void doDamage(float damage);

protected:
	Enemy(void);
	~Enemy(void);

	virtual bool init(const EnemyInfo &info);

	int m_typeEnemy;
};

#endif //__HOUND_ENEMY_FIGHTER_H__