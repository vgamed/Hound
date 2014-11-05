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

	void onEventCollision(cocos2d::EventCustom* event);
	void onEventDebug(cocos2d::EventCustom* event);

	void addActiveProjectile(Projectile *proj);
	bool removeActiveProjectile(Projectile *proj);

	void addActiveEnemy(Enemy *enemy);
	bool removeActiveEnemy(Enemy *enemy);

protected:
	Battlefield(void);
	~Battlefield(void);

	bool init(const PlayerInfo &player, const LevelInfo &level);

	void spawnEnemyWave(const WaveInfo &info);

private:
	Hound*							m_hound;
	cocos2d::Vec2					m_houndStartPosition;

	cocos2d::Vec2					m_enemyStartPosCenter;
	std::vector<WaveInfo>			m_enemyWaves;

	float							m_waveTimer;
	std::vector<WaveInfo>::iterator m_nextWave;

	std::vector<Enemy*>				m_activeEnemies;
	std::vector<Projectile*>		m_activeProjectiles;
};

#endif //__HOUND_BATTLEFIELD_H__