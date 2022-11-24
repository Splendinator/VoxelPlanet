#include "SystemBase.h"

#include "DomUtils/Pointers.h"
#include "UICanvas.h"

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

/** SystemNameslate
*
* System to render nameslates above units with health
*/
class SystemNameslate : public SystemBase
{

public:
	void Tick(const SystemTickParams& params, const std::tuple<ComponentTransform*, ComponentHealth*>& components);
	void OnEntityDeleted(const SystemEntityDeletionParams& params, const std::tuple<ComponentTransform*, ComponentHealth*>& components);

protected:
	//~ Begin SystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End SystemBase Interface

	HealthBarNameslate nameslates[NUM_ENTITIES]; // One nameslate for each entity
};
