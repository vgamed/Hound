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

enum BARREL_TYPE
{
	BARREL_NONE = 0,
	BARREL_BULLET,
	BARREL_LASER,
	BARREL_MISSILE,
	BARREL_MAX
};

enum BULLET_TYPE
{
	BULLET_NONE = 0,
	BULLET_NORMAL,
	BULLET_MAX
};

enum LASER_TYPE
{
	LASER_NONE = 0,
	LASER_NORMAL,
	LASER_MAX
};

enum MISSILE_TYPE
{
	MISSILE_NONE = 0,
	MISSILE_NORMAL,
	MISSILE_MAX
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
struct BarrelInfo
{
	BARREL_TYPE		type;
	cocos2d::Vec2	direction;
	std::string		effect_name; // animation name or spriteframe name or texture name
};

struct WeaponInfo
{
	WEAPON_TYPE		type;
	unsigned int	level;
	cocos2d::Vec2	dock_position;
	std::vector<BarrelInfo> barrells;
	std::string		texture_name;
};

struct HoundInfo
{
	BODY_TYPE		body_type;
	unsigned int	body_level;

	ARMOR_TYPE		armor_type;
	unsigned int	armor_level;

	ENGINE_TYPE		engine_type;
	unsigned int	engine_level;
	
	std::vector<WeaponInfo>	weapons;

	std::string		body_texture_name;
	std::string		armor_texture_name;
	std::string		engine_texture_name;
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