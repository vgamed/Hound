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

	// Data Center load static data
	if (!DataCenter::getInstance()->loadStaticData())
	{
		return false;
	}

	// load Hound static data
	//...

	// load player information from database
	if (!loadPlayerInfo())
	{
		return false;
	}

	// load specific level data from xml file
	if (!DataCenter::getInstance()->loadLevelInfo(1, m_levelInfo))
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
	// add search pathes
	FileUtils::getInstance()->addSearchPath("backgrounds");
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
	m_playerInfo.hound.body_type = (int)BODY_TYPE::BASIC;
	m_playerInfo.hound.body_texture_name = "mplane.png";
	m_playerInfo.hound.armor_level = 1;
	m_playerInfo.hound.armor_type = (int)ARMOR_TYPE::ENEGY_SHIELD;
	m_playerInfo.hound.armor_texture_name = "";
	m_playerInfo.hound.engine_level = 1;
	m_playerInfo.hound.engine_type = (int)ENGINE_TYPE::BASIC;
	m_playerInfo.hound.engine_texture_name = "";

	m_playerInfo.hound.scale_xy = 1.0f;
	scaleByDesign(m_playerInfo.hound.scale_xy);

	m_playerInfo.hound.bounding_circle_radius = 10.0f;

	m_playerInfo.hound.weapons.clear();

	BarrelInfo barrel;
	barrel.type = (int)BARREL_TYPE::BULLET;
	barrel.projectile_type = (int)PROJECTILE_TYPE::BULLET_NORMAL;
	
	barrel.projectile_scale_xy = 0.6f;
	scaleByDesign(barrel.projectile_scale_xy);

	//barrel.projectile_effect_name = "bullet_1.png";
	barrel.projectile_asset_name = "bullet18.png";
	barrel.projectile_damage = 2.0f;
	barrel.projectile_speed = 200.0f;
	barrel.firing_interval = 0.1f; //second

	WeaponInfo weapon;
	weapon.level  = 1;
	weapon.type = (int)WEAPON_TYPE::CANNON;
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

float AppDelegate::getHoundMaxLife(void)
{
	return 1000.0f;
}

float AppDelegate::getHoundArmor(void)
{
	return 500.0f;
}

const std::pair<float, float> AppDelegate::getEnemyMaxLifeAndArmor(int type, int level)
{
	return std::make_pair<float, float>(2000.0f, 300.0f);
}

void AppDelegate::stringToVec2(const std::string &str, cocos2d::Vec2 &vec)
{
	vec.x = FLT_MAX;
	vec.y = FLT_MAX;

	std::size_t pos = str.find(',');
	if (pos == std::string::npos)
	{
		return;
	}

	tinyxml2::XMLUtil::ToFloat(str.substr(0, pos).c_str(), &vec.x);
	tinyxml2::XMLUtil::ToFloat(str.substr(pos+1).c_str(), &vec.y);
}

void AppDelegate::charToString(const char *pstr, std::string &ret)
{
	ret = pstr==nullptr ? "" : pstr;
}
