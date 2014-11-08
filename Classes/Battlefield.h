#ifndef __HOUND_BATTLEFIELD_H__
#define __HOUND_BATTLEFIELD_H__

#include "Common.h"

class Hound;
class Projectile;
class Enemy;

class Battlefield :
	public cocos2d::Layer
{
public:
	static Battlefield* create(const PlayerInfo &player, const LevelInfo &level);

	void update(float dt);
	void onEnterTransitionDidFinish(void);

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);
    
	const std::vector<Enemy*>& getActiveEnemies(void) const { return m_activeEnemies; }

	void onEventCollision(cocos2d::EventCustom* event);
	void onEventDebug(cocos2d::EventCustom* event);

	void addActiveProjectile(Projectile *proj);
	bool removeActiveProjectile(Projectile *proj);

	void addActiveEnemy(Enemy *enemy);
	bool removeActiveEnemy(Enemy *enemy);

	static const int INVALID_TOUCH_ID = -1;

protected:
	Battlefield(void);
	~Battlefield(void);

	bool init(const PlayerInfo &player, const LevelInfo &level);

	void spawnEnemyWave(const WaveInfo &info);
	void processProjectileCollidesField(cocos2d::Node *who, std::vector<cocos2d::Node*> &whom);
	void processProjectileCollidesHound(cocos2d::Node *who, std::vector<cocos2d::Node*> &whom);
	void processProjectileCollidesEnemy(cocos2d::Node *who, std::vector<cocos2d::Node*> &whom);
	void processHoundCollidesField(cocos2d::Node *who, std::vector<cocos2d::Node*> &whom);
	void processHoundCollidesEnemy(cocos2d::Node *who, std::vector<cocos2d::Node*> &whom);

private:
	Hound*							m_hound;
	cocos2d::Vec2					m_houndStartPosition;
	cocos2d::Vec2					m_movingOffset;

	cocos2d::Vec2					m_enemyStartPosCenter;
	std::vector<WaveInfo>			m_enemyWaves;

	float							m_waveTimer;
	std::vector<WaveInfo>::iterator m_nextWave;

	std::vector<Enemy*>				m_activeEnemies;
	std::vector<Projectile*>		m_activeProjectiles;
};

#endif //__HOUND_BATTLEFIELD_H__