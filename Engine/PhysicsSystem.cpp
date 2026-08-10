#include "PhysicsSystem.h"
#include "PhysicsComponent.h"
#include "GameObject.h"
#include <algorithm>
#include <cassert>

// ---------------------------------------------------------------------------
PhysicsSystem* PhysicsSystem::get()
{
    static PhysicsSystem instance;
    return &instance;
}

void PhysicsSystem::initialize(const rp3d::Vector3& gravity)
{
    if (m_initialized) return;

    rp3d::PhysicsWorld::WorldSettings settings;
    settings.gravity           = gravity;
    settings.isSleepingEnabled = true;

    m_physicsWorld = m_physicsCommon.createPhysicsWorld(settings);
    assert(m_physicsWorld && "PhysicsSystem: failed to create PhysicsWorld");
    m_physicsWorld->enableSleeping(true);

    m_initialized = true;
    m_accumulator = 0.0f;
}

void PhysicsSystem::update(float deltaTime)
{
    if (!m_initialized || !m_physicsWorld) return;

    // Cap delta to avoid spiral-of-death on large frame spikes
    if (deltaTime > 0.25f) deltaTime = 0.25f;

    m_accumulator += deltaTime;

    constexpr int MAX_STEPS = 4;
    int steps = 0;
    while (m_accumulator >= FIXED_STEP && steps < MAX_STEPS)
    {
        m_physicsWorld->update(FIXED_STEP);
        m_accumulator -= FIXED_STEP;
        ++steps;
    }

    // Prevent unbounded accumulation if simulation falls behind
    if (m_accumulator > FIXED_STEP * 2.0f)
        m_accumulator = 0.0f;

    // Sync each component's rigid body transform back to its TransformComponent
    for (PhysicsComponent* comp : m_components)
    {
        if (comp) comp->update(deltaTime);
    }
}

PhysicsComponent* PhysicsSystem::createComponent(GameObject* owner, PhysicsComponent::BodyType type)
{
    assert(m_initialized && "PhysicsSystem::createComponent called before initialize()");
    assert(owner         && "PhysicsSystem::createComponent - owner is null");

    // Use the GameObject template system so the component is properly registered
    // and m_gameobject is set before initialize() reads the transform.
    PhysicsComponent* comp = owner->createComponent<PhysicsComponent>();
    comp->initialize(type);

    m_components.push_back(comp);
    return comp;
}

void PhysicsSystem::untrack(PhysicsComponent* component)
{
    auto it = std::find(m_components.begin(), m_components.end(), component);
    if (it != m_components.end())
        m_components.erase(it);
}

void PhysicsSystem::release()
{
    if (!m_initialized) return;

    // Components are owned by their GameObjects — only clear the tracking list
    m_components.clear();

    if (m_physicsWorld)
    {
        m_physicsCommon.destroyPhysicsWorld(m_physicsWorld);
        m_physicsWorld = nullptr;
    }

    m_initialized = false;
    m_accumulator = 0.0f;
}
