#include "GameScene.h"
#include "ScrollingBackground.h"
#include "Battlefield.h"

USING_NS_CC;

GameScene::GameScene(void)
{
}


GameScene::~GameScene(void)
{
}

GameScene* GameScene::create(const PlayerInfo &player, const LevelInfo &level)
{
	auto scene = new GameScene();
	if (scene && scene->init(player,level))
	{
		scene->autorelease();
		return scene;
	}
	CC_SAFE_DELETE(scene);
	return nullptr;
}

bool GameScene::init(const PlayerInfo &player, const LevelInfo &level)
{
	if( !Scene::init() )
		return false;

	ScrollingBackground::SpriteChain chain;
	Sprite* sprite = nullptr;
	Layer* layer = nullptr;
		
	sprite = Sprite::create("img_bg_1.jpg");
	chain.push_back(sprite);
	layer = ScrollingBackground::create(	chain,
											Vec2(0.0f,1.0f),
											200.0f,
											100.0f);
	if (layer != nullptr)
	{
		this->addChild(layer);
	}

	chain.clear();
	sprite = Sprite::create("clouds.png");
	chain.push_back(sprite);
	layer = ScrollingBackground::create(	chain,
											Vec2(0.0f,1.0f),
											100.0f);
	if (layer != nullptr)
	{
		this->addChild(layer);
	}

	layer = Battlefield::create(player, level);
	if (layer != nullptr)
	{
		this->addChild(layer);
	}

	return true;
}
