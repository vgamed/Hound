#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;

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
	m_playerInfo.hound.armor_type = ARMOR_TYPE::BASIC;
	m_playerInfo.hound.armor_texture_name = "";
	m_playerInfo.hound.engine_level = 1;
	m_playerInfo.hound.engine_type = ENGINE_TYPE::BASIC;
	m_playerInfo.hound.engine_texture_name = "";

	m_playerInfo.hound.scale_xy = 1.0f;
	scaleByDesign(m_playerInfo.hound.scale_xy);

	m_playerInfo.hound.bounding_circle_radius = 10.0f;

	m_playerInfo.hound.weapons.clear();

	BarrelInfo barrel;
	barrel.type = BARREL_TYPE::BULLET;
	barrel.projectile_type = PROJECTILE_TYPE::BULLET_NORMAL;
	
	barrel.projectile_scale_xy = 0.4f;
	scaleByDesign(barrel.projectile_scale_xy);

	barrel.projectile_effect_name = "bullet_1.png";
	barrel.projectile_damage = 2.0f;
	barrel.projectile_speed = 200.0f;
	barrel.firing_interval = 0.1f; //second

	WeaponInfo weapon;
	weapon.level  = 1;
	weapon.type = WEAPON_TYPE::CANNON;
	weapon.time_offset_firing_start = 2.0f; //second
	weapon.time_offset_firing_stop = FLT_MAX; //second
	weapon.speed = 1000.0f; //per second
	weapon.damage = 8.0f;

	weapon.texture_name = "frontgun.png";
	weapon.dock_position = Vec2(58.0f, 81.0f);
	weapon.barrells.clear();
	barrel.rotate_angle = 0.0f;
	weapon.barrells.push_back(barrel);
	m_playerInfo.hound.weapons.push_back(weapon);

	weapon.texture_name = "leftgun.png";
	weapon.dock_position = Vec2(29.0f, 65.0f);
	weapon.barrells.clear();
	barrel.rotate_angle = 0.0f;
	weapon.barrells.push_back(barrel);
	barrel.rotate_angle = -30.0f;
	weapon.barrells.push_back(barrel);
	barrel.rotate_angle = 30.0f;
	weapon.barrells.push_back(barrel);
	m_playerInfo.hound.weapons.push_back(weapon);

	weapon.texture_name = "rightgun.png";
	weapon.dock_position = Vec2(87.0f, 65.0f);
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

	w_info.time_offset = 2.0f;
	ef_info.type = ENEMY_TYPE::FIGHTER_BEE;
	ef_info.level = 1;
	ef_info.max_life = 100.0f;
	ef_info.armor = 100.0f;
	
	ef_info.scale_xy = 1.0f;
	scaleByDesign(ef_info.scale_xy);

	ef_info.bounding_circle_radius = 50.0f;
	ef_info.body_texture_name = "n1.png";

	// 1st wave
	// 1st enemy
	ef_info.start_position = Vec2(200.0f, 960.0f);
	scaleByDesign(ef_info.start_position);

	// AI States
	StateInfo state;
	StateMapInfo state_map;
	Movement movement;

	// 1st state
	state.id = 1;
	state.type = STATE_TYPE::ENTRY;
	// 1st movement
	movement.type = MOVEMENT_TYPE::DISPLACEMENT;

	movement.target_position = Vec2(200.0f, 560.0f);
	scaleByDesign(movement.target_position);

	movement.move_param.displmt.facing_dir = true;
	movement.move_param.displmt.speed = 100.0f;
	state.movements.push_back(movement);
	// 2nd movement
	movement.type = MOVEMENT_TYPE::ROTATION;

	movement.target_position = Vec2(200.0f, 560.0f);
	scaleByDesign(movement.target_position);

	movement.move_param.rotation.angle = 360.0f;
	movement.move_param.rotation.speed = 20.0f;
	state.movements.push_back(movement);
	// 3rd movement
	movement.type = MOVEMENT_TYPE::DISPLACEMENT;

	movement.target_position = Vec2(450.0f, 560.0f);
	scaleByDesign(movement.target_position);

	movement.move_param.displmt.facing_dir = false;
	movement.move_param.displmt.speed = 100.0f;
	state.movements.push_back(movement);
	// 4th movement
	movement.type = MOVEMENT_TYPE::STAY;

	movement.target_position = Vec2(450.0f, 560.0f);
	scaleByDesign(movement.target_position);

	movement.move_param.stay.period = 10.0f;
	movement.move_param.stay.angle = 30.0f;
	state.movements.push_back(movement);
	// 5th movement
	movement.type = MOVEMENT_TYPE::DISPLACEMENT;

	movement.target_position = Vec2(450.0f, 0.0f);
	scaleByDesign(movement.target_position);

	movement.move_param.displmt.facing_dir = true;
	movement.move_param.displmt.speed = 1000.0f;
	state.movements.push_back(movement);

	ef_info.state_infoes.push_back(state);

	state_map.event = STATE_MACHINE_EVENT::START;
	state_map.from = -1;
	state_map.to = 1;
	ef_info.state_map_infoes.push_back(state_map);
	
	w_info.enemies.push_back(ef_info);

	ef_info.state_infoes.clear();
	ef_info.state_map_infoes.clear();

	ef_info.start_position = Vec2(420.0f, 560.0f);
	scaleByDesign(ef_info.start_position);
	w_info.enemies.push_back(ef_info);

	ef_info.start_position = Vec2(320.0f, 760.0f);
	scaleByDesign(ef_info.start_position);
	w_info.enemies.push_back(ef_info);

	m_levelInfo.enemy_waves.push_back(w_info);	// 1st wave

	w_info.enemies.clear();
	ef_info.type = ENEMY_TYPE::FRIGATE_PUMA;
	ef_info.max_life = 200.0f;
	ef_info.armor = 300.0f;
	ef_info.bounding_circle_radius = 60.0f;
	ef_info.body_texture_name = "n2.png";

	ef_info.start_position = Vec2(220.0f, 610.0f);
	scaleByDesign(ef_info.start_position);

	w_info.enemies.push_back(ef_info);

	ef_info.start_position = Vec2(420.0f, 610.0f);
	scaleByDesign(ef_info.start_position);

	w_info.enemies.push_back(ef_info);

	ef_info.start_position = Vec2(320.0f, 710.0f);
	scaleByDesign(ef_info.start_position);

	w_info.enemies.push_back(ef_info);
	m_levelInfo.enemy_waves.push_back(w_info);	// 2nd wave

	w_info.enemies.clear();
	ef_info.type = ENEMY_TYPE::CARRIER_TIGER;
	ef_info.max_life = 500.0f;
	ef_info.armor = 500.0f;
	ef_info.bounding_circle_radius = 150.0f;
	ef_info.body_texture_name = "n_boss.png";
	ef_info.start_position = Vec2(320.0f, 610.0f);
	scaleByDesign(ef_info.start_position);
	w_info.enemies.push_back(ef_info);

	m_levelInfo.enemy_waves.push_back(w_info);	// 3rd wave

	return true;
}
