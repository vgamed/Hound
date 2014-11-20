#ifndef __HOUND_COMMON_H__
#define __HOUND_COMMON_H__

#include "cocos2d.h"
#include "Circle.h"

const int ZORDER_HOUND				= 100;
const int ZORDER_HOUND_PROJECTILE	= 10;
const int ZORDER_ENEMY_PROJECTILE	= -70;
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
	MAX
};

struct MoveParamDisplacement
{
	float speed;
	bool facing_dir;
};

struct MoveParamRotation
{
	float angle; //degree
	float speed; //degree per second
};

struct MoveParamStay
{
	float angle; //degree
	float period; // second
};

union MoveParam
{
	MoveParamDisplacement displmt;
	MoveParamRotation rotation;
	MoveParamStay stay;
};

struct Movement
{
	MOVEMENT_TYPE	type;
	cocos2d::Vec2	target_position;
	MoveParam		move_param;
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
	MAX
};

enum class STATE_MACHINE_EVENT
{
	START = 0,
	MOVE_FINISHED,
	ENTRY_FINISHED,
	BATTLE_PHASE_FINISHED,
	TRANSFORM_FINISHED,
	LEAVE_FINISHED,
	BATTLE_END,
	DEAD,
	MAX
};

typedef std::vector<int> WEAPON_GROUP;

struct StateInfo
{
	int id;
	STATE_TYPE type;
	MOVEMENTS movements;
	WEAPON_GROUP weapons;
};

typedef std::vector<StateInfo> STATE_INFOES;

struct StateMap
{
	int from;
	STATE_MACHINE_EVENT event;
	int to;
};

typedef std::vector<StateMap> STATE_MAPS;

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
	BASIC,
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
	BARREL_TYPE		type;
	PROJECTILE_TYPE	projectile_type;
	float			projectile_scale_xy;
	// the angle from positive Y direction, in degrees.
	// positive for rotating in clockwise, negtive for rotating in anti-clockwise
	float			rotate_angle;

	float			firing_interval; //second

	float			projectile_damage;
	float			projectile_speed; //per second
	// animation name or spriteframe name or texture name
	EFFECT_TYPE		projectile_effect_type;
	std::string		projectile_effect_name;
};

struct WeaponInfo
{
	WEAPON_TYPE		type;
	unsigned int	level;

	float			time_offset_firing_start; //second
	float			time_offset_firing_stop; //second

	float			damage;
	float			speed;	//per second

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
	ENEMY_TYPE type;
	unsigned int level;

	float			max_life;
	float			armor;

	float			scale_xy;
	float			bounding_circle_radius;
	std::string		body_texture_name;
	cocos2d::Vec2	start_position;

	std::vector<WeaponInfo>	weapons;

	std::vector<StateInfo> states;
};

struct WaveInfo
{
	float time_offset;
	std::vector<EnemyInfo> enemies;
};

struct LevelInfo
{
	unsigned long				id;
	std::vector<std::string>	sbg_layer_texture_names;
	cocos2d::Vec2				hound_start_offset;
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