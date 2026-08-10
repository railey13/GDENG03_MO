#pragma once
#include <vector>
#include <reactphysics3d/reactphysics3d.h>
#include "PhysicsComponent.h"

class GameObject;

// ---------------------------------------------------------------------------
// PhysicsSystem  (Meyer's Singleton)
//   Owns the ReactPhysics3D PhysicsCommon and PhysicsWorld.
//   Manages a tracking list of all active PhysicsComponents and steps the
//   simulation each frame at a fixed 60 Hz timestep.
//
//   NOTE: PhysicsComponents are owned by the GameObject component map.
//         This system only tracks (non-owning) pointers.
// ---------------------------------------------------------------------------
class PhysicsSystem
{
public:
    static PhysicsSystem* get();

    void initialize(const rp3d::Vector3& gravity = rp3d::Vector3(0.0f, -9.81f, 0.0f));
    void update(float deltaTime);
    void release();

    // Creates and initialises a PhysicsComponent on 'owner' via createComponent<>()
    PhysicsComponent* createComponent(GameObject* owner,
                                      PhysicsComponent::BodyType type
                                          = PhysicsComponent::BodyType::DYNAMIC);

    // Removes the component from the tracking list without deleting it
    // (deletion is handled by the GameObject's component map)
    void untrack(PhysicsComponent* component);

    rp3d::PhysicsWorld*  getPhysicsWorld()   const { return m_physicsWorld; }
    rp3d::PhysicsCommon& getPhysicsCommon()        { return m_physicsCommon; }
    bool                 isInitialized()     const { return m_initialized; }

private:
    PhysicsSystem()  = default;
    ~PhysicsSystem() = default;
    PhysicsSystem(const PhysicsSystem&)            = delete;
    PhysicsSystem& operator=(const PhysicsSystem&) = delete;

    rp3d::PhysicsCommon             m_physicsCommon;
    rp3d::PhysicsWorld*             m_physicsWorld = nullptr;
    std::vector<PhysicsComponent*>  m_components;          // non-owning
    bool                            m_initialized  = false;

    float m_accumulator = 0.0f;
    static constexpr float FIXED_STEP = 1.0f / 60.0f;
};
