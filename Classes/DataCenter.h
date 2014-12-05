#ifndef __HOUND_DATA_CENTER_H__
#define __HOUND_DATA_CENTER_H__

#include "Common.h"

#define FULL_TYPE_STRING(typeclass, type) std::string(#typeclass)+std::string("::")+std::string(#type)
#define ADD_COMMON_TYPE(container, typeclass, type) \
	container.insert(std::make_pair<std::string, const int>(FULL_TYPE_STRING(typeclass, type), (int)(typeclass##::##type)));

typedef std::map<std::string, int> CommonTypeMap;
typedef std::pair<float, float> TLMValue;
typedef std::map<int/*type*/, std::map<int/*level*/, TLMValue/*value*/>> TypeLevelMap;
typedef std::map<int/*type*/, std::map<int/*type*/, float/*factor*/>> DamageFactorMap;

class DataCenter
{
public:
	static DataCenter* getInstance(void);

	bool loadStaticData(void);

	bool loadLevelInfo(int id, LevelInfo &info);

	int getCommonType(const std::string &str);
	float getDamageFactor(int damage_type, int armor_type);

	const TLMValue& getHoundMaxLifeArmor(int type, int level);
	const TLMValue& getHoundWeaponDamageSpeed(int type, int level);
	const TLMValue& getHoundProjectileDamageSpeed(int type, int level);

	const TLMValue& getEnemyMaxLifeArmor(int type, int level);
	const TLMValue& getEnemyWeaponDamageSpeed(int type, int level);
	const TLMValue& getEnemyProjectileDamageSpeed(int type, int level);

private:
	DataCenter(void);
	DataCenter(const DataCenter&){}
	~DataCenter(void);
	DataCenter& operator = (const DataCenter&){}

	bool loadCommonTypeMap(void);
	bool loadDamageFactorMap(void);

	bool loadHoundMaxLifeArmorMap(void);
	bool loadHoundWeaponDamageSpeedMap(void);
	bool loadHoundProjectileDamageSpeedMap(void);

	bool loadEnemyMaxLifeArmorMap(void);
	bool loadEnemyWeaponDamageSpeedMap(void);
	bool loadEnemyProjectileDamageSpeedMap(void);

	// utilties
	void stringToVec2(const std::string &str, cocos2d::Vec2 &vec);
	void charToString(const char *pstr, std::string &ret);

	CommonTypeMap m_mapCommonTypes;
	DamageFactorMap m_mapDamageFactor;

	TypeLevelMap m_mapHoundMaxLifeArmor;
	TypeLevelMap m_mapHoundWeaponDamageSpeed;
	TypeLevelMap m_mapHoundProjectileDamageSpeed;

	TypeLevelMap m_mapEnemyMaxLifeArmor;
	TypeLevelMap m_mapEnemyWeaponDamageSpeed;
	TypeLevelMap m_mapEnemyProjectileDamageSpeed;

	friend class std::auto_ptr<DataCenter>;
	static std::auto_ptr<DataCenter> s_dataCenter;
};

#endif //__HOUND_DATA_CENTER_H__
