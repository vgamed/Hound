#include "Battlefield.h"
#include "Hound.h"
#include "Projectile.h"

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

void Battlefield::update(float dt)
{
	// update hound
	m_hound->update(dt);

	// update current enemies
	//...
	
	// trigger new wave
	//...

	// projectile collision detect
	std::vector<Projectile*>::iterator found = std::remove_if(m_activeProjectiles.begin(), 
		m_activeProjectiles.end(), 
		[&](Projectile *p)->bool
		{
			if (p->getBoundingBox().intersectsRect(this->getBoundingBox()))
			{
				return false;
			}
			else
			{
				this->removeChild(p);
				return true;
			}
		});
	m_activeProjectiles.erase(found, m_activeProjectiles.end());

	// if the left projectiles hitting some sprite, do damage calculation and remove the projectile
	//...
}

void Battlefield::onEnterTransitionDidFinish(void)
{
	auto size = getContentSize();
	Vec2 origin(0.0f,0.0f);
	m_hound->setPosition(Vec2(origin.x+size.width/2, origin.y+m_hound->getBoundingBox().size.height+100.0f));
	m_hound->configWeapons();

	this->scheduleUpdate();
}

void Battlefield::addProjectile(Projectile *proj)
{
	CC_ASSERT(proj!=nullptr);
	m_activeProjectiles.push_back(proj);
	this->addChild(proj);
}

void Battlefield::removeProjectile(Projectile *proj)
{
	std::vector<Projectile*>::iterator found = std::remove_if(m_activeProjectiles.begin(), 
		m_activeProjectiles.end(), 
		[&](Projectile *p)->bool { return (p == proj); });
	m_activeProjectiles.erase(found, m_activeProjectiles.end());
	this->removeChild(proj);
}
