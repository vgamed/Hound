#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "Common.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();

	// for multi-resolution scaling
	void scaleByDesign(cocos2d::Vec2 &design_vec2);
	void scaleByDesign(float &design_float);

	// for finding max life, armor and damage ratio by various types
	float getHoundMaxLife(void);
	float getHoundArmor(void);
	const std::pair<float, float> getEnemyMaxLifeAndArmor(ENEMY_TYPE type, int level);
	float getDamageRatio(PROJECTILE_TYPE proj_type, ARMOR_TYPE armor_type);

private:
	PlayerInfo	m_playerInfo;
	LevelInfo	m_levelInfo;

	bool loadGameResources(void);
	bool loadPlayerInfo(void);
	bool loadLevelInfo(void);
};

#endif // _APP_DELEGATE_H_

