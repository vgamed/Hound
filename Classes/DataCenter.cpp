#include "DataCenter.h"
#include "tinyxml2\tinyxml2.h"

USING_NS_CC;

std::auto_ptr<DataCenter> DataCenter::s_dataCenter(nullptr);

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

	if (!loadHoundMaxLifeArmorMap())
		return false;
	if (!loadHoundWeaponDamageSpeedMap())
		return false;
	if (!loadHoundProjectileDamageSpeedMap())
		return false;

	if (!loadEnemyMaxLifeArmorMap())
		return false;
	if (!loadEnemyWeaponDamageSpeedMap())
		return false;
	if (!loadEnemyProjectileDamageSpeedMap())
		return false;

	return true;
}

bool DataCenter::loadCommonTypeMap(void)
{
	return true;
}
bool DataCenter::loadDamageFactorMap(void)
{
	return true;
}

bool DataCenter::loadHoundMaxLifeArmorMap(void)
{
	return true;
}
bool DataCenter::loadHoundWeaponDamageSpeedMap(void)
{
	return true;
}
bool DataCenter::loadHoundProjectileDamageSpeedMap(void)
{
	return true;
}

bool DataCenter::loadEnemyMaxLifeArmorMap(void)
{
	return true;
}
bool DataCenter::loadEnemyWeaponDamageSpeedMap(void)
{
	return true;
}
bool DataCenter::loadEnemyProjectileDamageSpeedMap(void)
{
	return true;
}

bool DataCenter::loadLevelInfo(int id, LevelInfo &info)
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
	info.id = e_root->FindAttribute("id")->IntValue();
	//

	// Section: hound entry
	e_child_1 = e_root->FirstChildElement("HoundEntry");
	CC_ASSERT(e_child_1 != nullptr);
	{
		std::string str;
		str = e_child_1->FirstChildElement("From")->GetText();
		stringToVec2(str, info.hound_entry_from);
		str = e_child_1->FirstChildElement("To")->GetText();
		stringToVec2(str, info.hound_entry_to);
		e_child_1->FirstChildElement("Speed")->QueryFloatText(&info.hound_entry_speed);
		e_child_1->FirstChildElement("AutoFacing")->QueryBoolText(&info.hound_entry_auto_facing);
	}
	//

	// Section: hound leave
	e_child_1 = e_root->FirstChildElement("HoundLeave");
	CC_ASSERT(e_child_1 != nullptr);
		e_child_1->FirstChildElement("Speed")->QueryFloatText(&info.hound_leave_speed);
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
			auto value = getEnemyMaxLifeArmor(ef_info.type, ef_info.level);
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
		info.enemy_waves.push_back(w_info);
		w_info.enemies.clear();
	}
	//

	return true;
}

int DataCenter::getCommonType(const std::string &str)
{
	return m_mapCommonTypes[str];
}

float DataCenter::getDamageFactor(int damage_type, int armor_type)
{
	return m_mapDamageFactor[damage_type][armor_type];
}

const TLMValue& DataCenter::getHoundMaxLifeArmor(int type, int level)
{
	return m_mapHoundMaxLifeArmor[type][level];
}
const TLMValue& DataCenter::getHoundWeaponDamageSpeed(int type, int level)
{
	return m_mapHoundWeaponDamageSpeed[type][level];
}
const TLMValue& DataCenter::getHoundProjectileDamageSpeed(int type, int level)
{
	return m_mapHoundProjectileDamageSpeed[type][level];
}

const TLMValue& DataCenter::getEnemyMaxLifeArmor(int type, int level)
{
	return m_mapEnemyMaxLifeArmor[type][level];
}
const TLMValue& DataCenter::getEnemyWeaponDamageSpeed(int type, int level)
{
	return m_mapEnemyWeaponDamageSpeed[type][level];
}
const TLMValue& DataCenter::getEnemyProjectileDamageSpeed(int type, int level)
{
	return m_mapEnemyProjectileDamageSpeed[type][level];
}

void DataCenter::stringToVec2(const std::string &str, cocos2d::Vec2 &vec)
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

void DataCenter::charToString(const char *pstr, std::string &ret)
{
	ret = pstr==nullptr ? "" : pstr;
}
