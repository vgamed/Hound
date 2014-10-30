#include "Battlefield.h"
#include "Hound.h"

USING_NS_CC;

Battlefield::Battlefield(void)
{
}


Battlefield::~Battlefield(void)
{
}

Battlefield* Battlefield::create(const PlayerInfo &player, const LevelInfo &level)
{
	auto bf = new Battlefield();
	if (bf && bf->init(player,level))
	{
		bf->autorelease();
		return bf;
	}
	CC_SAFE_DELETE(bf);
	return nullptr;
}

bool Battlefield::init(const PlayerInfo &player, const LevelInfo &level)
{
	if (!Layer::init())
		return false;

	m_enemyWaves = level.enemy_waves;

	// create hound
	m_hound = Hound::create(player.hound);
	if (m_hound == nullptr)
		return false;

	this->addChild(m_hound);

	return true;
}

void Battlefield::onEnterTransitionDidFinish(void)
{
	auto size = getContentSize();
	Vec2 origin(0.0f,0.0f);
	m_hound->setPosition(Vec2(origin.x+size.width/2, origin.y+m_hound->getBoundingBox().size.height));
}
