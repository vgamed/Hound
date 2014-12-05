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

	// add search pathes
	FileUtils::getInstance()->addSearchPath("backgrounds");
	FileUtils::getInstance()->addSearchPath("bullet");
	FileUtils::getInstance()->addSearchPath("fonts");

	// Data Center Initialization
	DataCenter::getInstance()->loadStaticData();

	initCommonTypes();

	// load all game resources and information into caches
	if (!loadGameResources())
	{
		return false;
	}

	if (!loadPlayerInfo())
	{
		return false;
	}

	if (!loadLevelInfoFromXML(1))
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

bool AppDelegate::loadLevelInfoFromXML(int id)
{
	std::stringstream buffer;
	std::string str;

	buffer << "level_info_" << id << ".xml";
	str = FileUtils::getInstance()->fullPathForFilename(buffer.str());

	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();
	auto ret = doc->LoadFile(str.c_str());
	if (ret != tinyxml2::XML_NO_ERROR)
	{
		return false;
	}

	const tinyxml2::XMLElement* e_root = nullptr;
	const tinyxml2::XMLElement* e_child_1 = nullptr;
	const tinyxml2::XMLElement* e_child_2 = nullptr;
	const tinyxml2::XMLElement* e_child_3 = nullptr;
	const tinyxml2::XMLElement* e_child_4 = nullptr;
	const tinyxml2::XMLElement* e_child_5 = nullptr;
	const tinyxml2::XMLElement* e_child_6 = nullptr;
	const tinyxml2::XMLElement* e_child_7 = nullptr;
	const tinyxml2::XMLElement* e_child_8 = nullptr;

	// set level id
	e_root = doc->RootElement();
	CC_ASSERT(e_root != nullptr);
	m_levelInfo.id = e_root->FindAttribute("id")->IntValue();
	//

	// Section: hound entry
	e_child_1 = e_root->FirstChildElement("HoundEntry");
	CC_ASSERT(e_child_1 != nullptr);
	{
		std::string str;
		str = e_child_1->FirstChildElement("From")->GetText();
		stringToVec2(str, m_levelInfo.hound_entry_from);
		str = e_child_1->FirstChildElement("To")->GetText();
		stringToVec2(str, m_levelInfo.hound_entry_to);
		e_child_1->FirstChildElement("Speed")->QueryFloatText(&m_levelInfo.hound_entry_speed);
		e_child_1->FirstChildElement("AutoFacing")->QueryBoolText(&m_levelInfo.hound_entry_auto_facing);
	}
	//

	// Section: hound leave
	e_child_1 = e_root->FirstChildElement("HoundLeave");
	CC_ASSERT(e_child_1 != nullptr);
		e_child_1->FirstChildElement("Speed")->QueryFloatText(&m_levelInfo.hound_leave_speed);
		e_child_1->FirstChildElement("AutoFacing")->QueryBoolText(&m_levelInfo.hound_leave_auto_facing);
	//

	// Section: backgrounds
	e_child_1 = e_root->FirstChildElement("Backgrounds");
	CC_ASSERT(e_child_1 != nullptr);
	for (e_child_2 = e_child_1->FirstChildElement(); 
		e_child_2 != nullptr;
		e_child_2 = e_child_2->NextSiblingElement())
	{
		m_levelInfo.sbg_layer_texture_names.push_back(e_child_2->GetText());
	}
	//

	// Section: Enemy Waves
	WaveInfo w_info;
	EnemyInfo ef_info;
	BarrelInfo barrel;
	WeaponInfo weapon;
	StateInfo state;
	StateMapInfo state_map;
	Movement movement;

	e_child_1 = e_root->FirstChildElement("EnemyWaves");
	if (e_child_1 == nullptr)
	{
		return true; // empty level
	}

	// for each wave
	for (e_child_2 = e_child_1->FirstChildElement(); 
		e_child_2 != nullptr;
		e_child_2 = e_child_2->NextSiblingElement())
	{
		w_info.time_offset = e_child_2->FindAttribute("time_offset")->FloatValue();
		// for each enemy
		for (e_child_3 = e_child_2->FirstChildElement(); 
			e_child_3 != nullptr;
			e_child_3 = e_child_3->NextSiblingElement())
		{
			ef_info.id = e_child_3->FindAttribute("id")->IntValue();
			charToString(e_child_3->FirstChildElement("Type")->GetText(), str);
			ef_info.type = getCommonType(str);
			e_child_3->FirstChildElement("Level")->QueryUnsignedText(&ef_info.level);
			auto value = getEnemyMaxLifeAndArmor(ef_info.type, ef_info.level);
			ef_info.max_life = value.first;
			ef_info.armor = value.second;
			e_child_3->FirstChildElement("Scale")->QueryFloatText(&ef_info.scale_xy);
			e_child_3->FirstChildElement("BoundingCircleRadius")->QueryFloatText(&ef_info.bounding_circle_radius);
			e_child_3->FirstChildElement("RotateAngle")->QueryFloatText(&ef_info.rotate_angle);
			charToString(e_child_3->FirstChildElement("Asset")->GetText(), ef_info.body_texture_name);
			// entry
			e_child_4 = e_child_3->FirstChildElement("Entry");
			charToString(e_child_4->FirstChildElement("From")->GetText(), str);
			stringToVec2(str, ef_info.entry_from);
			charToString(e_child_4->FirstChildElement("To")->GetText(), str);
			stringToVec2(str, ef_info.entry_to);
			e_child_4->FirstChildElement("Speed")->QueryFloatText(&ef_info.entry_speed);
			e_child_4->FirstChildElement("AutoFacing")->QueryBoolText(&ef_info.entry_auto_facing);
			// leave
			e_child_4 = e_child_3->FirstChildElement("Leave");
			e_child_4->FirstChildElement("Speed")->QueryFloatText(&ef_info.leave_speed);
			e_child_4->FirstChildElement("AutoFacing")->QueryBoolText(&ef_info.leave_auto_facing);
			
			// for each weapon
			e_child_4 = e_child_3->FirstChildElement("Weapons");
			if (e_child_4 != nullptr)
			{
				for (e_child_5 = e_child_4->FirstChildElement(); 
					e_child_5 != nullptr;
					e_child_5 = e_child_5->NextSiblingElement())
				{
					weapon.id = e_child_5->FindAttribute("id")->IntValue();
					charToString(e_child_5->FirstChildElement("Type")->GetText(), str);
					weapon.type = getCommonType(str);
					e_child_5->FirstChildElement("Level")->QueryUnsignedText(&weapon.level);
					e_child_5->FirstChildElement("Speed")->QueryFloatText(&weapon.speed);
					e_child_5->FirstChildElement("Damage")->QueryFloatText(&weapon.damage);
					e_child_5->FirstChildElement("RotateAngle")->QueryFloatText(&weapon.rotate_angle);
					e_child_5->FirstChildElement("AutoAim")->QueryBoolText(&weapon.auto_aim);
					charToString(e_child_5->FirstChildElement("DockAt")->GetText(), str);
					stringToVec2(str, weapon.dock_position);
					e_child_5->FirstChildElement("FiringStart")->QueryFloatText(&weapon.time_offset_firing_start);
					e_child_5->FirstChildElement("FiringStop")->QueryFloatText(&weapon.time_offset_firing_stop);
					charToString(e_child_5->FirstChildElement("Asset")->GetText(), weapon.texture_name);

					// for each barrel
					e_child_6 = e_child_5->FirstChildElement("Barrells");
					if (e_child_6 != nullptr)
					{
						for (e_child_7 = e_child_6->FirstChildElement(); 
							e_child_7 != nullptr;
							e_child_7 = e_child_7->NextSiblingElement())
						{
							charToString(e_child_7->FindAttribute("type")->Value(), str);
							barrel.type = getCommonType(str);
							e_child_7->FirstChildElement("RotateAngle")->QueryFloatText(&barrel.rotate_angle);
							e_child_7->FirstChildElement("FiringInterval")->QueryFloatText(&barrel.firing_interval);
							charToString(e_child_7->FirstChildElement("ProjectileType")->GetText(), str);
							barrel.projectile_type = getCommonType(str);
							e_child_7->FirstChildElement("ProjectileScale")->QueryFloatText(&barrel.projectile_scale_xy);
							e_child_7->FirstChildElement("ProjectileDamage")->QueryFloatText(&barrel.projectile_damage);
							e_child_7->FirstChildElement("ProjectileSpeed")->QueryFloatText(&barrel.projectile_speed);
							charToString(e_child_7->FirstChildElement("Asset")->GetText(), barrel.projectile_asset_name);

							weapon.barrells.push_back(barrel);
						}
					}

					ef_info.weapons.push_back(weapon);
					weapon.barrells.clear();
				}
			}

			// for each state
			e_child_4 = e_child_3->FirstChildElement("States");
			if (e_child_4 != nullptr) 
			{
				for (e_child_5 = e_child_4->FirstChildElement(); 
					e_child_5 != nullptr;
					e_child_5 = e_child_5->NextSiblingElement())
				{
					state.id = e_child_5->FindAttribute("id")->IntValue();
					charToString(e_child_5->FindAttribute("type")->Value(), str);
					state.type = getCommonType(str);

					state.life_threshold = -FLT_MAX;
					e_child_6 = e_child_5->FirstChildElement("LifeThreshold");
					if (e_child_6 != nullptr)
					{
						e_child_6->QueryFloatText(&state.life_threshold);
					}

					state.repeat_movements = false;
					e_child_6 = e_child_5->FirstChildElement("RepeatMovements");
					if (e_child_6 != nullptr)
					{
						e_child_6->QueryBoolText(&state.repeat_movements);
					}

					// for each movement
					e_child_6 = e_child_5->FirstChildElement("Movements");
					if (e_child_6 != nullptr)
					{
						for (e_child_7 = e_child_6->FirstChildElement(); 
							e_child_7 != nullptr;
							e_child_7 = e_child_7->NextSiblingElement())
						{
							charToString(e_child_7->FindAttribute("type")->Value(), str);
							movement.type = getCommonType(str);
							charToString(e_child_7->FirstChildElement("TargetPosition")->GetText(), str);
							stringToVec2(str, movement.target_position);

							movement.displmt_auto_facing = false;
							e_child_8 = e_child_7->FirstChildElement("DisplacementAutoFacing");
							if (e_child_8 != nullptr)
							{
								e_child_8->QueryBoolText(&movement.displmt_auto_facing);
							}

							movement.stay_period = FLT_MAX;
							e_child_8 = e_child_7->FirstChildElement("StayPeriod");
							if (e_child_8 != nullptr)
							{
								e_child_8->QueryFloatText(&movement.stay_period);
							}

							movement.target_angle = 0.0f;
							e_child_8 = e_child_7->FirstChildElement("TargetAngle");
							if (e_child_8 != nullptr)
							{
								e_child_8->QueryFloatText(&movement.target_angle);
							}

							movement.speed = 0.0f;
							e_child_8 = e_child_7->FirstChildElement("Speed");
							if (e_child_8 != nullptr)
							{
								e_child_8->QueryFloatText(&movement.speed);
							}

							movement.jump = false;
							e_child_8 = e_child_7->FirstChildElement("Jump");
							if (e_child_8 != nullptr)
							{
								e_child_8->QueryBoolText(&movement.jump);
							}

							state.movements.push_back(movement);
						}
					}

					// for each weapon index
					e_child_6 = e_child_5->FirstChildElement("WeaponGroup");
					if (e_child_6 != nullptr)
					{
						for (e_child_7 = e_child_6->FirstChildElement(); 
							e_child_7 != nullptr;
							e_child_7 = e_child_7->NextSiblingElement())
						{
							int weapon_index;
							e_child_7->QueryIntText(&weapon_index);
							state.weapons.push_back(weapon_index);
						}
					}
				
					ef_info.state_infoes.push_back(state);
					state.movements.clear();
					state.weapons.clear();
				}
			}
			
			// for each state transition
			e_child_4 = e_child_3->FirstChildElement("StateTransitions");
			if (e_child_4 != nullptr)
			{
				for (e_child_5 = e_child_4->FirstChildElement(); 
					e_child_5 != nullptr;
					e_child_5 = e_child_5->NextSiblingElement())
				{
					charToString(e_child_5->FirstChildElement("Event")->GetText(), str);
					state_map.event = getCommonType(str);
					e_child_5->FirstChildElement("From")->QueryIntText(&state_map.from);
					e_child_5->FirstChildElement("To")->QueryIntText(&state_map.to);
					ef_info.state_map_infoes.push_back(state_map);
				}
			}

			w_info.enemies.push_back(ef_info);
			ef_info.weapons.clear();
			ef_info.state_infoes.clear();
			ef_info.state_map_infoes.clear();
		}
		m_levelInfo.enemy_waves.push_back(w_info);
		w_info.enemies.clear();
	}
	//

	return true;
}

bool AppDelegate::loadLevelInfo(void)
{
	m_levelInfo.id = 0;

	m_levelInfo.hound_entry_from = Vec2(320.0f, -50.0f);
	scaleByDesign(m_levelInfo.hound_entry_from);
	m_levelInfo.hound_entry_to = Vec2(320.0f, 150.0f);
	scaleByDesign(m_levelInfo.hound_entry_to);
	m_levelInfo.hound_entry_speed = 300.0f;
	m_levelInfo.hound_entry_auto_facing = true;
	m_levelInfo.hound_leave_speed = 1000.0f;
	m_levelInfo.hound_leave_auto_facing = true;

	m_levelInfo.sbg_layer_texture_names.push_back("img_bg_1.jpg");
	m_levelInfo.sbg_layer_texture_names.push_back("clouds.png");

	WaveInfo w_info;
	EnemyInfo ef_info;
	BarrelInfo barrel;
	WeaponInfo weapon;
	StateInfo state;
	Movement movement;

	// 1st wave
	w_info.time_offset = 2.0f;
	ef_info.type = (int)ENEMY_TYPE::FIGHTER_BEE;
	ef_info.level = 1;
	ef_info.max_life = 1200.0f;
	ef_info.armor = 100.0f;
	
	ef_info.scale_xy = 1.0f;
	scaleByDesign(ef_info.scale_xy);

	ef_info.bounding_circle_radius = 50.0f;
	ef_info.body_texture_name = "n1.png";

	// weapons
	barrel.type = (int)BARREL_TYPE::BULLET;
	barrel.projectile_type = (int)PROJECTILE_TYPE::BULLET_NORMAL;
	
	barrel.projectile_scale_xy = 0.6f;
	scaleByDesign(barrel.projectile_scale_xy);

	barrel.projectile_asset_name = "bullet0.png";
	barrel.rotate_angle = 0.0f;
	barrel.projectile_damage = 200.0f;
	barrel.projectile_speed = 200.0f;
	barrel.firing_interval = 0.2f; //second

	weapon.level  = 1;
	weapon.type = (int)WEAPON_TYPE::CANNON;
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
	ef_info.rotate_angle = 0.0f;
	ef_info.entry_from = Vec2(200.0f, 960.0f);
	scaleByDesign(ef_info.entry_from);
	ef_info.entry_to = Vec2(200.0f, 560.0f);
	scaleByDesign(ef_info.entry_to);
	ef_info.entry_speed = 300.0f;
	ef_info.entry_auto_facing = true;
	ef_info.leave_speed = 1000.0f;
	ef_info.leave_auto_facing = true;

		// 1st state - Entry State
		state.id = 1;
		state.type = (int)STATE_TYPE::ENTRY;
		state.movements.clear();
		state.weapons.clear();
		ef_info.state_infoes.push_back(state);
		//

		// 2nd state - Battle Phase State
		state.id = 2;
		state.type = (int)STATE_TYPE::BATTLE_PHASE;
		state.life_threshold = 1000.0f;
		state.movements.clear();
		state.weapons.clear();
			// 1st movement
			movement.type = (int)MOVEMENT_TYPE::DISPLACEMENT;
			movement.target_position = Vec2(450.0f, 560.0f);
			scaleByDesign(movement.target_position);
			movement.displmt_auto_facing = false;
			movement.speed = 100.0f;
			state.movements.push_back(movement);

			// Weapon group
			state.weapons.push_back(1);
			state.weapons.push_back(2);
		ef_info.state_infoes.push_back(state);
		//

		// 3rd state - Battle Phase State
		state.id = 3;
		state.type = (int)STATE_TYPE::BATTLE_PHASE;
		state.life_threshold = 800.0f;
		state.movements.clear();
		state.weapons.clear();
			// 1st movement
			movement.type = (int)MOVEMENT_TYPE::DISPLACEMENT;
			movement.target_position = Vec2(250.0f, 560.0f);
			scaleByDesign(movement.target_position);
			movement.displmt_auto_facing = false;
			movement.speed = 100.0f;
			state.movements.push_back(movement);

			// Weapon group
			state.weapons.push_back(1);
			state.weapons.push_back(2);
		ef_info.state_infoes.push_back(state);
		//

		// 4th state - Battle Phase State
		state.id = 4;
		state.type = (int)STATE_TYPE::BATTLE_PHASE;
		state.life_threshold = -FLT_MAX; // the last battle phase state
		state.movements.clear();
		state.weapons.clear();
			// 1st movement
			movement.type = (int)MOVEMENT_TYPE::ROTATION;
			movement.target_position = Vec2(450.0f, 560.0f);
			scaleByDesign(movement.target_position);
			movement.target_angle = 360.0f;
			movement.speed = 60.0f;
			movement.jump = false;
			state.movements.push_back(movement);

			// 2nd movement
			//movement.type = (int)MOVEMENT_TYPE::STAY;
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
		state.type = (int)STATE_TYPE::LEAVE;
		state.movements.clear();
		state.weapons.clear();
		ef_info.state_infoes.push_back(state);
		//

		// 6th state - Dead State
		state.id = 6;
		state.type = (int)STATE_TYPE::DEAD;
		state.movements.clear();
		state.weapons.clear();
		ef_info.state_infoes.push_back(state);
		//

		// 7th state - BattleEnd State
		state.id = 7;
		state.type = (int)STATE_TYPE::BATTLE_END;
		state.movements.clear();
		state.weapons.clear();
		ef_info.state_infoes.push_back(state);
		//

		// state maps
		StateMapInfo state_map;
		state_map.event = (int)STATE_MACHINE_EVENT::START;
		state_map.from = -1;
		state_map.to = 1;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = (int)STATE_MACHINE_EVENT::AI_DEAD;
		state_map.from = -1;
		state_map.to = 6;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = (int)STATE_MACHINE_EVENT::HOUND_DEAD;
		state_map.from = -1;
		state_map.to = 7;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = (int)STATE_MACHINE_EVENT::LEVEL_QUIT;
		state_map.from = -1;
		state_map.to = 7;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = (int)STATE_MACHINE_EVENT::ENTRY_FINISHED;
		state_map.from = 1;
		state_map.to = 2;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = (int)STATE_MACHINE_EVENT::BATTLE_PHASE_FINISHED;
		state_map.from = 2;
		state_map.to = 3;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = (int)STATE_MACHINE_EVENT::BATTLE_PHASE_FINISHED;
		state_map.from = 3;
		state_map.to = 4;
		ef_info.state_map_infoes.push_back(state_map);

		state_map.event = (int)STATE_MACHINE_EVENT::BATTLE_PHASE_FINISHED;
		state_map.from = 4;
		state_map.to = 5;
		ef_info.state_map_infoes.push_back(state_map);
		//
	
	w_info.enemies.push_back(ef_info);

	ef_info.entry_auto_facing = false;
	ef_info.entry_speed = 0.0f;
	ef_info.leave_auto_facing = false;
	ef_info.leave_speed = 0.0f;
	ef_info.state_infoes.clear();
	ef_info.state_map_infoes.clear();

	// enemy 2
	ef_info.entry_from = Vec2(420.0f, 560.0f);
	scaleByDesign(ef_info.entry_from);
	ef_info.entry_to = Vec2(420.0f, 560.0f);
	scaleByDesign(ef_info.entry_to);
		// 1st state - Entry State
		state.id = 1;
		state.type = (int)STATE_TYPE::ENTRY;
		state.movements.clear();
		state.weapons.clear();
		ef_info.state_infoes.push_back(state);
		//
		// state maps
		state_map.event = (int)STATE_MACHINE_EVENT::START;
		state_map.from = -1;
		state_map.to = 1;
		ef_info.state_map_infoes.push_back(state_map);
		//
	w_info.enemies.push_back(ef_info);

	// enemy 3
	ef_info.entry_from = Vec2(320.0f, 760.0f);
	scaleByDesign(ef_info.entry_from);
	ef_info.entry_to = Vec2(320.0f, 760.0f);
	scaleByDesign(ef_info.entry_to);
		// 1st state - Entry State
		state.id = 1;
		state.type = (int)STATE_TYPE::ENTRY;
		state.movements.clear();
		state.weapons.clear();
		ef_info.state_infoes.push_back(state);
		//
		// state maps
		state_map.event = (int)STATE_MACHINE_EVENT::START;
		state_map.from = -1;
		state_map.to = 1;
		ef_info.state_map_infoes.push_back(state_map);
		//
	w_info.enemies.push_back(ef_info);

	m_levelInfo.enemy_waves.push_back(w_info);	
	// end of 1st wave

	w_info.enemies.clear();
	ef_info.state_infoes.clear();
	ef_info.state_map_infoes.clear();
	ef_info.weapons.clear();

	// 2nd wave
	ef_info.type = (int)ENEMY_TYPE::FRIGATE_PUMA;
	ef_info.max_life = 200.0f;
	ef_info.armor = 300.0f;
	ef_info.bounding_circle_radius = 60.0f;
	ef_info.body_texture_name = "n2.png";
	ef_info.rotate_angle = 0.0f;

	ef_info.entry_from = Vec2(220.0f, 610.0f);
	scaleByDesign(ef_info.entry_from);
	ef_info.entry_to = Vec2(220.0f, 610.0f);
	scaleByDesign(ef_info.entry_to);
		// 1st state - Entry State
		state.id = 1;
		state.type = (int)STATE_TYPE::ENTRY;
		state.movements.clear();
		state.weapons.clear();
		ef_info.state_infoes.push_back(state);
		//
		// state maps
		state_map.event = (int)STATE_MACHINE_EVENT::START;
		state_map.from = -1;
		state_map.to = 1;
		ef_info.state_map_infoes.push_back(state_map);
		//
	w_info.enemies.push_back(ef_info);

	ef_info.entry_from = Vec2(420.0f, 610.0f);
	scaleByDesign(ef_info.entry_from);
	ef_info.entry_to = Vec2(420.0f, 610.0f);
	scaleByDesign(ef_info.entry_to);
		// 1st state - Entry State
		state.id = 1;
		state.type = (int)STATE_TYPE::ENTRY;
		state.movements.clear();
		state.weapons.clear();
		ef_info.state_infoes.push_back(state);
		//
		// state maps
		state_map.event = (int)STATE_MACHINE_EVENT::START;
		state_map.from = -1;
		state_map.to = 1;
		ef_info.state_map_infoes.push_back(state_map);
		//
	w_info.enemies.push_back(ef_info);

	ef_info.entry_from = Vec2(320.0f, 710.0f);
	scaleByDesign(ef_info.entry_from);
	ef_info.entry_to = Vec2(320.0f, 710.0f);
	scaleByDesign(ef_info.entry_to);
		// 1st state - Entry State
		state.id = 1;
		state.type = (int)STATE_TYPE::ENTRY;
		state.movements.clear();
		state.weapons.clear();
		ef_info.state_infoes.push_back(state);
		//
		// state maps
		state_map.event = (int)STATE_MACHINE_EVENT::START;
		state_map.from = -1;
		state_map.to = 1;
		ef_info.state_map_infoes.push_back(state_map);
		//
	w_info.enemies.push_back(ef_info);

	m_levelInfo.enemy_waves.push_back(w_info);	
	// end of 2nd wave

	w_info.enemies.clear();
	ef_info.state_infoes.clear();
	ef_info.state_map_infoes.clear();
	ef_info.weapons.clear();

	// 3rd wave
	ef_info.type = (int)ENEMY_TYPE::CARRIER_TIGER;
	ef_info.max_life = 500.0f;
	ef_info.armor = 500.0f;
	ef_info.bounding_circle_radius = 150.0f;
	ef_info.body_texture_name = "n_boss.png";
	ef_info.rotate_angle = 0.0f;
	ef_info.entry_from = Vec2(320.0f, 610.0f);
	scaleByDesign(ef_info.entry_from);
	ef_info.entry_to = Vec2(320.0f, 610.0f);
	scaleByDesign(ef_info.entry_to);
		// 1st state - Entry State
		state.id = 1;
		state.type = (int)STATE_TYPE::ENTRY;
		state.movements.clear();
		state.weapons.clear();
		ef_info.state_infoes.push_back(state);
		//
		// state maps
		state_map.event = (int)STATE_MACHINE_EVENT::START;
		state_map.from = -1;
		state_map.to = 1;
		ef_info.state_map_infoes.push_back(state_map);
		//
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

int AppDelegate::getCommonType(const std::string &str)
{
	return m_mapCommonTypes[str];
}

void AppDelegate::initCommonTypes(void)
{
	ADD_COMMON_TYPE(m_mapCommonTypes, BODY_TYPE, BASIC);

	ADD_COMMON_TYPE(m_mapCommonTypes, ARMOR_TYPE, ENEGY_SHIELD);
	ADD_COMMON_TYPE(m_mapCommonTypes, ARMOR_TYPE, REACTIVE_ARMOR);
	ADD_COMMON_TYPE(m_mapCommonTypes, ARMOR_TYPE, CERAMIC_ARMOR);

	ADD_COMMON_TYPE(m_mapCommonTypes, ENGINE_TYPE, BASIC);

	ADD_COMMON_TYPE(m_mapCommonTypes, WEAPON_TYPE, CANNON);

	ADD_COMMON_TYPE(m_mapCommonTypes, BARREL_TYPE, BULLET);
	ADD_COMMON_TYPE(m_mapCommonTypes, BARREL_TYPE, LASER);
	ADD_COMMON_TYPE(m_mapCommonTypes, BARREL_TYPE, MISSILE);

	ADD_COMMON_TYPE(m_mapCommonTypes, EFFECT_TYPE, TEXTURE);
	ADD_COMMON_TYPE(m_mapCommonTypes, EFFECT_TYPE, SPRITEFRAME);
	ADD_COMMON_TYPE(m_mapCommonTypes, EFFECT_TYPE, ANIMATION);

	ADD_COMMON_TYPE(m_mapCommonTypes, PROJECTILE_TYPE, BULLET_NORMAL);
	ADD_COMMON_TYPE(m_mapCommonTypes, PROJECTILE_TYPE, LASER_NORMAL);
	ADD_COMMON_TYPE(m_mapCommonTypes, PROJECTILE_TYPE, MISSILE_NORMAL);

	ADD_COMMON_TYPE(m_mapCommonTypes, WINGMAN_TYPE, BASIC);

	ADD_COMMON_TYPE(m_mapCommonTypes, ENEMY_TYPE, FIGHTER_BEE);
	ADD_COMMON_TYPE(m_mapCommonTypes, ENEMY_TYPE, FRIGATE_PUMA);
	ADD_COMMON_TYPE(m_mapCommonTypes, ENEMY_TYPE, CARRIER_TIGER);

	ADD_COMMON_TYPE(m_mapCommonTypes, MOVEMENT_TYPE, DISPLACEMENT);
	ADD_COMMON_TYPE(m_mapCommonTypes, MOVEMENT_TYPE, ROTATION);
	ADD_COMMON_TYPE(m_mapCommonTypes, MOVEMENT_TYPE, STAY);

	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_TYPE, MOVE);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_TYPE, ENTRY);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_TYPE, BATTLE_PHASE);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_TYPE, TRANSFORM);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_TYPE, LEAVE);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_TYPE, BATTLE_END);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_TYPE, DEAD);

	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_MACHINE_EVENT, START);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_MACHINE_EVENT, MOVE_FINISHED);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_MACHINE_EVENT, ENTRY_FINISHED);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_MACHINE_EVENT, BATTLE_PHASE_FINISHED);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_MACHINE_EVENT, TRANSFORM_FINISHED);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_MACHINE_EVENT, LEAVE_FINISHED);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_MACHINE_EVENT, HOUND_DEAD);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_MACHINE_EVENT, AI_DEAD);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_MACHINE_EVENT, LEVEL_QUIT);
	ADD_COMMON_TYPE(m_mapCommonTypes, STATE_MACHINE_EVENT, VICTORY);
}