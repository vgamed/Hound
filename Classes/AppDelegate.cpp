#include "AppDelegate.h"
#include "GameScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        //glview = GLView::create("My Game");
		glview = GLView::createWithRect("Hound", Rect(0, 0, 640, 960));
        director->setOpenGLView(glview);
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

	AnimationCache::getInstance()->addAnimationsWithFile("pig.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("nplane.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("bullet.plist");
	AnimationCache::getInstance()->addAnimationsWithFile("wsparticle_p01.plist");

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
	m_playerInfo.hound.scale_xy = 1.2f;
	m_playerInfo.hound.bounding_circle_radius = 10.0f;

	m_playerInfo.hound.weapons.clear();

	BarrelInfo barrel;
	barrel.type = BARREL_TYPE::BULLET;
	barrel.projectile_scale_xy = 0.4f;
	barrel.effect_name = "bullet_1.png";

	WeaponInfo weapon;
	weapon.level  = 1;
	weapon.type = WEAPON_TYPE::CANNON;
	weapon.firing_interval = 0.1f; //second
	weapon.projectile_speed = 1000.0f; //per second
	weapon.projectile_damage = 10.0f;

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
	m_levelInfo.hound_start_offset = Vec2(0.0f, 200.0f);
	m_levelInfo.sbg_layer_texture_names.push_back("img_bg_1.jpg");
	m_levelInfo.sbg_layer_texture_names.push_back("clouds.png");

	WaveInfo w_info;
	EnemyInfo ef_info;

	w_info.time_offset = 1.0f;
	ef_info.type = ENEMY_TYPE::FIGHTER_BEE;
	ef_info.level = 1;
	ef_info.scale_xy = 1.0f;
	ef_info.bounding_circle_radius = 50.0f;
	ef_info.body_texture_name = "n1.png";
	ef_info.start_position_offset = Vec2(-100.0f, -50.0f);
	w_info.enemies.push_back(ef_info);
	ef_info.start_position_offset = Vec2(100.0f, -50.0f);
	w_info.enemies.push_back(ef_info);
	ef_info.start_position_offset = Vec2(0.0f, 50.0f);
	w_info.enemies.push_back(ef_info);
	m_levelInfo.enemy_waves.push_back(w_info);	// 1st wave

	w_info.enemies.clear();
	ef_info.type = ENEMY_TYPE::FRIGATE_PUMA;
	ef_info.bounding_circle_radius = 60.0f;
	ef_info.body_texture_name = "n2.png";
	ef_info.start_position_offset = Vec2(-100.0f, -50.0f);
	w_info.enemies.push_back(ef_info);
	ef_info.start_position_offset = Vec2(100.0f, -50.0f);
	w_info.enemies.push_back(ef_info);
	ef_info.start_position_offset = Vec2(0.0f, 50.0f);
	w_info.enemies.push_back(ef_info);
	m_levelInfo.enemy_waves.push_back(w_info);	// 2nd wave

	w_info.enemies.clear();
	ef_info.type = ENEMY_TYPE::CARRIER_TIGER;
	ef_info.bounding_circle_radius = 150.0f;
	ef_info.body_texture_name = "n_boss.png";
	ef_info.start_position_offset = Vec2(0.0f, 0.0f);
	w_info.enemies.push_back(ef_info);
	m_levelInfo.enemy_waves.push_back(w_info);	// 3rd wave

	return true;
}
