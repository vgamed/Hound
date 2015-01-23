#ifndef __HOUND_ENTITY_STATE_FACTORY_H__
#define __HOUND_ENTITY_STATE_FACTORY_H__

#include "Entity.h"

class EntityStateFactory
{
public:
	static EntityState* create(const StateInfo &info);

private:
	EntityStateFactory(void);
	~EntityStateFactory(void);
};

#endif //__HOUND_ENTITY_STATE_FACTORY_H__