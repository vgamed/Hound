#include "Hound.h"
#include "AppDelegate.h"

USING_NS_CC;

const int Hound::TAG = 1001;

Hound::Hound(void)
	: m_stateMachine(*this)
	, m_invincible(false)
{
	m_stateMap.clear();
}


Hound::~Hound(void)
{
}

Hound* Hound::create(const HoundInfo &hdi)
{
    Hound *hound = new (std::nothrow) Hound();
	if (hound && hound->init(hdi))
    {
        hound->autorelease();
        return hound;
    }
    CC_SAFE_DELETE(hound);
    return nullptr;
}

bool Hound::init(const HoundInfo &hdi)
{
	Texture2D *texture = 
		Director::getInstance()->getTextureCache()->getTextureForKey(hdi.body_texture_name);
	if (!initWithTexture(texture))
	{
		return false;
	}

	setTag(TAG);
	setName("Hound");
	setScale(hdi.scale_xy);
	setLocalZOrder(ZORDER_HOUND);

	m_boundingCircle.radius = hdi.bounding_circle_radius;

	m_entryFrom = hdi.entry_from;
	m_entryTo = hdi.entry_to;
	m_entrySpeed = hdi.entry_speed;
	m_entryAutoFacing = hdi.entry_auto_facing;
	m_leaveSpeed = hdi.leave_speed;
	m_leaveAutoFacing = hdi.leave_auto_facing;

	auto app = dynamic_cast<AppDelegate*>(Application::getInstance());
	m_curLife = m_maxLife = app->getHoundMaxLife();
	m_armor = app->getHoundArmor();

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

	// Entry State
	state.id = 1;
	state.type = STATE_TYPE::ENTRY;
	state.movements.clear();
	state.weapons.clear();
	m_stateMap.insert(std::make_pair(state.id, new HoundEntryState(state)));

	// Battle State
	state.id = 2;
	state.type = STATE_TYPE::BATTLE_PHASE;
	state.movements.clear();
	state.weapons.clear();
	for (auto weapon : m_weapons)
	{
		state.weapons.push_back(weapon->getId());
	}
	m_stateMap.insert(std::make_pair(state.id, new HoundBattlePhaseState(state)));

	// Leave State
	state.id = 3;
	state.type = STATE_TYPE::LEAVE;
	state.movements.clear();
	state.weapons.clear();
	m_stateMap.insert(std::make_pair(state.id, new HoundLeaveState(state)));

	// Dead State
	state.id = 4;
	state.type = STATE_TYPE::DEAD;
	state.movements.clear();
	state.weapons.clear();
	m_stateMap.insert(std::make_pair(state.id, new HoundDeadState(state)));
	//

	// BattleEnd State
	state.id = 5;
	state.type = STATE_TYPE::BATTLE_END;
	state.movements.clear();
	state.weapons.clear();
	m_stateMap.insert(std::make_pair(state.id, new HoundBattleEndState(state)));
	//

	// state transitions
	HoundStateTransit trans;
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

	trans.event = (int)(STATE_MACHINE_EVENT::HOUND_DEAD);
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
