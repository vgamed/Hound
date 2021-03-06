#ifndef __HOUND_DATA_CENTER_H__
#define __HOUND_DATA_CENTER_H__

#include "Common.h"

#define FULL_TYPE_STRING(typeclass, type) std::string(#typeclass)+std::string("::")+std::string(#type)
#define ADD_COMMON_TYPE(container, typeclass, type) \
	container.insert(std::make_pair<std::string, const int>(FULL_TYPE_STRING(typeclass, type), (int)(typeclass##::##type)));

typedef struct _TripleValue
{
	_TripleValue(void)
		: first(0.0f)
		, second(0.0f)
		, third(0.0f)
	{}

	float first;
	float second;
	float third;
} TripleValue;

typedef struct _FourValue
{
	_FourValue(void)
		: first(0.0f)
		, second(0.0f)
		, third(0.0f)
		, fourth(0.0f)
	{}

	float first;
	float second;
	float third;
	float fourth;
} FourValue;

typedef std::map<std::string, int> CommonTypeMap;
typedef std::pair<float, float> PairValue;

typedef std::map<int/*type*/, std::map<int/*level*/, PairValue/*value*/>> TypeLevelPairMap;
typedef std::map<int/*type*/, std::map<int/*level*/, TripleValue/*value*/>> TypeLevelTripleMap;
typedef std::map<int/*type*/, std::map<int/*level*/, FourValue/*value*/>> TypeLevelFourMap;
typedef std::map<int/*type*/, std::map<int/*level*/, float/*value*/>> TypeLevelValueMap;
typedef std::map<int/*type*/, std::map<int/*type*/, float/*value*/>> TypeTypeValueMap;

class DataCenter
{
public:
	static DataCenter* getInstance(void);

	bool loadStaticData(void);

	bool loadPlayerInfo(PlayerInfo &info);
	bool loadLevelInfo(int id, LevelInfo &info);

	int getCommonType(const std::string &str);
	float getDamageFactor(int armor_type, int proj_type);

	float getHoundMaxLife(int body_type, int level);
	float getHoundArmor(int armor_type, int level);
	const FourValue& getHoundWeaponDSAT(int type, int level); // DSAT: Damage, Speed, Acceleration, sTeering speed
	const FourValue& getHoundProjectileDSAT(int type, int level);

	const PairValue& getEnemyMaxLifeArmor(int type, int level);
	const FourValue& getEnemyWeaponDSAT(int type, int level);
	const FourValue& getEnemyProjectileDSAT(int type, int level);

	bool generateDatabase(void);

	static const cocos2d::Size RESOURCE_RESOLUTION;
	static const cocos2d::Size DESIGN_RESOLUTION;
	static const PairValue PAIR_ZERO;
	static const TripleValue TRIPLE_ZERO;
	static const FourValue FOUR_ZERO;

private:
	DataCenter(void);
	DataCenter(const DataCenter&){}
	~DataCenter(void);
	DataCenter& operator = (const DataCenter&){ return *s_dataCenter; }

	bool loadCommonTypeMap(void);
	bool loadDamageFactorMap(void);

	bool loadHoundStaticData(void);
	bool loadEnemyStaticData(void);

	bool loadHoundInfoFromXml(HoundInfo &info);

	// for multi-resolution scaling
	void scaleByDesign(cocos2d::Vec2 &design_vec2);
	void scaleByDesign(float &design_float);

	// data conversions
	void stringToVec2(const std::string &str, cocos2d::Vec2 &ret);
	void stringToPairValue(const std::string &str, PairValue &ret);
	void stringToTripleValue(const std::string &str, TripleValue &ret);
	void stringToFourValue(const std::string &str, FourValue &ret);
	void charToString(const char *pstr, std::string &ret)
	{
		ret = pstr==nullptr ? "" : pstr;
	}

	CommonTypeMap m_mapCommonTypes;
	TypeTypeValueMap m_mapDamageFactor;

	TypeLevelValueMap m_mapHoundMaxLife;
	TypeLevelValueMap m_mapHoundArmor;
	TypeLevelFourMap m_mapHoundWeaponDSAT;
	TypeLevelFourMap m_mapHoundProjectileDSAT;

	TypeLevelPairMap m_mapEnemyMaxLifeArmor;
	TypeLevelFourMap m_mapEnemyWeaponDSAT;
	TypeLevelFourMap m_mapEnemyProjectileDSAT;

	friend class std::auto_ptr<DataCenter>;
	static std::auto_ptr<DataCenter> s_dataCenter;
};

#endif //__HOUND_DATA_CENTER_H__
