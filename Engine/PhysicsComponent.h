#pragma once
#include "Component.h"
#include "Vector3D.h"
#include <reactphysics3d/reactphysics3d.h>

// ---------------------------------------------------------------------------
// PhysicsComponent
//   Wraps a ReactPhysics3D RigidBody and attaches it to a GameObject.
//   Must be created via PhysicsSystem::createComponent() so that initialize()
//   is called after the template system sets m_gameobject.
// ---------------------------------------------------------------------------
class PhysicsComponent : public Component
{
public:
    enum class BodyType { STATIC, KINEMATIC, DYNAMIC };

    PhysicsComponent();
    ~PhysicsComponent() override;

    // Called by PhysicsSystem::createComponent() after m_gameobject is wired up
    void initialize(BodyType type = BodyType::DYNAMIC);

    // Called each fixed-step frame by PhysicsSystem::update()
    void update(float deltaTime);

    // --- Body properties ---
    void setBodyType(BodyType type);
    void setMass(float mass);
    void enableGravity(bool enable);
    void setLinearVelocity(const Vector3D& velocity);
    void setAngularVelocity(const Vector3D& velocity);
    void applyWorldForceAtCenter(const Vector3D& force);
    void applyWorldTorque(const Vector3D& torque);

    // Syncs the owner GameObject's current transform into the rigid body.
    // Call before entering Play mode to avoid a snap on the first frame.
    void resetToOwnerTransform();

    // --- Collider helpers (Box only; sphere/capsule added next iteration) ---
    rp3d::Collider* addBoxCollider(const Vector3D& halfExtents);
    rp3d::Collider* addBoxColliderFromScale();   // half-extents = scale * 0.5
    void            resizeBoxCollider(const Vector3D& halfExtents);
    void            syncColliderToScale();        // auto-called when scale changes
    const Vector3D& getColliderHalfExtents() const { return m_last_half_extents; }

    // --- Accessors ---
    rp3d::RigidBody* getRigidBody()        const { return m_rigidBody; }
    BodyType         getBodyType()         const { return m_bodyType; }
    Vector3D         getLinearVelocity()   const;
    Vector3D         getAngularVelocity()  const;

private:
    rp3d::PhysicsWorld* m_physicsWorld      = nullptr;
    rp3d::RigidBody*    m_rigidBody         = nullptr;
    BodyType            m_bodyType          = BodyType::DYNAMIC;

    rp3d::Collider*     m_tracked_collider  = nullptr;
    rp3d::BoxShape*     m_tracked_box_shape = nullptr;
    Vector3D            m_last_half_extents;
    Vector3D            m_last_scale;

    // Coordinate helpers
    static rp3d::Vector3 toRp3d(const Vector3D& v)        { return { v.m_x, v.m_y, v.m_z }; }
    static Vector3D      fromRp3d(const rp3d::Vector3& v) { return { v.x,   v.y,   v.z   }; }
    static Vector3D      quaternionToEuler(const rp3d::Quaternion& q);
};
