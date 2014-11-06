#include "Battlefield.h"
#include "Hound.h"
#include "Projectile/Projectile.h"
#include "Enemy/EnemyFactory.h"
#include "SFX/SFXFactory.h"

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
	{
		return false;
	}

	m_enemyWaves = level.enemy_waves;

	// create hound
	m_hound = Hound::create(player.hound);
	if (m_hound == nullptr)
	{
		return false;
	}
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

	// test for sfx
	//explosion = Explosion::createWithSpriteFrames("b_", 1, 8, 1.0f, 1.0f);
	//explosion->setPosition(Vec2(150.0f, getBoundingBox().getMidY()));
	//addChild(explosion);
	//explosion = Explosion::createWithSpriteFrames("c_", 1, 8, 1.0f, 1.0f);
	//explosion->setPosition(Vec2(250.0f, getBoundingBox().getMidY()));
	//addChild(explosion);
	//explosion = Explosion::createWithSpriteFrames("d_00", 1, 9, 2.0f, 1.0f);
	//explosion->setPosition(Vec2(350.0f, getBoundingBox().getMidY()));
	//addChild(explosion);

	this->scheduleUpdate();
}

void Battlefield::onEventCollision(cocos2d::EventCustom* event)
{
	auto data = (CollisionData*)event->getUserData();
	switch(data->type)
	{
	case COLLISION_TYPE::PROJECTILE_TO_FIELD:
		processProjectileCollidesField(data->who, data->whom);
		break;
	case COLLISION_TYPE::PROJECTILE_TO_HOUND:
		processProjectileCollidesHound(data->who, data->whom);
		break;
	case COLLISION_TYPE::PROJECTILE_TO_ENEMY:
		processProjectileCollidesEnemy(data->who, data->whom);
		break;
	case COLLISION_TYPE::HOUND_TO_ENEMY:
		processHoundCollidesEnemy(data->who, data->whom);
		break;
	case COLLISION_TYPE::HOUND_TO_FIELD:
		processHoundCollidesField(data->who, data->whom);
		break;
		break;
	default:
		break;
	}
}

void Battlefield::onEventDebug(cocos2d::EventCustom* event)
{
	auto data = (DebugData*)event->getUserData();
	switch(data->command)
	{
	case DEBUG_COMMAND::ENEMY_SELF_DESTROY:
		{
			auto enemy = dynamic_cast<Enemy*>(data->target);
			removeActiveEnemy(enemy);
		}
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

void Battlefield::processProjectileCollidesField(Node *who, std::vector<Node*> &whom)
{
	CC_ASSERT(dynamic_cast<Projectile*>(who) != nullptr); // who should be a projectile here

	removeActiveProjectile((Projectile*)who);
}

void Battlefield::processProjectileCollidesHound(Node *who, std::vector<Node*> &whom)
{
	CC_ASSERT(dynamic_cast<Projectile*>(who) != nullptr); // who should be a projectile here
	CC_ASSERT((whom.size()==1) && (dynamic_cast<Hound*>(whom[0])!=nullptr)); // whom should be the hound
}

void Battlefield::processProjectileCollidesEnemy(Node *who, std::vector<Node*> &whom)
{
	auto proj = dynamic_cast<Projectile*>(who);
	CC_ASSERT(proj != nullptr); // who should be a projectile here
	CC_ASSERT(whom.size() > 0); // whom should be active enemies

	for (Node* node : whom)
	{
		auto enemy = dynamic_cast<Enemy*>(node);
		CC_ASSERT(enemy != nullptr);
		
		// play hitting effect
		auto sfx = SFXFactory::createProjectileSFX(proj->getProjectileType());
		if (sfx != nullptr)
		{
			sfx->setPosition(proj->getPosition());
			this->addChild(sfx);
		}

		enemy->doDamage(proj->getDamage());
		if (enemy->isDead())
		{
			// play explosion effect
			auto sfx = SFXFactory::createEnemyExplosionSFX(enemy->getType());
			if (sfx != nullptr)
			{
				sfx->setPosition(enemy->getPosition());
				this->addChild(sfx);
			}

			removeActiveEnemy(enemy);
		}
	}

	removeActiveProjectile(proj);
}

void Battlefield::processHoundCollidesField(Node *who, std::vector<Node*> &whom)
{
	CC_ASSERT(dynamic_cast<Hound*>(who) != nullptr); // who should be the hound

}

void Battlefield::processHoundCollidesEnemy(Node *who, std::vector<Node*> &whom)
{
	CC_ASSERT(dynamic_cast<Hound*>(who) != nullptr); // who should be the hound
	CC_ASSERT(whom.size() > 0); // whom should be active enemies

}

