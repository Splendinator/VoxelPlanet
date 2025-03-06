#include "ECSSystemBase.h"

struct ComponentHealth;

EDITORCLASS()
class ECSSystemDamage : public ECSSystemBase
{
	EDITORBODY()
public:
	void Tick(const ECSSystemTickParams& params, const std::tuple<ComponentHealth*>& components);
	void OnEntityDeleted(const ECSSystemEntityDeletionParams& params, const std::tuple<ComponentHealth*>& components) {};

protected:
	//~ Begin ECSSystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End ECSSystemBase Interface
};
