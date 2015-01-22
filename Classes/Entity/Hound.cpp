#include "Hound.h"
#include "AppDelegate.h"

USING_NS_CC;

Hound::Hound(void)
	: Entity((int)ENTITY_TYPE::HOUND)
{
}


Hound::~Hound(void)
{
}

Hound* Hound::create(const HoundInfo &hdi, const LevelInfo &lli)
{
    Hound *hound = new (std::nothrow) Hound();
	if (hound && hound->init(hdi, lli))
    {
        hound->autorelease();
        return hound;
    }
    CC_SAFE_DELETE(hound);
    return nullptr;
}

bool Hound::init(const HoundInfo &hdi, const LevelInfo &lli)
{
	Texture2D *texture = 
		Director::getInstance()->getTextureCache()->getTextureForKey(hdi.body_asset_name);
	if (!initWithTexture(texture))
	{
		return false;
	}

	setName("Hound");
	setLocalZOrder(ZORDER_HOUND);

	m_boundingCircle.radius = lli.hound_bounding_circle_radius;

	m_entryFrom = lli.hound_entry_from;
	m_entryTo = lli.hound_entry_to;
	m_entrySpeed = lli.hound_entry_speed;
	m_entryAutoFacing = lli.hound_entry_auto_facing;
	m_leaveSpeed = lli.hound_leave_speed;
	m_leaveAutoFacing = lli.hound_leave_auto_facing;
	setScale(lli.hound_scale);

	m_curLife = m_maxLife = hdi.max_life;
	m_armor = hdi.armor;

	// initialize weapons
	auto scale = Director::getInstance()->getContentScaleFactor();
	for (const auto &winfo : hdi.weapons)
	{
		auto weapon = Weapon::create(winfo);
		if (weapon != nullptr)
		{
			m_weapons.push_back(weapon);
			addChild(weapon);
			weapon->setPosition(winfo.dock_position / scale);
			weapon->setRotation(getRotation() + winfo.rotate_angle);
		}
	}
	
	// initialize wingmen
	//...

	// init states and state transitions for state machine
	StateInfo state;
	Movement movement;

	state.repeat_movements = false;
	state.life_threshold = -FLT_MAX;

	// Entry State
	state.id = 1;
	state.type = (int)STATE_TYPE::ENTRY;
	state.movements.clear();
	state.weapons.clear();
	m_stateMap.insert(std::make_pair(state.id, new EntityEntryState(state)));

	// Battle State
	state.id = 2;
	state.type = (int)STATE_TYPE::BATTLE_PHASE;
	state.movements.clear();
	state.weapons.clear();
	for (auto weapon : m_weapons)
	{
		state.weapons.push_back(weapon->getId());
	}
	m_stateMap.insert(std::make_pair(state.id, new EntityBattlePhaseState(state)));

	// Leave State
	state.id = 3;
	state.type = (int)STATE_TYPE::LEAVE;
	state.movements.clear();
	state.weapons.clear();
	m_stateMap.insert(std::make_pair(state.id, new EntityLeaveState(state)));

	// Dead State
	state.id = 4;
	state.type = (int)STATE_TYPE::DEAD;
	state.movements.clear();
	state.weapons.clear();
	m_stateMap.insert(std::make_pair(state.id, new EntityDeadState(state)));
	//

	// BattleEnd State
	state.id = 5;
	state.type = (int)STATE_TYPE::BATTLE_END;
	state.movements.clear();
	state.weapons.clear();
	m_stateMap.insert(std::make_pair(state.id, new EntityBattleEndState(state)));
	//

	// state transitions
	EntityStateTransit trans;
	trans.event = (int)(STATE_MACHINE_EVENT::START);
	trans.from = nullptr;
	trans.to = m_stateMap.find(1)->second;
	m_stateMachine.addStateTransition(trans);

	trans.event = (int)(STATE_MACHINE_EVENT::ENTRY_FINISHED);
	trans.from = m_stateMap.find(1)->second;
	trans.to = m_stateMap.find(2)->second;
	m_stateMachine.addStateTransition(trans);

	trans.event = (int)(STATE_MACHINE_EVENT::VICTORY);
	trans.from = nullptr;
	trans.to = m_stateMap.find(3)->second;
	m_stateMachine.addStateTransition(trans);

	trans.event = (int)(STATE_MACHINE_EVENT::DESTROYED);
	trans.from = nullptr;
	trans.to = m_stateMap.find(4)->second;
	m_stateMachine.addStateTransition(trans);

	trans.event = (int)(STATE_MACHINE_EVENT::LEVEL_QUIT);
	trans.from = nullptr;
	trans.to = m_stateMap.find(5)->second;
	m_stateMachine.addStateTransition(trans);
	//

	return true;
}

void Hound::update(float dt)
{
	// update hound states
	m_stateMachine.update(dt);

	//// update weapons
	//for (auto weapon : m_weapons)
	//{
	//	weapon->update(dt);
	//}
}

void Hound::doDamage(float damage)
{
	if (m_invincible)
		return;

	m_curLife -= damage*(1-m_armor/1000.0f);
}
