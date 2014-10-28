#ifndef __BATTLEFIELD_H__
#define __BATTLEFIELD_H__

#include "Common.h"

class Hound;

class Battlefield :
	public cocos2d::Layer
{
public:
	static Battlefield* create(const PlayerInfo &player, const LevelInfo &level);

	void onEnterTransitionDidFinish(void);

protected:
	Battlefield(void);
	~Battlefield(void);

	bool init(const PlayerInfo &player, const LevelInfo &level);

private:
	std::vector<Wave*> m_enemyWaves;

	Hound* m_hound;
	std::vector<cocos2d::Sprite*> m_curEnemies;
};

#endif //__BATTLEFIELD_H__