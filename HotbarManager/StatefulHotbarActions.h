#pragma once

class ActionHandlerBase;
class ActionHandlerSkill;
class HotbarManager;
class InputActionBase;
class InputContext;
class InputSystem;
class RPGSkillData;
class RPGSkillSystem;
struct OnInputActionDelegateParams;

// Used with the hotbar manager. When you activate an item on the hotbar it needs to ultimately provide an ActionDeciderBase for the player entity to do.
// Certain simple items like drinking a health potion can just immediately produce the action when you click the button but for other more
// complicated items they need some idea of a toggleable stateful action. (e.g. if you cast a spell you need to aim it and then confirm the aim target before returning an ActionDeciderSpell)
EDITORCLASS(Abstract, Instanced)
class StatefulHotbarActionBase
{
	EDITORBODY()
public:

	// Called when the player hits the hot bar button the first time
	virtual void ToggleOnStatefulAction() {};

	// Called when the player hits the hot bar button again, or a different button to start a new action 
	virtual void ToggleOffStatefulAction() {};

	// Fires when this stateful action has decided on the player action
	DelegateClass<HotbarManager, TransientPtr<ActionHandlerBase>> onPlayerActionDecided;

	// Fire this to request cancelling this stateful action 
	DelegateClass<HotbarManager> onRequestCancelStatefulAction;
};


// Starts aiming a skill, then can either cancel or confirm the spell
EDITORCLASS()
class StatefulHotbarActionCastSkill : public StatefulHotbarActionBase
{
	EDITORBODY()

public:

	void Setup(const RPGSkillData* pInSkill) { pSkill = pInSkill; }

protected:
	//~ Begin StatefulHotbarActionBase Interface
	void ToggleOnStatefulAction() override;
	void ToggleOffStatefulAction() override;
	//~ End StatefulHotbarActionBase Interface

	void OnConfirmSkill(const OnInputActionDelegateParams& params);
	void OnCancelSkill(const OnInputActionDelegateParams& params);
	
	const RPGSkillData* pSkill = nullptr;

	EDITORPROPERTY()
	RPGSkillSystem* pSkillSystem = nullptr;

	EDITORPROPERTY()
	InputSystem* pInputSystem = nullptr;

	// Input context to push while aiming
	EDITORPROPERTY()
	InputContext* pAimingSkillInputContext = nullptr;

	// Confirm and cast skill
	EDITORPROPERTY()
	InputActionBase* pConfirmSkillAction = nullptr;

	// Cancel aiming skill and return to regular gameplay
	EDITORPROPERTY()
	InputActionBase* pCancelSkillAction = nullptr;

	EDITORPROPERTY()
	ActionHandlerSkill* pActionHandlerSkill = nullptr;

	DelegateClass<StatefulHotbarActionCastSkill, const OnInputActionDelegateParams&> onConfirmSkillDelegate;
	DelegateClass<StatefulHotbarActionCastSkill, const OnInputActionDelegateParams&> onCancelSkillDelegate;
};
