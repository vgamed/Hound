#ifndef __HOUND_GAME_SCENE_H__
#define __HOUND_GAME_SCENE_H__

#include "Common.h"

class GameScene :
	public cocos2d::Scene
{
public:
	static GameScene* create(const PlayerInfo &player, const LevelInfo &level);

protected:
	GameScene(void);
	~GameScene(void);

	bool init(const PlayerInfo &player, const LevelInfo &level);
};

#endif //__HOUND_GAME_SCENE_H__
