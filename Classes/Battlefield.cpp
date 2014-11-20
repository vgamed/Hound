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

	m_hound->setTouchID(INVALID_TOUCH_ID); // no available touching inputs

	m_houndStartPosition.x = getBoundingBox().size.width/2 + level.hound_start_offset.x;
	m_houndStartPosition.y = m_hound->getBoundingBox().size.height + level.hound_start_offset.y;

	m_waveTimer = 0.0f;
	m_nextWave = m_enemyWaves.begin();

	// register touch listeners
    auto listener_touch = EventListenerTouchOneByOne::create();
    listener_touch->setSwallowTouches(true);
    listener_touch->onTouchBegan = CC_CALLBACK_2(Battlefield::onTouchBegan, this);
    listener_touch->onTouchMoved = CC_CALLBACK_2(Battlefield::onTouchMoved, this);
    listener_touch->onTouchEnded = CC_CALLBACK_2(Battlefield::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener_touch, this);

	// register custom event listeners
	auto listener_custom = EventListenerCustom::create(EVENT_CUSTOM_COLLISION, 
		CC_CALLBACK_1(Battlefield::onEventCollision, this));
	_eventDispatcher->addEventListenerWithFixedPriority(listener_custom, 1);

	listener_custom = EventListenerCustom::create(EVENT_CUSTOM_DEBUG, 
		CC_CALLBACK_1(Battlefield::onEventDebug, this));
	_eventDispatcher->addEventListenerWithFixedPriority(listener_custom, 2);

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
	m_hound->setPosition(m_houndStartPosition);

	this->scheduleUpdate();
}

bool Battlefield::onTouchBegan(Touch *touch, Event *event)
{
	// remove the following part if you wanna control like À×öªÕ½»ú
	//Rect bb = m_hound->getBoundingBox();
	//if (!bb.containsPoint(touch->getLocation()))
	//	return false;

	//if (!m_hound->getIsMoving())
	//{
	//	m_movingOffset = m_hound->getPosition() - touch->getLocation();
	//	m_hound->setIsMoving(true);
	//}

	return true;
}

void Battlefield::onTouchMoved(Touch *touch, Event *event)
{
	if (m_hound->getTouchID() == INVALID_TOUCH_ID)
	{
		m_movingOffset = m_hound->getPosition() - touch->getLocation();
		m_hound->setTouchID(touch->getID());
		return ;
	}

	if (m_hound->getTouchID() == touch->getID())
	{
		Vec2 new_pos = touch->getLocation()+m_movingOffset;
		Rect bb = getBoundingBox();
		float radius = m_hound->getBoundingCircleRadius();

		if (new_pos.x+radius > bb.getMaxX())
		{
			new_pos.x = bb.getMaxX() - radius;
		}
		else if (new_pos.x-radius < bb.getMinX())
		{
			new_pos.x = bb.getMinX() + radius;
		}

		if (new_pos.y+radius > bb.getMaxY())
		{
			new_pos.y = bb.getMaxY() - radius;
		}
		else if (new_pos.y-radius < bb.getMinY())
		{
			new_pos.y = bb.getMinY() + radius;
		}

		m_hound->setPosition(new_pos);
		m_movingOffset = m_hound->getPosition() - touch->getLocation();
	}
}

void Battlefield::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
	if (m_hound->getTouchID() == touch->getID())
	{
		m_hound->setTouchID(INVALID_TOUCH_ID);
	}
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
			enemy->setPosition(einfo.start_position);
			enemy->getStateMachine().triggerEvent(0);
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

