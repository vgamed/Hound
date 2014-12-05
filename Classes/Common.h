#ifndef __HOUND_COMMON_H__
#define __HOUND_COMMON_H__

#include "cocos2d.h"
#include "Circle.h"

const int ZORDER_SFX_EXPLOSION		= 100;
const int ZORDER_HOUND_PROJECTILE	= 50;
const int ZORDER_ENEMY_PROJECTILE	= 50;
const int ZORDER_HOUND				= 10;
const int ZORDER_ENEMY_FIGHTER		= -80;
const int ZORDER_ENEMY_FRIGATE		= -90;
const int ZORDER_ENEMY_BOSS			= -100;

//////////////////////////////////////////////////////////////////////////////
// enumerations and structures for Movement info
//////////////////////////////////////////////////////////////////////////////

enum class MOVEMENT_TYPE
{
	NONE = 0,
	DISPLACEMENT,
	ROTATION,
	STAY,
	MAX_VALUE
};

struct Movement
{
	int				type;
	cocos2d::Vec2	target_position; // target for displacement
	bool			displmt_auto_facing; // for displacement
	float			stay_period; // second
	float			target_angle; //target angle for rotation and stay, in degree
	float			speed; //degree per second for rotation
	bool			jump; // if jump to the target position(for rotation), if jump to the target angle(stay)
};

typedef std::vector<Movement> MOVEMENTS;

//////////////////////////////////////////////////////////////////////////////
// enumerations and structures for AI state info
//////////////////////////////////////////////////////////////////////////////
enum class STATE_TYPE
{
	NONE = 0,
	MOVE,
	ENTRY,
	BATTLE_PHASE,
	TRANSFORM,
	LEAVE,
	BATTLE_END,
	DEAD,
	MAX_VALUE
};

enum class STATE_MACHINE_EVENT
{
	NONE = -1,
	START = 0,
	MOVE_FINISHED,
	ENTRY_FINISHED,
	BATTLE_PHASE_FINISHED,
	TRANSFORM_FINISHED,
	LEAVE_FINISHED,
	HOUND_DEAD,
	AI_DEAD,
	LEVEL_QUIT,
	VICTORY,
	MAX_VALUE
};

typedef std::vector<int> WEAPON_GROUP;

struct StateInfo
{
	int				id;
	int				type;
	float			life_threshold;
	bool			repeat_movements;

	MOVEMENTS		movements;
	WEAPON_GROUP	weapons;
};

typedef std::vector<StateInfo> STATE_INFOES;

struct StateMapInfo
{
	int event;
	int	from;
	int	to;
};

typedef std::vector<StateMapInfo> STATE_MAP_INFOES;

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// type enumerations for Hound and fighters
//////////////////////////////////////////////////////////////////////////////

enum class BODY_TYPE
{
	NONE = 0,
	BASIC,
	MAX_VALUE
};

enum class ARMOR_TYPE
{
	NONE = 0,
	ENEGY_SHIELD,
	REACTIVE_ARMOR,
	CERAMIC_ARMOR,
	MAX_VALUE
};

enum class ENGINE_TYPE
{
	NONE = 0,
	BASIC,
	MAX_VALUE
};

enum class WEAPON_TYPE
{
	NONE = 0,
	CANNON,
	MAX_VALUE
};

enum class BARREL_TYPE
{
	NONE = 0,
	BULLET,
	LASER,
	MISSILE,
	MAX_VALUE
};

enum class EFFECT_TYPE
{
	NONE = 0,
	TEXTURE,
	SPRITEFRAME,
	ANIMATION,
	MAX_VALUE
};

enum class PROJECTILE_TYPE
{
	NONE = 0,
	BULLET_NORMAL,
	LASER_NORMAL,
	MISSILE_NORMAL,
	MAX_VALUE
};

enum class WINGMAN_TYPE
{
	NONE = 0,
	BASIC,
	MAX_VALUE
};

enum class ENEMY_TYPE
{
	NONE = 0,
	FIGHTER_BEE,
	FRIGATE_PUMA,
	CARRIER_TIGER,
	MAX_VALUE
};

// player and hound data declarations
struct BarrelInfo
{
	int				type;
	int				projectile_type;
	float			projectile_scale_xy;

	// the angle from weapon direction, in degrees.
	// positive for rotating in clockwise, negtive for rotating in anti-clockwise
	float			rotate_angle;

	float			firing_interval; //second

	float			projectile_damage;
	float			projectile_speed; //per second
	// animation name or spriteframe name or texture name
	std::string		projectile_asset_name;
};

struct WeaponInfo
{
	int				id;
	int				type;
	unsigned int	level;

	float			time_offset_firing_start; //second
	float			time_offset_firing_stop; //second

	float			damage;
	float			speed;	//per second

	cocos2d::Vec2	dock_position;

	// the angle from Y+ direction, in degrees.
	// positive for rotating in clockwise, negtive for rotating in anti-clockwise
	float			rotate_angle;
	// if auto_aim is true, the rotate_angle doesn't matter
	bool			auto_aim;

	std::vector<BarrelInfo> barrells;
	std::string		texture_name;
};

struct HoundInfo
{
	int				body_type;
	unsigned int	body_level;

	int				armor_type;
	unsigned int	armor_level;

	int				engine_type;
	unsigned int	engine_level;
	
	std::vector<WeaponInfo>	weapons;

	float			scale_xy;
	float			bounding_circle_radius;

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
	int				id;
	int				type;
	unsigned int	level;

	float			scale_xy;
	float			rotate_angle;

	cocos2d::Vec2	entry_from;
	cocos2d::Vec2	entry_to;
	float			entry_speed;
	bool			entry_auto_facing;

	float			leave_speed;
	bool			leave_auto_facing;

	float			max_life;
	float			armor;

	float			bounding_circle_radius;
	std::string		body_texture_name;

	std::vector<WeaponInfo>	weapons;

	STATE_INFOES state_infoes;
	STATE_MAP_INFOES state_map_infoes;
};

struct WaveInfo
{
	float time_offset;
	std::vector<EnemyInfo> enemies;
};

struct LevelInfo
{
	int							id;

	cocos2d::Vec2				hound_entry_from;
	cocos2d::Vec2				hound_entry_to;
	float						hound_entry_speed;
	bool						hound_entry_auto_facing;
	float						hound_leave_speed;
	bool						hound_leave_auto_facing;

	std::vector<std::string>	sbg_layer_texture_names;

	std::vector<WaveInfo>		enemy_waves;
};
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// custom cocos2d events and related enumerations and structures
//////////////////////////////////////////////////////////////////////////////

extern const char * EVENT_CUSTOM_COLLISION;
extern const char * EVENT_CUSTOM_DEBUG;

enum class COLLISION_TYPE
{
	NONE = 0,
	HOUND_TO_ENEMY,
	HOUND_TO_FIELD,
	PROJECTILE_TO_HOUND,
	PROJECTILE_TO_ENEMY,
	PROJECTILE_TO_FIELD,
	MAX_VALUE
};

struct CollisionData
{
	COLLISION_TYPE type;
	cocos2d::Node *who;
	std::vector<cocos2d::Node*> whom;
};

enum class DEBUG_COMMAND
{
	NONE = 0,
	ENEMY_SELF_DESTROY,
	MAX_VALUE
};
 
struct DebugData
{
	 DEBUG_COMMAND command;
	 cocos2d::Node *target;
};
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// data structures for special effect
//////////////////////////////////////////////////////////////////////////////

//struct SFXInfo
//{
//	std::string name_prefix;
//	int			start_id;
//	int			frame_number;
//
//	float		scale;
//
//	float		last;
//	int			repeat;
//};

#endif //__HOUND_COMMON_H__