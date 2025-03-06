#include "ECSSystemBase.h"

struct ComponentRigid;
struct ComponentTransform;

EDITORCLASS()
class ECSSystemPhysics : public ECSSystemBase
{
	EDITORBODY()
	static constexpr int GRID_SIZE = 128;

public:
	
	ECSSystemPhysics() = default;
	ECSSystemPhysics(const ECSSystemPhysics& other) = default;
	ECSSystemPhysics(ECSSystemPhysics&& other);
	~ECSSystemPhysics() = default;
	
	ECSSystemPhysics& operator=(ECSSystemPhysics& other) = default;
	ECSSystemPhysics& operator=(ECSSystemPhysics&& other) = default;
	

	bool CanMoveTo(int x, int y) const;

	void Tick(const ECSSystemTickParams& params, const std::tuple<ComponentTransform*, ComponentRigid*>& components);
	void OnEntityDeleted(const ECSSystemEntityDeletionParams& params, const std::tuple<ComponentTransform*, ComponentRigid*>& components);

protected:
	//~ Begin ECSSystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End ECSSystemBase Interface
	
	// Used to get coordinates into bGrid
	int GetEffectiveX(int x) const;
	int GetEffectiveY(int y) const;

	// Grid of whether a tile is rigid or not
	bool bGrid[GRID_SIZE][GRID_SIZE] = {};
};
