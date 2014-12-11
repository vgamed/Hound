#include "GameScene.h"
#include "ScrollingBackground.h"
#include "Battlefield.h"
#include "UILayer.h"

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
	Texture2D *texture = nullptr;
	Vec2 direction(0.0f, 1.0f);

	for (const std::string &t_name : level.sbg_layer_texture_names)
	{
		chain.clear();
		texture = Director::getInstance()->getTextureCache()->getTextureForKey(t_name);
		sprite = Sprite::createWithTexture(texture);
		chain.push_back(sprite);
		layer = ScrollingBackground::create(	chain,
												direction,
												200.0f,
												100.0f);
		if (layer != nullptr)
		{
			this->addChild(layer);
		}
	}

	layer = Battlefield::create(player, level);
	if (layer != nullptr)
	{
		this->addChild(layer);
	}

	layer = UILayer::create();
	if (layer != nullptr)
	{
		this->addChild(layer);
	}

	return true;
}
