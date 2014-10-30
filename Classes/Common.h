#ifndef __HOUND_COMMON_H__
#define __HOUND_COMMON_H__

#include "cocos2d.h"

// type enumerations
enum BODY_TYPE
{
	BODY_NONE = 0,
	BODY_BASIC,
	BODY_MAX
};

enum ARMOR_TYPE
{
	ARMOR_NONE = 0,
	ARMOR_BASIC,
	ARMOR_MAX
};

enum ENGINE_TYPE
{
	ENGINE_NONE = 0,
	ENGINE_BASIC,
	ENGINE_MAX
};

enum WEAPON_TYPE
{
	WEAPON_NONE = 0,
	WEAPON_CANNON,
	WEAPON_MAX
};

enum WINGMAN_TYPE
{
	WINGMAN_NONE = 0,
	WINGMAN_BASIC,
	WINGMAN_MAX
};

enum ENEMY_TYPE
{
	ENEMY_NONE = 0,
	ENEMY_FIGHTER_BEE,
	ENEMY_FRIGATE_PUMA,
	ENEMY_CARRIER_TIGER,
	ENEMY_MAX
};

// player and hound data declarations
struct WeaponInfo
{
	WEAPON_TYPE		type;
	unsigned int	level;
	std::string		texture_name;
	cocos2d::Vec2	dock_position;
};

struct HoundInfo
{
	BODY_TYPE		body_type;
	unsigned int	body_level;
	std::string		body_texture_name;

	ARMOR_TYPE		armor_type;
	unsigned int	armor_level;
	std::string		armor_texture_name;

	ENGINE_TYPE		engine_type;
	unsigned int	engine_level;
	std::string		engine_texture_name;
	
	std::vector<WeaponInfo>	weapons;
};

struct PlayerInfo
{
	unsigned long	id;
	unsigned int	vip_level;
	std::string		name;
	HoundInfo		hound;
	//Inventory inventory;
};

// enemy fighter informations
struct EnemyInfo
{
	ENEMY_TYPE type;
	unsigned int level;
	std::vector<cocos2d::Vec2> positions;
};

struct WaveInfo
{
	float time_offset;
	std::vector<EnemyInfo> enemies;
};

struct LevelInfo
{
	unsigned long id;
	std::string sbg_layer1_name;
	std::string sbg_layer2_name;
	std::vector<WaveInfo> enemy_waves;
};

#endif //__HOUND_COMMON_H__