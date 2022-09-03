#include "SystemBase.h"

class RendererObject;
class VectorPrimitiveRectangle;
struct ComponentHealth;
struct ComponentTransform;

class HealthBarNameslate
{
public:
	void Init();
	void Uninit();
	void SetPos(int x, int y);
	void SetHealth(int health, int maxHealth);

	bool IsInitialised() const { return pHealthBarProgress != nullptr; }
	
protected:
	VectorPrimitiveRectangle* pHealthBarProgress = nullptr;
	RendererObject* pHealthBarRenderObject = nullptr;
};

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
