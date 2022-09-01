#include "SystemBase.h"

struct ComponentHealth;

class SystemDamage : public SystemBase
{
	
public:
	void Tick(const SystemTickParams& params, const std::tuple<ComponentHealth*>& components);
	void OnEntityDeleted(const SystemEntityDeletionParams& params, const std::tuple<ComponentHealth*>& components) {};

protected:
	//~ Begin SystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End SystemBase Interface
};
