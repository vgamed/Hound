#include "AppDelegate.h"
#include "cocos-ext.h"
#include "tinyxml2\tinyxml2.h"
#include "GameScene.h"

USING_NS_CC;
using namespace tinyxml2;

static const Size resourceResolution(720.0f, 1280.0f);
static const Size designResolution(640.0f, 960.0f);

AppDelegate::AppDelegate() 
{
}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
		glview = GLView::create("Hound");
		//glview = GLView::createWithRect("Hound", Rect(0, 0, 1280, 960));
        director->setOpenGLView(glview);
		glview->setDesignResolutionSize(designResolution.width, designResolution.height, ResolutionPolicy::SHOW_ALL);
		director->setContentScaleFactor(resourceResolution.width/designResolution.width);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

	// load all game resources and information into caches
	if (!loadGameResources())
	{
		return false;
	}

	if (!loadPlayerInfo())
	{
		return false;
	}

	if (!loadLevelInfo())
	{
		return false;
	}

    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();
	auto scene = GameScene::create(m_playerInfo, m_levelInfo);
	if (scene == nullptr)
	{
		return false;
	}

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AppDelegate::scaleByDesign(Vec2 &vec2)
{
	static float scale_x = Director::getInstance()->getVisibleSize().width / designResolution.width;
	static float scale_y = Director::getInstance()->getVisibleSize().height / designResolution.height;

	CCLOG("BEFORE SCALE: x=%f, y=%f", vec2.x, vec2.y);

	vec2.x *= scale_x;
	vec2.y *= scale_y;

	CCLOG("AFTER SCALE: x=%f, y=%f", vec2.x, vec2.y);
}

void AppDelegate::scaleByDesign(float &flt)
{
	static float scale = MIN(Director::getInstance()->getVisibleSize().width / designResolution.width,
							Director::getInstance()->getVisibleSize().height / designResolution.height);

	CCLOG("BEFORE SCALE: floatValue = %f", flt);

	flt *= scale;

	CCLOG("AFTER SCALE: floatValue = %f", flt);
}

bool AppDelegate::loadGameResources(void)
{
	FileUtils::getInstance()->addSearchPath("bullet");
	FileUtils::getInstance()->addSearchPath("fonts");
	
	TextureCache *tcache = Director::getInstance()->getTextureCache();
	tcache->addImage("img_bg_1.jpg");
	tcache->addImage("img_bg_2.jpg");
	tcache->addImage("img_bg_3.jpg");
	tcache->addImage("img_bg_4.jpg");
	tcache->addImage("img_bg_5.jpg");

	tcache->addImage("clouds.png");

	tcache->addImage("mplane.png");
	tcache->addImage("frontgun.png");
	tcache->addImage("leftgun.png");
	tcache->addImage("rightgun.png");

	tcache->addImage("n_boss.png");
	tcache->addImage("n1.png");
	tcache->addImage("n2.png");
	tcache->addImage("n3.png");
	tcache->addImage("bullet0.png");
	tcache->addImage("bullet1.png");
	tcache->addImage("bullet2.png");
	tcache->addImage("bullet3.png");
	tcache->addImage("bullet4.png");
	tcache->addImage("bullet5.png");
	tcache->addImage("bullet6.png");
	tcache->addImage("bullet7.png");
	tcache->addImage("bullet8.png");
	tcache->addImage("bullet9.png");
	tcache->addImage("bullet10.png");
	tcache->addImage("bullet11.png");
	tcache->addImage("bullet12.png");
	tcache->addImage("bullet13.png");
	tcache->addImage("bullet14.png");
	tcache->addImage("bullet15.png");
	tcache->addImage("bullet16.png");
	tcache->addImage("bullet17.png");
	tcache->addImage("bullet18.png");

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("bullet.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("wsparticle_p01.plist");

	AnimationCache::getInstance()->addAnimationsWithFile("pig.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("nplane.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("bullet.plist");

	return true;
}

bool AppDelegate::loadPlayerInfo(void)
{
	m_playerInfo.id = 0;
	m_playerInfo.name = "player";
	m_playerInfo.vip_level = 0;
	m_playerInfo.hound.body_level = 1;
	m_playerInfo.hound.body_type = BODY_TYPE::BASIC;
	m_playerInfo.hound.body_texture_name = "mplane.png";
	m_playerInfo.hound.armor_level = 1;
	m_playerInfo.hound.armor_type = ARMOR_TYPE::ENEGY_SHIELD;
	m_playerInfo.hound.armor_texture_name = "";
	m_playerInfo.hound.engine_level = 1;
	m_playerInfo.hound.engine_type = ENGINE_TYPE::BASIC;
	m_playerInfo.hound.engine_texture_name = "";

	m_playerInfo.hound.start_position = Vec2(320.0f, 150.0f);
	scaleByDesign(m_playerInfo.hound.start_position);

	m_playerInfo.hound.entry_speed = 300.0f;
	m_playerInfo.hound.leave_speed = 300.0f;

	m_playerInfo.hound.scale_xy = 1.0f;
	scaleByDesign(m_playerInfo.hound.scale_xy);

	m_playerInfo.hound.bounding_circle_radius = 10.0f;

	m_playerInfo.hound.weapons.clear();

	BarrelInfo barrel;
	barrel.type = BARREL_TYPE::BULLET;
	barrel.projectile_type = PROJECTILE_TYPE::BULLET_NORMAL;
	
	barrel.projectile_scale_xy = 0.6f;
	scaleByDesign(barrel.projectile_scale_xy);

	//barrel.projectile_effect_name = "bullet_1.png";
	barrel.projectile_effect_name = "bullet18.png";
	barrel.projectile_damage = 2.0f;
	barrel.projectile_speed = 200.0f;
	barrel.firing_interval = 0.1f; //second

	WeaponInfo weapon;
	weapon.level  = 1;
	weapon.type = WEAPON_TYPE::CANNON;
	weapon.auto_aim = false;
	weapon.time_offset_firing_start = 2.0f; //second
	weapon.time_offset_firing_stop = FLT_MAX; //second
	weapon.speed = 1000.0f; //per second
	weapon.damage = 8.0f;

	weapon.id = 1;
	weapon.texture_name = "frontgun.png";
	weapon.dock_position = Vec2(58.0f, 81.0f);
	weapon.rotate_angle = 0.0f;
	weapon.barrells.clear();
	barrel.rotate_angle = 0.0f;
	weapon.barrells.push_back(barrel);
	m_playerInfo.hound.weapons.push_back(weapon);

	weapon.id = 2;
	weapon.texture_name = "leftgun.png";
	weapon.dock_position = Vec2(29.0f, 65.0f);
	weapon.rotate_angle = -30.0f;
	weapon.barrells.clear();
	barrel.rotate_angle = 0.0f;
	weapon.barrells.push_back(barrel);
	barrel.rotate_angle = -30.0f;
	weapon.barrells.push_back(barrel);
	barrel.rotate_angle = 30.0f;
	weapon.barrells.push_back(barrel);
	m_playerInfo.hound.weapons.push_back(weapon);

	weapon.id = 3;
	weapon.texture_name = "rightgun.png";
	weapon.dock_position = Vec2(87.0f, 65.0f);
	weapon.rotate_angle = 30.0f;
	weapon.barrells.clear();
	barrel.rotate_angle = 0.0f;
	weapon.barrells.push_back(barrel);
	barrel.rotate_angle = -30.0f;
	weapon.barrells.push_back(barrel);
	barrel.rotate_angle = 30.0f;
	weapon.barrells.push_back(barrel);
	m_playerInfo.hound.weapons.push_back(weapon);

	return true;
}

bool AppDelegate::loadLevelInfo(void)
{
	m_levelInfo.id = 0;

	m_levelInfo.hound_start_offset = Vec2(0.0f, 150.0f);
	scaleByDesign(m_levelInfo.hound_start_offset);

	m_levelInfo.sbg_layer_texture_names.push_back("img_bg_1.jpg");
	m_levelInfo.sbg_layer_texture_names.push_back("clouds.png");

	WaveInfo w_info;
	EnemyInfo ef_info;
	StateInfo state;
	Movement movement;

	// 1st wave
	w_info.time_offset = 2.0f;
	ef_info.type = ENEMY_TYPE::FIGHTER_BEE;
	ef_info.level = 1;
	ef_info.max_life = 1200.0f;
	ef_info.armor = 100.0f;
	
	ef_info.scale_xy = 1.0f;
	scaleByDesign(ef_info.scale_xy);

	ef_info.bounding_circle_radius = 50.0f;
	ef_info.body_texture_name = "n1.png";
	ef_info.facing_dir = -Vec2::UNIT_Y;

	// weapons
	BarrelInfo barrel;
	barrel.type = BARREL_TYPE::BULLET;
	barrel.projectile_type = PROJECTILE_TYPE::BULLET_NORMAL;
	
	barrel.projectile_scale_xy = 0.6f;
	scaleByDesign(barrel.projectile_scale_xy);

	barrel.projectile_effect_name = "bullet0.png";
	barrel.rotate_angle = 0.0f;
	barrel.projectile_damage = 200.0f;
	barrel.projectile_speed = 200.0f;
	barrel.firing_interval = 0.2f; //second

	WeaponInfo weapon;
	weapon.level  = 1;
	weapon.type = WEAPON_TYPE::CANNON;
	weapon.speed = 0.0f; //per second
	weapon.damage = 0.0f;
	weapon.texture_name = "";
	weapon.barrells.push_back(barrel); // 1 barrel for this weapon

	//1st weapon
	weapon.time_offset_firing_start = 0.5f; //second
	weapon.time_offset_firing_stop = 1.5f; //second
	weapon.auto_aim = true;
	weapon.id = 1;
	weapon.dock_position = Vec2(60.0f, 100.0f);
	weapon.rotate_angle = -20.0f;
	ef_info.weapons.push_back(weapon);

	//2nd weapon
	weapon.id = 2;
	weapon.dock_position = Vec2(98.0f, 100.0f);
	weapon.rotate_angle = 20.0f;
	ef_info.weapons.push_back(weapon);

	//3rd weapon
	weapon.time_offset_firing_start = 0.1f; //second
	weapon.time_offset_firing_stop = 7.5f; //second
	weapon.auto_aim = false;
	weapon.id = 3;
	weapon.dock_position = Vec2(60.0f, 50.0f);
	weapon.rotate_angle = -90.0f;
	ef_info.weapons.push_back(weapon);

	//4th weapon
	weapon.id = 4;
	weapon.dock_position = Vec2(98.0f, 50.0f);
	weapon.rotate_angle = 90.0f;
	ef_info.weapons.push_back(weapon);
	//

	// enemy 1
	ef_info.start_position = Vec2(200.0f, 960.0f);
	scaleByDesign(ef_info.start_position);

		// 1st state - Entry State
		state.id = 1;
		state.type = STATE_TYPE::ENTRY;
	
			// 1st movement
			movement.type = MOVEMENT_TYPE::DISPLACEMENT;
			movement.target_position = Vec2(200.0f, 560.0f);
			scaleByDesign(movement.target_position);
			movement.move_param.displmt.facing_dir = true;
			movement.move_param.displmt.speed = 100.0f;
			state.movements.push_back(movement);

		ef_info.state_infoes.push_back(state);
		//

		// 2nd state - Battle Phase State
		state.id = 2;
		state.type = STATE_TYPE::BATTLE_PHASE;
		state.life_threshold = 1000.0f;
		state.movements.clear();
		state.weapons.clear();
			// 1st movement
			movement.type = MOVEMENT_TYPE::DISPLACEMENT;
			movement.target_position = Vec2(450.0f, 560.0f);
			scaleByDesign(movement.target_position);
			movement.move_param.displmt.facing_dir = false;
			movement.move_param.displmt.speed = 100.0f;
			state.movements.push_back(movement);

			// Weapon group
			state.weapons.push_back(1);
			state.weapons.push_back(2);
		ef_info.state_infoes.push_back(state);
		//

		// 3rd state - Battle Phase State
		state.id = 3;
		state.type = STATE_TYPE::BATTLE_PHASE;
		state.life_threshold = 800.0f;
		state.movements.clear();
		state.weapons.clear();
			// 1st movement
			movement.type = MOVEMENT_TYPE::DISPLACEMENT;
			movement.target_position = Vec2(250.0f, 560.0f);
			scaleByDesign(movement.target_position);
			movement.move_param.displmt.facing_dir = false;
			movement.move_param.displmt.speed = 100.0f;
			state.movements.push_back(movement);

			// Weapon group
			state.weapons.push_back(1);
			state.weapons.push_back(2);
		ef_info.state_infoes.push_back(state);
		//

		// 4th state - Battle Phase State
		state.id = 4;
		state.type = STATE_TYPE::BATTLE_PHASE;
		state.life_threshold = -FLT_MAX; // the last battle phase state
		state.movements.clear();
		state.weapons.clear();
			// 1st movement
			movement.type = MOVEMENT_TYPE::ROTATION;
			movement.target_position = Vec2(450.0f, 560.0f);
			scaleByDesign(movement.target_position);
			movement.move_param.rotation.angle = 360.0f;
			movement.move_param.rotation.speed = 60.0f;
			movement.move_param.rotation.jump = false;
			state.movements.push_back(movement);

			// 2nd movement
			//movement.type = MOVEMENT_TYPE::STAY;
			//movement.target_position = Vec2(450.0f, 560.0f);
			//scaleByDesign(movement.target_position);
			//movement.move_param.stay.period = 10.0f;
			//movement.move_param.stay.angle = 30.0f;
			//movement.move_param.stay.jump_rotate = false;
			//state.movements.push_back(movement);

			// Weapon group
			state.weapons.push_back(3);
			state.weapons.push_back(4);
		ef_info.state_infoes.push_back(state);
		//

		// 5th state - Leave State
		state.id = 5;
		state.type = STATE_TYPE::LEAVE;
		state.leave_speed = 1000.0f;
		state.movements.clear();
		state.weapons.clear();
		ef_info.state_infoes.push_back(state);
		//

		// 6th state - Dead State
		state.id = 6;
		state.type = STATE_TYPE::DEAD;
		state.movements.clear();
		state.weapons.clear();
		ef_info.state_infoes.push_back(state);
		//

		// 7th state - BattleEnd State
		state.id = 7;
		state.type = STATE_TYPE::BATTLE_END;
		state.movements.clear();
		state.weapons.clear();
		ef_info.state_infoes.push_back(state);
		//

		// state maps
		StateMapInfo state_map;
		state_map.event = STATE_MACHINE_EVENT::START;
		state_map.from = -1;
		state_map.to = 1;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = STATE_MACHINE_EVENT::AI_DEAD;
		state_map.from = -1;
		state_map.to = 6;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = STATE_MACHINE_EVENT::HOUND_DEAD;
		state_map.from = -1;
		state_map.to = 7;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = STATE_MACHINE_EVENT::LEVEL_QUIT;
		state_map.from = -1;
		state_map.to = 7;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = STATE_MACHINE_EVENT::ENTRY_FINISHED;
		state_map.from = 1;
		state_map.to = 2;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = STATE_MACHINE_EVENT::BATTLE_PHASE_FINISHED;
		state_map.from = 2;
		state_map.to = 3;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = STATE_MACHINE_EVENT::BATTLE_PHASE_FINISHED;
		state_map.from = 3;
		state_map.to = 4;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = STATE_MACHINE_EVENT::BATTLE_PHASE_FINISHED;
		state_map.from = 4;
		state_map.to = 5;
		ef_info.state_map_infoes.push_back(state_map);
		//
	
	w_info.enemies.push_back(ef_info);

	ef_info.state_infoes.clear();
	ef_info.state_map_infoes.clear();

	// enemy 2
	ef_info.start_position = Vec2(420.0f, 560.0f);
	scaleByDesign(ef_info.start_position);
	w_info.enemies.push_back(ef_info);

	// enemy 3
	ef_info.start_position = Vec2(320.0f, 760.0f);
	scaleByDesign(ef_info.start_position);
	w_info.enemies.push_back(ef_info);

	m_levelInfo.enemy_waves.push_back(w_info);	
	// end of 1st wave

	w_info.enemies.clear();
	ef_info.state_infoes.clear();
	ef_info.state_map_infoes.clear();
	ef_info.weapons.clear();

	// 2nd wave
	ef_info.type = ENEMY_TYPE::FRIGATE_PUMA;
	ef_info.max_life = 200.0f;
	ef_info.armor = 300.0f;
	ef_info.bounding_circle_radius = 60.0f;
	ef_info.body_texture_name = "n2.png";
	ef_info.facing_dir = Vec2::UNIT_Y;

	ef_info.start_position = Vec2(220.0f, 610.0f);
	scaleByDesign(ef_info.start_position);

	w_info.enemies.push_back(ef_info);

	ef_info.start_position = Vec2(420.0f, 610.0f);
	scaleByDesign(ef_info.start_position);

	w_info.enemies.push_back(ef_info);

	ef_info.start_position = Vec2(320.0f, 710.0f);
	scaleByDesign(ef_info.start_position);

	w_info.enemies.push_back(ef_info);
	m_levelInfo.enemy_waves.push_back(w_info);	
	// end of 2nd wave

	w_info.enemies.clear();
	ef_info.state_infoes.clear();
	ef_info.state_map_infoes.clear();
	ef_info.weapons.clear();

	// 3rd wave
	ef_info.type = ENEMY_TYPE::CARRIER_TIGER;
	ef_info.max_life = 500.0f;
	ef_info.armor = 500.0f;
	ef_info.bounding_circle_radius = 150.0f;
	ef_info.body_texture_name = "n_boss.png";
	ef_info.start_position = Vec2(320.0f, 610.0f);
	scaleByDesign(ef_info.start_position);
	ef_info.facing_dir = Vec2::UNIT_Y;
	w_info.enemies.push_back(ef_info);

	m_levelInfo.enemy_waves.push_back(w_info);	
	// end of 3rd wave

	return true;
}

float AppDelegate::getHoundMaxLife(void)
{
	return 1000.0f;
}

float AppDelegate::getHoundArmor(void)
{
	return 500.0f;
}

const std::pair<float, float> AppDelegate::getEnemyMaxLifeAndArmor(ENEMY_TYPE type, int level)
{
	return std::make_pair<float, float>(2000.0f, 300.0f);
}

float AppDelegate::getDamageRatio(PROJECTILE_TYPE proj_type, ARMOR_TYPE armor_type)
{
	return 1.0f;
}
