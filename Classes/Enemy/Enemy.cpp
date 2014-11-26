#include "Enemy.h"
#include "EnemyStateFactory.h"

USING_NS_CC;

Enemy::Enemy(void)
	: m_stateMachine(*this)
	, m_invincible(false)
{
	m_stateMap.clear();
}


Enemy::~Enemy(void)
{
	for (auto state : m_stateMap)
	{
		if (state.second != nullptr)
			delete state.second;
	}
}

bool Enemy::init(const EnemyInfo &info)
{
	Texture2D *texture = 
		Director::getInstance()->getTextureCache()->getTextureForKey(info.body_texture_name);
	if (!initWithTexture(texture))
	{
		return false;
	}

	m_type = info.type;
	m_curLife = m_maxLife = info.max_life;
	m_armor = info.armor;

	setScale(info.scale_xy);
	setLocalZOrder(ZORDER_ENEMY_FIGHTER);

	m_boundingCircle.radius = info.bounding_circle_radius;

	// init state transit for state machine
	for (const auto &state_info : info.state_infoes)
	{
		auto state = EnemyStateFactory::create(state_info);
		if (state != nullptr)
		{
			m_stateMap.insert(std::make_pair(state->getId(), state));
		}
	}

	for (const auto &state_map_info : info.state_map_infoes)
	{
		EnemyStateTransit trans;
		trans.event = (int)(state_map_info.event);
		auto it = m_stateMap.find(state_map_info.from);
		if (it == m_stateMap.end())
		{
			trans.from = nullptr;
		}
		else
		{
			trans.from = it->second;
			CC_ASSERT(trans.from->getId() == state_map_info.from);
		}

		it = m_stateMap.find(state_map_info.to);
		CC_ASSERT(it != m_stateMap.end());
		trans.to = it->second;
		CC_ASSERT(trans.to->getId() == state_map_info.to);
		
		m_stateMachine.addStateTransition(trans);
	}

	// initialize weapons
	auto scale = Director::getInstance()->getContentScaleFactor();
	for (const auto &winfo : info.weapons)
	{
		auto weapon = Weapon::create(winfo, false);
		if (weapon != nullptr)
		{
			m_weapons.push_back(weapon);
			addChild(weapon);
			weapon->setPosition(winfo.dock_position / scale);
			weapon->setRotation(winfo.rotate_angle);
		}
	}

	scheduleUpdate();

	return true;
}

void Enemy::update(float dt)
{
	m_stateMachine.update(dt);
}

void Enemy::doDamage(float damage)
{
	if (m_invincible)
		return;

	m_curLife -= damage*(1-m_armor/1000.0f);
}
