#include "DataCenter.h"
#include "tinyxml2\tinyxml2.h"
#include "sqlite3.h"
#include "BlobData.h"

USING_NS_CC;

std::auto_ptr<DataCenter> DataCenter::s_dataCenter(nullptr);
const PairValue DataCenter::PAIR_ZERO = std::make_pair(0.0f,0.0f);
const TripleValue DataCenter::TRIPLE_ZERO = TripleValue();
const FourValue DataCenter::FOUR_ZERO = FourValue();
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
	groups.push_back("EntityTypes");
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
	m_mapHoundWeaponDSAT.clear();
	m_mapHoundProjectileDSAT.clear();

	std::string str = "";
	float value = 0.0f;
	PairValue pair_value;
	FourValue four_value;

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
				stringToFourValue(str, four_value);
				m_mapHoundWeaponDSAT[weapon_type][level] = four_value;
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
				stringToFourValue(str, four_value);
				m_mapHoundProjectileDSAT[proj_type][level] = four_value;
			}
		}
	}

	return true;
}

bool DataCenter::loadEnemyStaticData(void)
{
	m_mapEnemyMaxLifeArmor.clear();
	m_mapEnemyWeaponDSAT.clear();
	m_mapEnemyProjectileDSAT.clear();

	std::string str = "";
	PairValue pair_value;
	FourValue four_value;

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
				stringToFourValue(str, four_value);
				m_mapEnemyWeaponDSAT[weapon_type][level] = four_value;
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
				stringToFourValue(str, four_value);
				m_mapEnemyProjectileDSAT[proj_type][level] = four_value;
			}
		}
	}

	return true;
}

bool DataCenter::loadHoundInfoFromXml(HoundInfo &info)
{
	std::string str;
	str = FileUtils::getInstance()->fullPathForFilename("hound_info.xml");

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

	// get root element
	e_root = doc.RootElement();
	CC_ASSERT(e_root != nullptr);
	//

	// Section: body
	e_child_1 = e_root->FirstChildElement("Body");
	CC_ASSERT(e_child_1 != nullptr);
	charToString(e_child_1->FirstChildElement("Type")->GetText(), str);
	info.body_type = getCommonType(str);
	e_child_1->FirstChildElement("Level")->QueryUnsignedText(&info.body_level);
	charToString(e_child_1->FirstChildElement("Asset")->GetText(), info.body_asset_name);
	// exacting docking positions
	std::map<int, Vec2> docks;
	int dock_id;
	Vec2 dock_pos;
	e_child_2 = e_child_1->FirstChildElement("DockPoints");
	for (e_child_3 = e_child_2->FirstChildElement(); 
		e_child_3 != nullptr;
		e_child_3 = e_child_3->NextSiblingElement())
	{
		dock_id = e_child_3->FindAttribute("id")->IntValue();
		charToString(e_child_3->GetText(), str);
		stringToVec2(str, dock_pos);
		docks[dock_id] = dock_pos;
	}

	// Section: armor
	e_child_1 = e_root->FirstChildElement("Armor");
	CC_ASSERT(e_child_1 != nullptr);
	charToString(e_child_1->FirstChildElement("Type")->GetText(), str);
	info.armor_type = getCommonType(str);
	e_child_1->FirstChildElement("Level")->QueryUnsignedText(&info.armor_level);
	charToString(e_child_1->FirstChildElement("Asset")->GetText(), info.armor_asset_name);

	// Section: engine
	e_child_1 = e_root->FirstChildElement("Engine");
	CC_ASSERT(e_child_1 != nullptr);
	charToString(e_child_1->FirstChildElement("Type")->GetText(), str);
	info.engine_type = getCommonType(str);
	e_child_1->FirstChildElement("Level")->QueryUnsignedText(&info.engine_level);
	charToString(e_child_1->FirstChildElement("Asset")->GetText(), info.engine_asset_name);

	// get hound max life and armor value
	info.max_life = getHoundMaxLife(info.body_type, info.body_level);
	info.armor = getHoundArmor(info.armor_type, info.armor_level);

	// for each weapon
	e_child_1 = e_root->FirstChildElement("Weapons");
	if (e_child_1 != nullptr)
	{
		for (e_child_2 = e_child_1->FirstChildElement(); 
			e_child_2 != nullptr;
			e_child_2 = e_child_2->NextSiblingElement())
		{
			WeaponInfo weapon;
			weapon.id = e_child_2->FindAttribute("id")->IntValue();
			charToString(e_child_2->FirstChildElement("Type")->GetText(), str);
			weapon.type = getCommonType(str);
			e_child_2->FirstChildElement("Level")->QueryUnsignedText(&weapon.level);
			auto wds = getHoundWeaponDSAT(weapon.type, weapon.level);
			weapon.damage = wds.first;
			weapon.speed = wds.second;
			weapon.acceleration = wds.third;
			weapon.steering_accel = wds.fourth;
			scaleByDesign(weapon.speed);
			scaleByDesign(weapon.acceleration);
			e_child_2->FirstChildElement("RotateAngle")->QueryFloatText(&weapon.rotate_angle);
			e_child_2->FirstChildElement("AutoAim")->QueryBoolText(&weapon.auto_aim);
			e_child_2->FirstChildElement("DockAt")->QueryIntText(&dock_id);
			weapon.dock_position = docks[dock_id];
			e_child_2->FirstChildElement("FiringStart")->QueryFloatText(&weapon.time_offset_firing_start);
			e_child_2->FirstChildElement("FiringStop")->QueryFloatText(&weapon.time_offset_firing_stop);
			charToString(e_child_2->FirstChildElement("Asset")->GetText(), weapon.texture_name);

			// for each barrel
			e_child_3 = e_child_2->FirstChildElement("Barrells");
			if (e_child_3 != nullptr)
			{
				for (e_child_4 = e_child_3->FirstChildElement(); 
					e_child_4 != nullptr;
					e_child_4 = e_child_4->NextSiblingElement())
				{
					BarrelInfo barrel;
					charToString(e_child_4->FindAttribute("type")->Value(), str);
					barrel.type = getCommonType(str);
					e_child_4->FirstChildElement("RotateAngle")->QueryFloatText(&barrel.rotate_angle);
					e_child_4->FirstChildElement("FiringInterval")->QueryFloatText(&barrel.firing_interval);
					charToString(e_child_4->FirstChildElement("ProjectileType")->GetText(), str);
					barrel.projectile_type = getCommonType(str);
					e_child_4->FirstChildElement("ProjectileLevel")->QueryIntText(&barrel.projectile_level);
					e_child_4->FirstChildElement("ProjectileScale")->QueryFloatText(&barrel.projectile_scale_xy);
					scaleByDesign(barrel.projectile_scale_xy);
					auto pds = getHoundProjectileDSAT(barrel.projectile_type, barrel.projectile_level);
					barrel.projectile_damage = pds.first;
					barrel.projectile_speed = pds.second;
					barrel.projectile_acceleration = pds.third;
					barrel.projectile_steering_accel = pds.fourth;
					scaleByDesign(barrel.projectile_speed);
					scaleByDesign(barrel.projectile_acceleration);
					scaleByDesign(barrel.projectile_steering_accel);
					barrel.projectile_final_damage = barrel.projectile_damage + weapon.damage;
					barrel.projectile_final_speed = barrel.projectile_speed + weapon.speed;
					barrel.projectile_final_acceleration = barrel.projectile_acceleration + weapon.acceleration;
					barrel.projectile_final_steering_accel = barrel.projectile_steering_accel + weapon.steering_accel;
					charToString(e_child_4->FirstChildElement("Asset")->GetText(), barrel.projectile_asset_name);

					weapon.barrells.push_back(barrel);
				}
			}

			info.weapons.push_back(weapon);
			weapon.barrells.clear();
		}
	}

	return true;
}

bool DataCenter::generateDatabase(void)
{
#define SAFE_SQLITE_EXEC(db, sql) \
	{ \
		std::string str = sql; \
		int result = sqlite3_exec(db, str.c_str(), nullptr, nullptr, nullptr); \
		if (result != SQLITE_OK) \
		{ \
			CCLOG("sqlite3_exec() failed! %s, %d, %s", str.c_str(), __LINE__, __FILE__); \
			return false; \
		} \
	}

	sqlite3 *pdb = nullptr;
	std::string path = FileUtils::getInstance()->getWritablePath()+"hound.db";

	// create database
	int result = sqlite3_open(path.c_str(), &pdb);
	if (result != SQLITE_OK)
	{
		CCLOG("sqlite3_open() failed! %d, %s", __LINE__, __FILE__);
		return false;
	}

	// create tables
	SAFE_SQLITE_EXEC(pdb, "Begin Transaction");
	SAFE_SQLITE_EXEC(pdb, "Drop Table player");
	SAFE_SQLITE_EXEC(pdb, "Drop Table s_body");
	SAFE_SQLITE_EXEC(pdb, "Drop Table s_armor");
	SAFE_SQLITE_EXEC(pdb, "Drop Table s_engine");
	SAFE_SQLITE_EXEC(pdb, "Drop Table s_weapon");
	SAFE_SQLITE_EXEC(pdb, "CREATE TABLE player (id INTEGER PRIMARY KEY UNIQUE, "
											"name TEXT, "
											"gold INTEGER, "
											"diamond INTEGER, "
											"body INTEGER, "
											"body_level INTEGER, "
											"armor INTEGER, "
											"armor_level INTEGER, "
											"engine INTEGER, "
											"engine_level INTEGER, "
											"weapon BLOB, "
											"inventory BLOB)");
	SAFE_SQLITE_EXEC(pdb, "CREATE TABLE s_body (id INTEGER PRIMARY KEY UNIQUE, "
											"type TEXT, "
											"enum INTEGER UNIQUE, "
											"asset TEXT)");
	SAFE_SQLITE_EXEC(pdb, "CREATE TABLE s_armor (id INTEGER PRIMARY KEY UNIQUE, "
											"type TEXT, "
											"enum INTEGER UNIQUE, "
											"asset TEXT)");
	SAFE_SQLITE_EXEC(pdb, "CREATE TABLE s_engine (id INTEGER PRIMARY KEY UNIQUE, "
											"type TEXT, "
											"enum NUMERIC UNIQUE, "
											"asset TEXT)");
	SAFE_SQLITE_EXEC(pdb, "CREATE TABLE s_weapon (id INTEGER PRIMARY KEY UNIQUE, "
											"type TEXT, "
											"enum INTEGER UNIQUE, "
											"asset TEXT)");

	// make up player info
	PlayerInfo player;
	player.id = 1;
	player.name = "lqtjl";

	// load hound info from hound_info.xml
	loadHoundInfoFromXml(player.hound);

	// insert records
	sqlite3_stmt *stmt = nullptr;
	//sqlite3_prepare("insert...", &stmt);
	//sqlite3_bind_blob(stmt, ...);
	//sqlite3_step(stmt);
	//sqlite3_finalize(stmt);

	// query records
	//sqlite3_prepare("select...", &stmt);
	//sqlite3_step(stmt);
	//sqlite3_column_blob(stmt, index);
	//sqlite3_column_size(stmt, index);
	//sqlite3_finalize(stmt);

	SAFE_SQLITE_EXEC(pdb, "Commit Transaction");

	CCLOG("generateDummyDatabase succeeded!");
	return true;
}

bool DataCenter::loadPlayerInfo(PlayerInfo &info)
{
	// player info dummy config
	info.id = 0;
	info.name = "player";
	info.vip_level = 0;

	return loadHoundInfoFromXml(info.hound);
	/*
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

		// get weapon damage and speed
		for (auto &weapon : info.hound.weapons)
		{
			auto wds = DataCenter::getInstance()->getHoundWeaponDSAT(
				weapon.type, weapon.level);
			weapon.damage = wds.first;
			weapon.speed = wds.second;

			// get projectile damage and speed
			for (auto &barrel : weapon.barrells)
			{
				auto pds = DataCenter::getInstance()->getHoundProjectileDSAT(
					barrel.projectile_type, barrel.projectile_level);
				barrel.projectile_damage = pds.first;
				barrel.projectile_speed = pds.second;
			}
		}
	*/ 
	// end of player info dummy config
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
	e_child_1 = e_root->FirstChildElement("Hound");
	CC_ASSERT(e_child_1 != nullptr);
	e_child_1->FirstChildElement("Scale")->QueryFloatText(&info.hound_scale);
	scaleByDesign(info.hound_scale);
	e_child_1->FirstChildElement("BoundingCircleRadius")->QueryFloatText(&info.hound_bounding_circle_radius);
	scaleByDesign(info.hound_bounding_circle_radius);

	e_child_2 = e_child_1->FirstChildElement("Entry");
	CC_ASSERT(e_child_2 != nullptr);
		str = e_child_2->FirstChildElement("From")->GetText();
		stringToVec2(str, info.hound_entry_from);
		scaleByDesign(info.hound_entry_from);
		str = e_child_2->FirstChildElement("To")->GetText();
		stringToVec2(str, info.hound_entry_to);
		scaleByDesign(info.hound_entry_to);
		e_child_2->FirstChildElement("Speed")->QueryFloatText(&info.hound_entry_speed);
		scaleByDesign(info.hound_entry_speed);
		e_child_2->FirstChildElement("AutoFacing")->QueryBoolText(&info.hound_entry_auto_facing);
	//

	// Section: hound leave
	e_child_2 = e_child_1->FirstChildElement("Leave");
	CC_ASSERT(e_child_2 != nullptr);
		e_child_2->FirstChildElement("Speed")->QueryFloatText(&info.hound_leave_speed);
		scaleByDesign(info.hound_leave_speed);
		e_child_2->FirstChildElement("AutoFacing")->QueryBoolText(&info.hound_leave_auto_facing);
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
					auto wds = getEnemyWeaponDSAT(weapon.type, weapon.level);
					weapon.damage = wds.first;
					weapon.speed = wds.second;
					weapon.acceleration = wds.third;
					weapon.steering_accel = wds.fourth;
					scaleByDesign(weapon.speed);
					scaleByDesign(weapon.acceleration);
					scaleByDesign(weapon.steering_accel);
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
							auto pds = getEnemyProjectileDSAT(barrel.projectile_type, barrel.projectile_level);
							barrel.projectile_damage = pds.first;
							barrel.projectile_speed = pds.second;
							barrel.projectile_acceleration = pds.third;
							barrel.projectile_steering_accel = pds.fourth;
							scaleByDesign(barrel.projectile_speed);
							scaleByDesign(barrel.projectile_acceleration);
							barrel.projectile_final_damage = barrel.projectile_damage + weapon.damage;
							barrel.projectile_final_speed = barrel.projectile_speed + weapon.speed;
							barrel.projectile_final_acceleration = barrel.projectile_acceleration + weapon.acceleration;
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

const FourValue& DataCenter::getHoundWeaponDSAT(int type, int level)
{
	TypeLevelFourMap::iterator it = m_mapHoundWeaponDSAT.find(type);
	if (it == m_mapHoundWeaponDSAT.end())
	{
		CCLOG("Error: getHoundWeaponDSAT => Could not find armor type %d.", type);
		return FOUR_ZERO;
	}

	std::map<int, FourValue>::iterator it2 = it->second.find(level);
	if (it2 == it->second.end())
	{
		CCLOG("Error: getHoundWeaponDSAT => Could not find level %d.", level);
		return FOUR_ZERO;
	}

	return it2->second;
}
const FourValue& DataCenter::getHoundProjectileDSAT(int type, int level)
{
	TypeLevelFourMap::iterator it = m_mapHoundProjectileDSAT.find(type);
	if (it == m_mapHoundProjectileDSAT.end())
	{
		CCLOG("Error: getHoundProjectileDSAT => Could not find armor type %d.", type);
		return FOUR_ZERO;
	}

	std::map<int, FourValue>::iterator it2 = it->second.find(level);
	if (it2 == it->second.end())
	{
		CCLOG("Error: getHoundProjectileDSAT => Could not find level %d.", level);
		return FOUR_ZERO;
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
const FourValue& DataCenter::getEnemyWeaponDSAT(int type, int level)
{
	TypeLevelFourMap::iterator it = m_mapEnemyWeaponDSAT.find(type);
	if (it == m_mapEnemyWeaponDSAT.end())
	{
		CCLOG("Error: getEnemyWeaponDSAT => Could not find Weapon type %d.", type);
		return FOUR_ZERO;
	}

	std::map<int, FourValue>::iterator it2 = it->second.find(level);
	if (it2 == it->second.end())
	{
		CCLOG("Error: getEnemyWeaponDSAT => Could not find level %d.", level);
		return FOUR_ZERO;
	}

	return it2->second;
}
const FourValue& DataCenter::getEnemyProjectileDSAT(int type, int level)
{
	TypeLevelFourMap::iterator it = m_mapEnemyProjectileDSAT.find(type);
	if (it == m_mapEnemyProjectileDSAT.end())
	{
		CCLOG("Error: getEnemyProjectileDSAT => Could not find Projectile type %d.", type);
		return FOUR_ZERO;
	}

	std::map<int, FourValue>::iterator it2 = it->second.find(level);
	if (it2 == it->second.end())
	{
		CCLOG("Error: getEnemyProjectileDSAT => Could not find level %d.", level);
		return FOUR_ZERO;
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

void DataCenter::stringToVec2(const std::string &str, cocos2d::Vec2 &ret)
{
	ret.x = 0.0f;
	ret.y = 0.0f;

	std::size_t pos = str.find(',');
	if (pos == std::string::npos)
	{
		return;
	}

	tinyxml2::XMLUtil::ToFloat(str.substr(0, pos).c_str(), &ret.x);
	tinyxml2::XMLUtil::ToFloat(str.substr(pos+1).c_str(), &ret.y);
}

void DataCenter::stringToPairValue(const std::string &str, PairValue &ret)
{
	std::size_t pos = str.find(',');
	if (pos == std::string::npos)
	{
		ret.first = 0.0f;
		ret.second = 0.0f;
		return;
	}
	tinyxml2::XMLUtil::ToFloat(str.substr(0, pos).c_str(), &ret.first);
	tinyxml2::XMLUtil::ToFloat(str.substr(pos+1).c_str(), &ret.second);
}

void DataCenter::stringToTripleValue(const std::string &str, TripleValue &ret)
{
	do{
		std::size_t pos = str.find(',');
		if (pos == std::string::npos)
		{
			break;
		}
		tinyxml2::XMLUtil::ToFloat(str.substr(0,pos).c_str(), &ret.first);

		std::string str1 = str.substr(pos+1);
		pos = str1.find(',');
		if (pos == std::string::npos)
		{
			break;
		}
		tinyxml2::XMLUtil::ToFloat(str1.substr(0,pos).c_str(), &ret.second);
		tinyxml2::XMLUtil::ToFloat(str1.substr(pos+1).c_str(), &ret.third);

		return;
	}while(0);
	
	ret.first = 0.0f;
	ret.second = 0.0f;
	ret.third = 0.0f;
}

void DataCenter::stringToFourValue(const std::string &str, FourValue &ret)
{
	do{
		std::size_t pos = str.find(',');
		if (pos == std::string::npos)
		{
			break;
		}
		tinyxml2::XMLUtil::ToFloat(str.substr(0,pos).c_str(), &ret.first);

		std::string str1 = str.substr(pos+1);
		pos = str1.find(',');
		if (pos == std::string::npos)
		{
			break;
		}
		tinyxml2::XMLUtil::ToFloat(str1.substr(0,pos).c_str(), &ret.second);

		std::string str2 = str1.substr(pos+1);
		pos = str2.find(',');
		if (pos == std::string::npos)
		{
			break;
		}
		tinyxml2::XMLUtil::ToFloat(str2.substr(0,pos).c_str(), &ret.third);
		tinyxml2::XMLUtil::ToFloat(str2.substr(pos+1).c_str(), &ret.fourth);

		return;
	}while(0);
	
	ret.first = 0.0f;
	ret.second = 0.0f;
	ret.third = 0.0f;
	ret.fourth = 0.0f;
}
