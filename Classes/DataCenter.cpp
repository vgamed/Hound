#include "DataCenter.h"
#include "tinyxml2\tinyxml2.h"
#include "sqlite3.h"

USING_NS_CC;

std::auto_ptr<DataCenter> DataCenter::s_dataCenter(nullptr);
const PairValue DataCenter::PAIR_ZERO = std::make_pair(0.0f,0.0f);
const Size DataCenter::RESOURCE_RESOLUTION = Size(720.0f, 1280.0f);
const Size DataCenter::DESIGN_RESOLUTION = Size(640.0f, 960.0f);

DataCenter::DataCenter(void)
{
	FileUtils::getInstance()->addSearchPath("data");
	FileUtils::getInstance()->addSearchPath("data/levels");
}

DataCenter::~DataCenter(void)
{
}

DataCenter* DataCenter::getInstance(void)
{
	if (s_dataCenter.get() == nullptr)
	{
		s_dataCenter.reset(new DataCenter());
	}
	return s_dataCenter.get();
}

bool DataCenter::loadStaticData(void)
{
	if (!loadCommonTypeMap())
		return false;

	if (!loadDamageFactorMap())
		return false;

	if (!loadHoundStaticData())
		return false;

	if (!loadEnemyStaticData())
		return false;

	return true;
}

bool DataCenter::loadCommonTypeMap(void)
{
	m_mapCommonTypes.clear();

	std::string str = "";
	int value = 0;

	str = FileUtils::getInstance()->fullPathForFilename("common_types.xml");
	tinyxml2::XMLDocument doc;
	auto data = FileUtils::getInstance()->getDataFromFile(str);
	auto ret = doc.Parse((const char*)data.getBytes(), data.getSize());
	//auto ret = doc.LoadFile(str.c_str());
	if (ret != tinyxml2::XML_NO_ERROR)
	{
		return false;
	}

	std::vector<std::string> groups;
	groups.push_back("HoundTypes");
	groups.push_back("WingmanTypes");
	groups.push_back("EnemyTypes");
	groups.push_back("WeaponTypes");
	groups.push_back("BarrelTypes");
	groups.push_back("ProjectileTypes");
	groups.push_back("MovementTypes");
	groups.push_back("StateTypes");
	groups.push_back("StateMachineEvents");

	const tinyxml2::XMLElement* e_root = nullptr;
	const tinyxml2::XMLElement* e_child_1 = nullptr;
	const tinyxml2::XMLElement* e_child_2 = nullptr;

	e_root = doc.RootElement();
	CC_ASSERT(e_root != nullptr);

	for (std::size_t i = 0; i < groups.size(); i++)
	{
		e_child_1 = e_root->FirstChildElement(groups[i].c_str());
		if (e_child_1 == nullptr)
			continue;

		for (e_child_2 = e_child_1->FirstChildElement();
			e_child_2 != nullptr;
			e_child_2 = e_child_2->NextSiblingElement())
		{
			str = e_child_2->Value();
			e_child_2->QueryIntText(&value);
			m_mapCommonTypes.insert(std::make_pair(str, value));
		}
	}

	return true;
}

bool DataCenter::loadDamageFactorMap(void)
{
	m_mapDamageFactor.clear();

	std::string str = FileUtils::getInstance()->fullPathForFilename("damage_factor.xml");
	tinyxml2::XMLDocument doc;
	auto data = FileUtils::getInstance()->getDataFromFile(str);
	auto ret = doc.Parse((const char*)data.getBytes(), data.getSize());
	//auto ret = doc.LoadFile(str.c_str());
	if (ret != tinyxml2::XML_NO_ERROR)
	{
		return false;
	}

	const tinyxml2::XMLElement* e_root = nullptr;
	const tinyxml2::XMLElement* e_child_1 = nullptr;
	const tinyxml2::XMLElement* e_child_2 = nullptr;
	float factor = 0.0f;

	e_root = doc.RootElement();
	CC_ASSERT(e_root != nullptr);

	for (e_child_1 = e_root->FirstChildElement();
		e_child_1 != nullptr;
		e_child_1 = e_child_1->NextSiblingElement())
	{
		int armor_type = getCommonType(e_child_1->Value());

		for (e_child_2 = e_child_1->FirstChildElement();
			e_child_2 != nullptr;
			e_child_2 = e_child_2->NextSiblingElement())
		{
			int proj_type = getCommonType(e_child_2->Value());
			e_child_2->QueryFloatText(&factor);
			m_mapDamageFactor[armor_type][proj_type] = factor;
		}
	}

	return true;
}

bool DataCenter::loadHoundStaticData(void)
{
	m_mapHoundMaxLife.clear();
	m_mapHoundArmor.clear();
	m_mapHoundWeaponDamageSpeed.clear();
	m_mapHoundProjectileDamageSpeed.clear();

	std::string str = "";
	float value = 0.0f;
	PairValue pair_value;

	str = FileUtils::getInstance()->fullPathForFilename("hound_static_data.xml");
	tinyxml2::XMLDocument doc;
	auto data = FileUtils::getInstance()->getDataFromFile(str);
	auto ret = doc.Parse((const char*)data.getBytes(), data.getSize());
	//auto ret = doc.LoadFile(str.c_str());
	if (ret != tinyxml2::XML_NO_ERROR)
	{
		return false;
	}

	const tinyxml2::XMLElement* e_root = nullptr;
	const tinyxml2::XMLElement* e_child_1 = nullptr;
	const tinyxml2::XMLElement* e_child_2 = nullptr;
	const tinyxml2::XMLElement* e_child_3 = nullptr;

	e_root = doc.RootElement();
	CC_ASSERT(e_root != nullptr);

	//load Hound Max Life Map
	e_child_1 = e_root->FirstChildElement("LifeValue");
	if (e_child_1 != nullptr)
	{
		for (e_child_2 = e_child_1->FirstChildElement();
			e_child_2 != nullptr;
			e_child_2 = e_child_2->NextSiblingElement())
		{
			int body_type = getCommonType(e_child_2->FindAttribute("type")->Value());
			for (e_child_3 = e_child_2->FirstChildElement();
				e_child_3 != nullptr;
				e_child_3 = e_child_3->NextSiblingElement())
			{
				int level = e_child_3->FindAttribute("value")->IntValue();
				e_child_3->QueryFloatText(&value);
				m_mapHoundMaxLife[body_type][level] = value;
			}
		}
	}

	//load Hound Armor Map
	e_child_1 = e_root->FirstChildElement("ArmorValue");
	if (e_child_1 != nullptr)
	{
		for (e_child_2 = e_child_1->FirstChildElement();
			e_child_2 != nullptr;
			e_child_2 = e_child_2->NextSiblingElement())
		{
			int armor_type = getCommonType(e_child_2->FindAttribute("type")->Value());
			for (e_child_3 = e_child_2->FirstChildElement();
				e_child_3 != nullptr;
				e_child_3 = e_child_3->NextSiblingElement())
			{
				int level = e_child_3->FindAttribute("value")->IntValue();
				e_child_3->QueryFloatText(&value);
				m_mapHoundArmor[armor_type][level] = value;
			}
		}
	}

	//load Hound Weapon Damage/Speed Map
	e_child_1 = e_root->FirstChildElement("WeaponData");
	if (e_child_1 != nullptr)
	{
		for (e_child_2 = e_child_1->FirstChildElement();
			e_child_2 != nullptr;
			e_child_2 = e_child_2->NextSiblingElement())
		{
			int weapon_type = getCommonType(e_child_2->FindAttribute("type")->Value());
			for (e_child_3 = e_child_2->FirstChildElement();
				e_child_3 != nullptr;
				e_child_3 = e_child_3->NextSiblingElement())
			{
				int level = e_child_3->FindAttribute("value")->IntValue();
				str = e_child_3->GetText();
				stringToPairValue(str, pair_value);
				m_mapHoundWeaponDamageSpeed[weapon_type][level] = pair_value;
			}
		}
	}

	//load Hound Projectile Damage/Speed Map
	e_child_1 = e_root->FirstChildElement("ProjectileData");
	if (e_child_1 != nullptr)
	{
		for (e_child_2 = e_child_1->FirstChildElement();
			e_child_2 != nullptr;
			e_child_2 = e_child_2->NextSiblingElement())
		{
			int proj_type = getCommonType(e_child_2->FindAttribute("type")->Value());
			for (e_child_3 = e_child_2->FirstChildElement();
				e_child_3 != nullptr;
				e_child_3 = e_child_3->NextSiblingElement())
			{
				int level = e_child_3->FindAttribute("value")->IntValue();
				str = e_child_3->GetText();
				stringToPairValue(str, pair_value);
				m_mapHoundProjectileDamageSpeed[proj_type][level] = pair_value;
			}
		}
	}

	return true;
}

bool DataCenter::loadEnemyStaticData(void)
{
	m_mapEnemyMaxLifeArmor.clear();
	m_mapEnemyWeaponDamageSpeed.clear();
	m_mapEnemyProjectileDamageSpeed.clear();

	std::string str = "";
	PairValue pair_value;

	str = FileUtils::getInstance()->fullPathForFilename("enemy_static_data.xml");
	tinyxml2::XMLDocument doc;
	auto data = FileUtils::getInstance()->getDataFromFile(str);
	auto ret = doc.Parse((const char*)data.getBytes(), data.getSize());
	//auto ret = doc.LoadFile(str.c_str());
	if (ret != tinyxml2::XML_NO_ERROR)
	{
		return false;
	}

	const tinyxml2::XMLElement* e_root = nullptr;
	const tinyxml2::XMLElement* e_child_1 = nullptr;
	const tinyxml2::XMLElement* e_child_2 = nullptr;
	const tinyxml2::XMLElement* e_child_3 = nullptr;

	e_root = doc.RootElement();
	CC_ASSERT(e_root != nullptr);

	//load Enemy MaxLife/Armor Map
	e_child_1 = e_root->FirstChildElement("LifeArmor");
	if (e_child_1 != nullptr)
	{
		for (e_child_2 = e_child_1->FirstChildElement();
			e_child_2 != nullptr;
			e_child_2 = e_child_2->NextSiblingElement())
		{
			int enemy_type = getCommonType(e_child_2->FindAttribute("type")->Value());
			for (e_child_3 = e_child_2->FirstChildElement();
				e_child_3 != nullptr;
				e_child_3 = e_child_3->NextSiblingElement())
			{
				int level = e_child_3->FindAttribute("value")->IntValue();
				str = e_child_3->GetText();
				stringToPairValue(str, pair_value);
				m_mapEnemyMaxLifeArmor[enemy_type][level] = pair_value;
			}
		}
	}
	//load Enemy Weapon Damage/Speed Map
	e_child_1 = e_root->FirstChildElement("WeaponData");
	if (e_child_1 != nullptr)
	{
		for (e_child_2 = e_child_1->FirstChildElement();
			e_child_2 != nullptr;
			e_child_2 = e_child_2->NextSiblingElement())
		{
			int weapon_type = getCommonType(e_child_2->FindAttribute("type")->Value());
			for (e_child_3 = e_child_2->FirstChildElement();
				e_child_3 != nullptr;
				e_child_3 = e_child_3->NextSiblingElement())
			{
				int level = e_child_3->FindAttribute("value")->IntValue();
				str = e_child_3->GetText();
				stringToPairValue(str, pair_value);
				m_mapEnemyWeaponDamageSpeed[weapon_type][level] = pair_value;
			}
		}
	}
	//load Enemy Projectile Damage/Speed Map
	e_child_1 = e_root->FirstChildElement("ProjectileData");
	if (e_child_1 != nullptr)
	{
		for (e_child_2 = e_child_1->FirstChildElement();
			e_child_2 != nullptr;
			e_child_2 = e_child_2->NextSiblingElement())
		{
			int proj_type = getCommonType(e_child_2->FindAttribute("type")->Value());
			for (e_child_3 = e_child_2->FirstChildElement();
				e_child_3 != nullptr;
				e_child_3 = e_child_3->NextSiblingElement())
			{
				int level = e_child_3->FindAttribute("value")->IntValue();
				str = e_child_3->GetText();
				stringToPairValue(str, pair_value);
				m_mapEnemyProjectileDamageSpeed[proj_type][level] = pair_value;
			}
		}
	}

	return true;
}

bool DataCenter::loadPlayerInfo(PlayerInfo &info)
{
	// player info dummy config
	{
		info.id = 0;
		info.name = "player";
		info.vip_level = 0;

		// hound
		info.hound.body_level = 1;
		info.hound.body_type = (int)BODY_TYPE::BASIC;
		info.hound.body_texture_name = "mplane.png";
		info.hound.armor_level = 1;
		info.hound.armor_type = (int)ARMOR_TYPE::ENEGY_SHIELD;
		info.hound.armor_texture_name = "";
		info.hound.engine_level = 1;
		info.hound.engine_type = (int)ENGINE_TYPE::BASIC;
		info.hound.engine_texture_name = "";
		info.hound.scale_xy = 1.0f;
		scaleByDesign(info.hound.scale_xy);
		info.hound.bounding_circle_radius = 10.0f;
		scaleByDesign(info.hound.bounding_circle_radius);

		// barrel config
		BarrelInfo barrel;
		barrel.type = (int)BARREL_TYPE::BULLET;
		barrel.projectile_type = (int)PROJECTILE_TYPE::BULLET_NORMAL;
		barrel.projectile_level = 1;
		barrel.projectile_scale_xy = 0.6f;
		scaleByDesign(barrel.projectile_scale_xy);

		//barrel.projectile_effect_name = "bullet_1.png";
		barrel.projectile_asset_name = "bullet18.png";
		barrel.firing_interval = 0.1f; //second

		// weapon config
		WeaponInfo weapon;
		weapon.level  = 1;
		weapon.type = (int)WEAPON_TYPE::CANNON;
		weapon.auto_aim = false;
		weapon.time_offset_firing_start = 2.0f; //second
		weapon.time_offset_firing_stop = FLT_MAX; //second

		// add weapons
		info.hound.weapons.clear();

		// front gun
		weapon.id = 1;
		weapon.texture_name = "frontgun.png";
		weapon.dock_position = Vec2(58.0f, 81.0f);
		weapon.rotate_angle = 0.0f;
		weapon.barrells.clear();
		barrel.rotate_angle = 0.0f;
		weapon.barrells.push_back(barrel);
		info.hound.weapons.push_back(weapon);

		// left gun
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
		info.hound.weapons.push_back(weapon);

		// right gun
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
		info.hound.weapons.push_back(weapon);
	} 
	// end of player info dummy config

	// get hound max life and armor value
	info.hound.max_life = getHoundMaxLife(info.hound.body_type, info.hound.body_level);
	info.hound.armor = getHoundArmor(info.hound.armor_type, info.hound.armor_level);

	// get weapon damage and speed
	for (auto &weapon : info.hound.weapons)
	{
		auto wds = DataCenter::getInstance()->getHoundWeaponDamageSpeed(
			weapon.type, weapon.level);
		weapon.damage = wds.first;
		weapon.speed = wds.second;

		// get projectile damage and speed
		for (auto &barrel : weapon.barrells)
		{
			auto pds = DataCenter::getInstance()->getHoundProjectileDamageSpeed(
				barrel.projectile_type, barrel.projectile_level);
			barrel.projectile_damage = pds.first;
			barrel.projectile_speed = pds.second;
		}
	}

	return true;
}

bool DataCenter::loadLevelInfo(int id, LevelInfo &info)
{
	std::stringstream buffer;
	std::string str;

	buffer << "level_info_" << id << ".xml";
	str = FileUtils::getInstance()->fullPathForFilename(buffer.str());

	tinyxml2::XMLDocument doc;
	auto data = FileUtils::getInstance()->getDataFromFile(str);
	auto ret = doc.Parse((const char*)data.getBytes(), data.getSize());
	//auto ret = doc.LoadFile(str.c_str());
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
	e_root = doc.RootElement();
	CC_ASSERT(e_root != nullptr);
	info.id = e_root->FindAttribute("id")->IntValue();
	//

	// Section: hound entry
	e_child_1 = e_root->FirstChildElement("HoundEntry");
	CC_ASSERT(e_child_1 != nullptr);
	{
		std::string str;
		str = e_child_1->FirstChildElement("From")->GetText();
		stringToVec2(str, info.hound_entry_from);
		scaleByDesign(info.hound_entry_from);
		str = e_child_1->FirstChildElement("To")->GetText();
		stringToVec2(str, info.hound_entry_to);
		scaleByDesign(info.hound_entry_to);
		e_child_1->FirstChildElement("Speed")->QueryFloatText(&info.hound_entry_speed);
		scaleByDesign(info.hound_entry_speed);
		e_child_1->FirstChildElement("AutoFacing")->QueryBoolText(&info.hound_entry_auto_facing);
	}
	//

	// Section: hound leave
	e_child_1 = e_root->FirstChildElement("HoundLeave");
	CC_ASSERT(e_child_1 != nullptr);
		e_child_1->FirstChildElement("Speed")->QueryFloatText(&info.hound_leave_speed);
		scaleByDesign(info.hound_leave_speed);
		e_child_1->FirstChildElement("AutoFacing")->QueryBoolText(&info.hound_leave_auto_facing);
	//

	// Section: backgrounds
	e_child_1 = e_root->FirstChildElement("Backgrounds");
	CC_ASSERT(e_child_1 != nullptr);
	for (e_child_2 = e_child_1->FirstChildElement(); 
		e_child_2 != nullptr;
		e_child_2 = e_child_2->NextSiblingElement())
	{
		info.sbg_layer_texture_names.push_back(e_child_2->GetText());
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
			auto mla = getEnemyMaxLifeArmor(ef_info.type, ef_info.level);
			ef_info.max_life = mla.first;
			ef_info.armor = mla.second;
			e_child_3->FirstChildElement("Scale")->QueryFloatText(&ef_info.scale_xy);
			scaleByDesign(ef_info.scale_xy);
			e_child_3->FirstChildElement("BoundingCircleRadius")->QueryFloatText(&ef_info.bounding_circle_radius);
			scaleByDesign(ef_info.bounding_circle_radius);
			e_child_3->FirstChildElement("RotateAngle")->QueryFloatText(&ef_info.rotate_angle);
			charToString(e_child_3->FirstChildElement("Asset")->GetText(), ef_info.body_texture_name);
			// entry
			e_child_4 = e_child_3->FirstChildElement("Entry");
			charToString(e_child_4->FirstChildElement("From")->GetText(), str);
			stringToVec2(str, ef_info.entry_from);
			scaleByDesign(ef_info.entry_from);
			charToString(e_child_4->FirstChildElement("To")->GetText(), str);
			stringToVec2(str, ef_info.entry_to);
			scaleByDesign(ef_info.entry_to);
			e_child_4->FirstChildElement("Speed")->QueryFloatText(&ef_info.entry_speed);
			scaleByDesign(ef_info.entry_speed);
			e_child_4->FirstChildElement("AutoFacing")->QueryBoolText(&ef_info.entry_auto_facing);
			// leave
			e_child_4 = e_child_3->FirstChildElement("Leave");
			e_child_4->FirstChildElement("Speed")->QueryFloatText(&ef_info.leave_speed);
			scaleByDesign(ef_info.leave_speed);
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
					auto wds = getEnemyWeaponDamageSpeed(weapon.type, weapon.level);
					weapon.damage = wds.first;
					weapon.speed = wds.second;
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
							e_child_7->FirstChildElement("ProjectileLevel")->QueryIntText(&barrel.projectile_level);
							e_child_7->FirstChildElement("ProjectileScale")->QueryFloatText(&barrel.projectile_scale_xy);
							scaleByDesign(barrel.projectile_scale_xy);
							auto pds = getEnemyProjectileDamageSpeed(barrel.projectile_type, barrel.projectile_level);
							barrel.projectile_damage = pds.first;
							barrel.projectile_speed = pds.second;
							scaleByDesign(barrel.projectile_speed);
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
							scaleByDesign(movement.target_position);

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
								if (movement.type == (int)MOVEMENT_TYPE::DISPLACEMENT)
									scaleByDesign(movement.speed);
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
		info.enemy_waves.push_back(w_info);
		w_info.enemies.clear();
	}
	//

	return true;
}

int DataCenter::getCommonType(const std::string &str)
{
	CommonTypeMap::iterator it = m_mapCommonTypes.find(str);
	if (it == m_mapCommonTypes.end())
	{
		CCLOG("Error: getCommonType => Could not find type %s.", str.c_str());
		return 0; //invalid type
	}
	return it->second;
}

float DataCenter::getDamageFactor(int armor_type, int proj_type)
{
	TypeTypeValueMap::iterator it = m_mapDamageFactor.find(armor_type);
	if (it == m_mapDamageFactor.end())
	{
		CCLOG("Error: getDamageFactor => Could not find armor type %d.", armor_type);
		return 1.0f;
	}

	std::map<int, float>::iterator it2 = it->second.find(proj_type);
	if (it2 == it->second.end())
	{
		CCLOG("Error: getDamageFactor => Could not find projectile type %d.", proj_type);
		return 1.0f;
	}

	return it2->second;
}

float DataCenter::getHoundMaxLife(int body_type, int level)
{
	TypeLevelValueMap::iterator it = m_mapHoundMaxLife.find(body_type);
	if (it == m_mapHoundMaxLife.end())
	{
		CCLOG("Error: getHoundMaxLife => Could not find body type %d.", body_type);
		return 1.0f;
	}

	std::map<int, float>::iterator it2 = it->second.find(level);
	if (it2 == it->second.end())
	{
		CCLOG("Error: getHoundMaxLife => Could not find level %d.", level);
		return 1.0f;
	}

	return it2->second;
}

float DataCenter::getHoundArmor(int armor_type, int level)
{
	TypeLevelValueMap::iterator it = m_mapHoundArmor.find(armor_type);
	if (it == m_mapHoundArmor.end())
	{
		CCLOG("Error: getHoundArmor => Could not find armor type %d.", armor_type);
		return 1.0f;
	}

	std::map<int, float>::iterator it2 = it->second.find(level);
	if (it2 == it->second.end())
	{
		CCLOG("Error: getHoundArmor => Could not find level %d.", level);
		return 1.0f;
	}

	return it2->second;
}

const PairValue& DataCenter::getHoundWeaponDamageSpeed(int type, int level)
{
	TypeLevelPairMap::iterator it = m_mapHoundWeaponDamageSpeed.find(type);
	if (it == m_mapHoundWeaponDamageSpeed.end())
	{
		CCLOG("Error: getHoundWeaponDamageSpeed => Could not find armor type %d.", type);
		return PAIR_ZERO;
	}

	std::map<int, PairValue>::iterator it2 = it->second.find(level);
	if (it2 == it->second.end())
	{
		CCLOG("Error: getHoundWeaponDamageSpeed => Could not find level %d.", level);
		return PAIR_ZERO;
	}

	return it2->second;
}
const PairValue& DataCenter::getHoundProjectileDamageSpeed(int type, int level)
{
	TypeLevelPairMap::iterator it = m_mapHoundProjectileDamageSpeed.find(type);
	if (it == m_mapHoundProjectileDamageSpeed.end())
	{
		CCLOG("Error: getHoundProjectileDamageSpeed => Could not find armor type %d.", type);
		return PAIR_ZERO;
	}

	std::map<int, PairValue>::iterator it2 = it->second.find(level);
	if (it2 == it->second.end())
	{
		CCLOG("Error: getHoundProjectileDamageSpeed => Could not find level %d.", level);
		return PAIR_ZERO;
	}

	return it2->second;
}

const PairValue& DataCenter::getEnemyMaxLifeArmor(int type, int level)
{
	TypeLevelPairMap::iterator it = m_mapEnemyMaxLifeArmor.find(type);
	if (it == m_mapEnemyMaxLifeArmor.end())
	{
		CCLOG("Error: getEnemyMaxLifeArmor => Could not find Enemy type %d.", type);
		return PAIR_ZERO;
	}

	std::map<int, PairValue>::iterator it2 = it->second.find(level);
	if (it2 == it->second.end())
	{
		CCLOG("Error: getEnemyMaxLifeArmor => Could not find level %d.", level);
		return PAIR_ZERO;
	}

	return it2->second;
}
const PairValue& DataCenter::getEnemyWeaponDamageSpeed(int type, int level)
{
	TypeLevelPairMap::iterator it = m_mapEnemyWeaponDamageSpeed.find(type);
	if (it == m_mapEnemyWeaponDamageSpeed.end())
	{
		CCLOG("Error: getEnemyWeaponDamageSpeed => Could not find Weapon type %d.", type);
		return PAIR_ZERO;
	}

	std::map<int, PairValue>::iterator it2 = it->second.find(level);
	if (it2 == it->second.end())
	{
		CCLOG("Error: getEnemyWeaponDamageSpeed => Could not find level %d.", level);
		return PAIR_ZERO;
	}

	return it2->second;
}
const PairValue& DataCenter::getEnemyProjectileDamageSpeed(int type, int level)
{
	TypeLevelPairMap::iterator it = m_mapEnemyProjectileDamageSpeed.find(type);
	if (it == m_mapEnemyProjectileDamageSpeed.end())
	{
		CCLOG("Error: getEnemyProjectileDamageSpeed => Could not find Projectile type %d.", type);
		return PAIR_ZERO;
	}

	std::map<int, PairValue>::iterator it2 = it->second.find(level);
	if (it2 == it->second.end())
	{
		CCLOG("Error: getEnemyProjectileDamageSpeed => Could not find level %d.", level);
		return PAIR_ZERO;
	}

	return it2->second;
}

void DataCenter::scaleByDesign(Vec2 &vec2)
{
	static float scale_x = Director::getInstance()->getVisibleSize().width / DESIGN_RESOLUTION.width;
	static float scale_y = Director::getInstance()->getVisibleSize().height / DESIGN_RESOLUTION.height;

	CCLOG("BEFORE SCALE: x=%f, y=%f", vec2.x, vec2.y);

	vec2.x *= scale_x;
	vec2.y *= scale_y;

	CCLOG("AFTER SCALE: x=%f, y=%f", vec2.x, vec2.y);
}

void DataCenter::scaleByDesign(float &flt)
{
	static float scale = MIN(Director::getInstance()->getVisibleSize().width / DESIGN_RESOLUTION.width,
							Director::getInstance()->getVisibleSize().height / DESIGN_RESOLUTION.height);

	CCLOG("BEFORE SCALE: floatValue = %f", flt);

	flt *= scale;

	CCLOG("AFTER SCALE: floatValue = %f", flt);
}

void DataCenter::stringToVec2(const std::string &str, cocos2d::Vec2 &vec)
{
	vec.x = 0.0f;
	vec.y = 0.0f;

	std::size_t pos = str.find(',');
	if (pos == std::string::npos)
	{
		return;
	}

	tinyxml2::XMLUtil::ToFloat(str.substr(0, pos).c_str(), &vec.x);
	tinyxml2::XMLUtil::ToFloat(str.substr(pos+1).c_str(), &vec.y);
}

void DataCenter::stringToPairValue(const std::string &str, PairValue &value)
{
	value.first = 0.0f;
	value.second = 0.0f;

	std::size_t pos = str.find(',');
	if (pos == std::string::npos)
	{
		return;
	}

	tinyxml2::XMLUtil::ToFloat(str.substr(0, pos).c_str(), &value.first);
	tinyxml2::XMLUtil::ToFloat(str.substr(pos+1).c_str(), &value.second);
}
