#ifndef __HOUND_COMMON_H__
#define __HOUND_COMMON_H__

#include "cocos2d.h"

// common informations
struct ComponentInfo
{
	unsigned int type;
	unsigned int level;
	unsigned int dock_id;
};

struct HoundInfo
{
	unsigned int body_type;
	unsigned int body_level;
	unsigned int armor_type;
	unsigned int armor_level;
	unsigned int engine_type;
	unsigned int engine_level;
	std::vector<ComponentInfo> front_weapons;
	std::vector<ComponentInfo> side_weapons;
	std::vector<ComponentInfo> back_weapons;
};

struct PlayerInfo
{
	unsigned long id;
	std::string name;
	HoundInfo hound;
};

struct EnemyInfo
{
	unsigned int type;
	unsigned int level;
	unsigned int number;
	std::vector<cocos2d::Vec2> positions;
};

struct Wave
{
	std::vector<EnemyInfo> enemies;
};

struct LevelInfo
{
	unsigned long id;
	std::string sbg_layer1_name;
	std::string sbg_layer2_name;
	std::vector<Wave*> enemy_waves;
};

#endif //__HOUND_COMMON_H__