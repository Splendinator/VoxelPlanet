#include "SystemBase.h"

struct ComponentRigid;
struct ComponentTransform;

class SystemPhysics : public SystemBase
{
	static constexpr int GRID_SIZE = 256;

public:
	
	SystemPhysics() = default;
	SystemPhysics(const SystemPhysics& other) = default;
	SystemPhysics(SystemPhysics&& other);
	~SystemPhysics();
	
	SystemPhysics& operator=(SystemPhysics& other) = default;
	SystemPhysics& operator=(SystemPhysics&& other) = default;
	

	bool CanMoveTo(int x, int y) const;

	void Tick(const SystemTickParams& params, const std::tuple<ComponentTransform*, ComponentRigid*>& components);
	void OnEntityDeleted(const SystemEntityDeletionParams& params, const std::tuple<ComponentTransform*, ComponentRigid*>& components);

protected:
	//~ Begin SystemBase Interface
	void InitialiseInternal(ECS* pEcs) override;
	//~ End SystemBase Interface
	
	// Used to get coordinates into bGrid
	int GetEffectiveX(int x) const;
	int GetEffectiveY(int y) const;

	// Grid of whether a tile is rigid or not
	bool bGrid[GRID_SIZE][GRID_SIZE] = {};
};
