#include "ECSSystemBase.h"

#include "DomUtils/Pointers.h"
#include "UI/UICanvas.h"

class UIObjectProgressBar;
struct ComponentHealth;
struct ComponentTransform;

// Nameslate UI
class HealthBarNameslate
{
public:
	void Init();
	void Uninit();
	void SetPos(int x, int y);
	void SetHealth(int health, int maxHealth);
	
	bool IsInitialised() { return canvas.get() != nullptr; }

protected:
	std::unique_ptr<UICanvas> canvas;
	
	TransientPtr<UIObjectProgressBar> pHealthBarProgress = nullptr;
};

/** ECSSystemNameslate
*
* System to render nameslates above units with health
*/
class ECSSystemNameslate : public ECSSystemBase
{

public:
	void Tick(const ECSSystemTickParams& params, const std::tuple<ComponentTransform*, ComponentHealth*>& components);
	void OnEntityDeleted(const ECSSystemEntityDeletionParams& params, const std::tuple<ComponentTransform*, ComponentHealth*>& components);

protected:
	//~ Begin ECSSystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End ECSSystemBase Interface

	HealthBarNameslate nameslates[NUM_ENTITIES]; // One nameslate for each entity
};
