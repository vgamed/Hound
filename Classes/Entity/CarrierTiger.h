#ifndef __HOUND_CARRIER_TIGER_H__
#define __HOUND_CARRIER_TIGER_H__

#include "Enemy.h"

class CarrierTiger :
	public Enemy
{
public:
	static Enemy* create(const EnemyInfo &info);
	void update(float dt);

protected:
	CarrierTiger(void);
	~CarrierTiger(void);
};

#endif //__HOUND_CARRIER_TIGER_H__