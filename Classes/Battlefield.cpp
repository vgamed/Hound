#include "Battlefield.h"
#include "Hound.h"
#include "Projectile/Projectile.h"
#include "Enemy/EnemyFactory.h"

USING_NS_CC;

const char * EVENT_CUSTOM_COLLISION = "Collision";
const char * EVENT_CUSTOM_DEBUG = "Debug";

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

	m_houndStartPosition.x = getBoundingBox().size.width/2 + level.hound_start_offset.x;
	m_houndStartPosition.y = m_hound->getBoundingBox().size.height + level.hound_start_offset.y;

	m_enemyStartPosCenter.x = getBoundingBox().size.width/2;
	m_enemyStartPosCenter.y = getBoundingBox().size.height - 200.0f;

	m_waveTimer = 0.0f;
	m_nextWave = m_enemyWaves.begin();

	// register custom event listeners
	auto listener = EventListenerCustom::create(EVENT_CUSTOM_COLLISION, 
		CC_CALLBACK_1(Battlefield::onEventCollision, this));
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 1);

	listener = EventListenerCustom::create(EVENT_CUSTOM_DEBUG, 
		CC_CALLBACK_1(Battlefield::onEventDebug, this));
	_eventDispatcher->addEventListenerWithFixedPriority(listener, 2);

	return true;
}

void Battlefield::update(float dt)
{
	// update hound
	m_hound->update(dt);

	// update current enemies
	//for (Enemy *enemy : m_activeEnemies)
	//{
	//	//enemy->update(dt);
	//	enemy->Enemy::update(dt); // for temporarily demonstrate the enemy waves
	//}
	
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

	// trigger new wave
	if ((m_activeEnemies.size()<=0) && (m_nextWave != m_enemyWaves.end()))
	{
		m_waveTimer += dt;
		if (m_waveTimer>=m_nextWave->time_offset)
		{
			spawnEnemyWave(*m_nextWave);
			m_nextWave++;
			m_waveTimer = 0.0f;
		}
	}
}

void Battlefield::onEnterTransitionDidFinish(void)
{
	auto size = getContentSize();
	m_hound->setPosition(m_houndStartPosition);
	m_hound->configWeapons();

	this->scheduleUpdate();
}

void Battlefield::onEventCollision(cocos2d::EventCustom* event)
{
}

void Battlefield::onEventDebug(cocos2d::EventCustom* event)
{
	DebugData *data = (DebugData*)event->getUserData();
	switch(data->command)
	{
	case DEBUG_COMMAND::ENEMY_SELF_DESTROY:
		removeActiveEnemy((Enemy*)(data->target));
		break;
	default:
		break;
	}
}

void Battlefield::addActiveProjectile(Projectile *proj)
{
	CC_ASSERT(proj!=nullptr);

	m_activeProjectiles.push_back(proj);
	this->addChild(proj);
}

bool Battlefield::removeActiveProjectile(Projectile *proj)
{
	std::vector<Projectile*>::iterator found = std::remove_if(m_activeProjectiles.begin(), 
		m_activeProjectiles.end(), 
		[&](Projectile *p)->bool { return (p == proj); });
	
	m_activeProjectiles.erase(found, m_activeProjectiles.end());
	
	this->removeChild(proj);
	
	return true;
}

void Battlefield::addActiveEnemy(Enemy *enemy)
{
	CC_ASSERT(enemy!=nullptr);

	m_activeEnemies.push_back(enemy);
	this->addChild(enemy);
}

bool Battlefield::removeActiveEnemy(Enemy *enemy)
{
	std::vector<Enemy*>::iterator found = std::remove_if(m_activeEnemies.begin(), 
		m_activeEnemies.end(), 
		[&](Enemy *p)->bool { return (p == enemy); });
	
	m_activeEnemies.erase(found, m_activeEnemies.end());
	
	this->removeChild(enemy);
	
	return true;
}

void Battlefield::spawnEnemyWave(const WaveInfo &info)
{
	for (const EnemyInfo &einfo : info.enemies)
	{
		Enemy *enemy = EnemyFactory::create(einfo);
		if (enemy != nullptr)
		{
			enemy->setPosition(m_enemyStartPosCenter+einfo.start_position_offset);
			addActiveEnemy(enemy);
		}
	}
}
