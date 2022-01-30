#pragma once

/// #TEMP: PRAGMA
#pragma optimize( "", off )

static constexpr int NUM_ENTITIES = 1024;

enum class EComponents
{
	Mesh,
	Transform,
};

class Components
{
public:

	bool HasComponent(EComponents component) const { return components & GetBitValue(component); }
	void AddComponent(EComponents component) { components |= GetBitValue(component); }
	void RemoveComponent(EComponents component) { components &= ~GetBitValue(component); }

private:

	u64 GetBitValue(EComponents component) const { return (1ull << (u64)component); }

	u64 components; // Bitfield of all components
};

class Entity
{
public:
	Components components;
};

class RendererObject;
struct Mesh
{
	RendererObject* pRendererObject = nullptr;
};

struct Transform
{
	int x;
	int y;
};

#define REGISTER_COMPONENT(ComponentName) \
private:\
ComponentName m##ComponentName[NUM_ENTITIES];\
public:\
template<>\
ComponentName& GetComponent<ComponentName>(int entityId) { return m##ComponentName[entityId]; }\
template<>\
bool EntityHasComponents<ComponentName>(int entityId) const { return entities[entityId].components.HasComponent(EComponents::##ComponentName); }

class SystemBase;
class SystemCallbackBase;
class ECS
{

public:
	void RegisterSystem(std::unique_ptr<SystemBase>&& pSystem);
	void RegisterSystemCallback(std::unique_ptr<SystemCallbackBase>&& pSystemCallback) { systemCallbacks.emplace_back(std::move(pSystemCallback)); }

	void Tick(float deltaTime);

	Entity& GetEntity(int index) { return entities[index]; }

	template<typename T>
	T& GetComponent(int entityId) { DOMASSERT(false, "This shouldn't be called! Are you missing a REGISTER_COMPONENT?"); }
	
	template<typename THead, typename... TTail>
	bool EntityHasComponents(int entityId) const;

	REGISTER_COMPONENT(Mesh);
	REGISTER_COMPONENT(Transform);

private:
	std::vector<std::unique_ptr<SystemBase>> systems;
	std::vector<std::unique_ptr<SystemCallbackBase>> systemCallbacks;

	Entity entities[NUM_ENTITIES];
};

template<typename THead, typename... TTail>
bool ECS::EntityHasComponents(int entityId) const
{
	return EntityHasComponents<THead>(entityId) && EntityHasComponents<TTail...>(entityId);
}

class SystemBase
{
public:
	SystemBase() = default;
	SystemBase(const SystemBase& other) = default;
	SystemBase(SystemBase&& other) = default;
	virtual ~SystemBase() = default;
	
	SystemBase& operator=(SystemBase& other) = default;
	SystemBase& operator=(SystemBase&& other) = default;
	

	void Initialise(ECS* pEcs) { DoInitialise(pEcs); };

protected:
	virtual void DoInitialise(ECS* pEcs) = 0;
};

class SystemRender : public SystemBase
{
public:
	void Tick(float deltaTime, int entityIndex, const std::tuple<Mesh*, Transform*>& components);

protected:
	//~ Begin SystemBase Interface
	void DoInitialise(ECS* pEcs) override;
	//~ End SystemBase Interface
};

class SystemCallbackBase
{
public:
	SystemCallbackBase() = default;
	SystemCallbackBase(const SystemCallbackBase& other) = default;
	SystemCallbackBase(SystemCallbackBase&& other) = default;
	virtual ~SystemCallbackBase() = default;

	SystemCallbackBase& operator=(SystemCallbackBase& other) = default;
	SystemCallbackBase& operator=(SystemCallbackBase&& other) = default;

	void HandleCallbacks(ECS* pEcs, float deltaTime) { DoCallback(pEcs, deltaTime); }

protected:

	virtual void DoCallback(ECS* pEcs, float deltaTime) = 0;
};

template<typename TSystem, typename... T>
class SystemCallback : public SystemCallbackBase
{
public:
	SystemCallback(TSystem* pInSystem) : pSystem(pInSystem) {};
	SystemCallback(const SystemCallback& other) = default;
	SystemCallback(SystemCallback&& other) = default;
	~SystemCallback() = default;
	
	SystemCallback& operator=(SystemCallback& other) = default;
	SystemCallback& operator=(SystemCallback&& other) = default;
	

protected:

	template<typename THead, typename... TTail>
	void PopulateTuple(std::tuple<T*...>& tuple, ECS* pEcs, int entityIndex)
	{
		std::get<THead*>(tuple) = &pEcs->GetComponent<THead>(entityIndex);
		if constexpr(sizeof...(TTail) > 0)
		{ 
			PopulateTuple<TTail...>(tuple, pEcs, entityIndex); 
		}
	};

	//~ Begin SystemCallbackBase Interface
	void DoCallback(ECS* pEcs, float deltaTime) override;
	//~ End SystemCallbackBase Interface

	TSystem* pSystem = nullptr;
};

template<typename TSystem, typename... T>
void SystemCallback<TSystem, T...>::DoCallback(ECS* pEcs, float deltaTime)
{
	// #TODO: This should probably cache a list of entities that updates if anything changes
	for (int i = 0; i < NUM_ENTITIES; ++i)
	{
		if (pEcs->EntityHasComponents<T...>(i))
		{
			std::tuple<T*...> tuple;
			PopulateTuple<T...>(tuple, pEcs, i);
			pSystem->Tick(deltaTime, i, std::move(tuple));
		}
	}
}


// SystemRender
void SystemRender::DoInitialise(ECS* pEcs)
{
	pEcs->RegisterSystemCallback(std::make_unique<SystemCallback<SystemRender, Mesh, Transform>>(this));
}

#include "RendererObject.h"
void SystemRender::Tick(float deltaTime, int entityIndex, const std::tuple<Mesh*, Transform*>& components)
{
	RendererObject* pRendererObject = std::get<Mesh*>(components)->pRendererObject;
	Transform* transform = std::get<Transform*>(components);

	pRendererObject->SetSize(100.f, 100.f);
	pRendererObject->SetPosition(100.f * transform->x, 100.f * transform->y);
}

// ECS
void ECS::RegisterSystem(std::unique_ptr<SystemBase>&& pSystem)
{
	pSystem->Initialise(this);
	systems.emplace_back(std::move(pSystem));
}

void ECS::Tick(float deltaTime)
{
	for (std::unique_ptr<SystemCallbackBase>& callback : systemCallbacks)
	{
		callback->HandleCallbacks(this, deltaTime);
	}
}

#pragma optimize( "", on )
