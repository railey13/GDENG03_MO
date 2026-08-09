#include "PhysicsComponent.h"
#include "PhysicsSystem.h"
#include "GameObject.h"
#include "Plane.h"
#include <cmath>
#include <cassert>

// ---------------------------------------------------------------------------
PhysicsComponent::PhysicsComponent()
    : m_physicsWorld(nullptr), m_rigidBody(nullptr), m_bodyType(BodyType::DYNAMIC)
{
}

PhysicsComponent::~PhysicsComponent()
{
    // Self-deregister from the tracking list FIRST (before the world goes away)
    PhysicsSystem::get()->untrack(this);

    // Destroy the rigid body if the world is still alive
    if (m_rigidBody && m_physicsWorld && PhysicsSystem::get()->isInitialized())
    {
        m_physicsWorld->destroyRigidBody(m_rigidBody);
        m_rigidBody = nullptr;
    }
}

void PhysicsComponent::initialize(BodyType type)
{
    assert(getGameObject()                   && "PhysicsComponent::initialize - m_gameobject is null");
    assert(PhysicsSystem::get()->isInitialized() && "Call PhysicsSystem::initialize() first");

    m_physicsWorld = PhysicsSystem::get()->getPhysicsWorld();
    m_bodyType     = type;

    // Build an initial transform from the owner's TransformComponent
    Vector3D pos = getGameObject()->getTransform()->getPosition();
    rp3d::Transform tf(toRp3d(pos), rp3d::Quaternion::identity());
    m_rigidBody = m_physicsWorld->createRigidBody(tf);

    resetToOwnerTransform(); // also syncs rotation
    setBodyType(type);
}

// ---------------------------------------------------------------------------
void PhysicsComponent::update(float /*deltaTime*/)
{
    if (!m_rigidBody || !getGameObject()) return;

    // --- Sync scale changes → collider shape ---
    Vector3D curScale = getGameObject()->getTransform()->getScale();
    if (m_tracked_collider &&
        (curScale.m_x != m_last_scale.m_x ||
         curScale.m_y != m_last_scale.m_y ||
         curScale.m_z != m_last_scale.m_z))
    {
        syncColliderToScale();
    }

    // Static bodies never move — nothing more to do
    if (m_bodyType == BodyType::STATIC) return;

    // --- Push physics transform → TransformComponent ---
    const rp3d::Transform& tf = m_rigidBody->getTransform();
    getGameObject()->getTransform()->setPosition(fromRp3d(tf.getPosition()));
    getGameObject()->getTransform()->setRotation(quaternionToEuler(tf.getOrientation()));
}

// ---------------------------------------------------------------------------
void PhysicsComponent::setBodyType(BodyType type)
{
    m_bodyType = type;
    if (!m_rigidBody) return;
    switch (type)
    {
    case BodyType::STATIC:    m_rigidBody->setType(rp3d::BodyType::STATIC);    break;
    case BodyType::KINEMATIC: m_rigidBody->setType(rp3d::BodyType::KINEMATIC); break;
    case BodyType::DYNAMIC:   m_rigidBody->setType(rp3d::BodyType::DYNAMIC);   break;
    }
}

void PhysicsComponent::setMass(float mass)
{
    if (m_rigidBody) m_rigidBody->setMass(mass);
}

void PhysicsComponent::enableGravity(bool enable)
{
    if (m_rigidBody) m_rigidBody->enableGravity(enable);
}

void PhysicsComponent::setLinearVelocity(const Vector3D& velocity)
{
    if (m_rigidBody) m_rigidBody->setLinearVelocity(toRp3d(velocity));
}

void PhysicsComponent::setAngularVelocity(const Vector3D& velocity)
{
    if (m_rigidBody) m_rigidBody->setAngularVelocity(toRp3d(velocity));
}

void PhysicsComponent::applyWorldForceAtCenter(const Vector3D& force)
{
    if (m_rigidBody) m_rigidBody->applyWorldForceAtCenterOfMass(toRp3d(force));
}

void PhysicsComponent::applyWorldTorque(const Vector3D& torque)
{
    if (m_rigidBody) m_rigidBody->applyWorldTorque(toRp3d(torque));
}

// ---------------------------------------------------------------------------
void PhysicsComponent::resetToOwnerTransform()
{
    if (!m_rigidBody || !getGameObject()) return;

    Vector3D pos = getGameObject()->getTransform()->getPosition();
    Vector3D rot = getGameObject()->getTransform()->getRotation();

    // Convert intrinsic XYZ Euler angles to quaternion: q = qZ * qY * qX
    float cx = std::cosf(rot.m_x * 0.5f), sx = std::sinf(rot.m_x * 0.5f);
    float cy = std::cosf(rot.m_y * 0.5f), sy = std::sinf(rot.m_y * 0.5f);
    float cz = std::cosf(rot.m_z * 0.5f), sz = std::sinf(rot.m_z * 0.5f);

    rp3d::Quaternion qx(sx, 0.0f, 0.0f, cx);
    rp3d::Quaternion qy(0.0f, sy, 0.0f, cy);
    rp3d::Quaternion qz(0.0f, 0.0f, sz, cz);
    rp3d::Quaternion orient = qz * qy * qx;
    orient.normalize();

    rp3d::Transform tf(toRp3d(pos), orient);
    m_rigidBody->setTransform(tf);
    m_rigidBody->setLinearVelocity(rp3d::Vector3(0.0f, 0.0f, 0.0f));
    m_rigidBody->setAngularVelocity(rp3d::Vector3(0.0f, 0.0f, 0.0f));
}

// ---------------------------------------------------------------------------
rp3d::Collider* PhysicsComponent::addBoxCollider(const Vector3D& halfExtents)
{
    if (!m_rigidBody) return nullptr;

    Vector3D clamped = halfExtents;
    if (clamped.m_x < 0.001f) clamped.m_x = 0.001f;
    if (clamped.m_y < 0.001f) clamped.m_y = 0.001f;
    if (clamped.m_z < 0.001f) clamped.m_z = 0.001f;

    rp3d::BoxShape* box = PhysicsSystem::get()->getPhysicsCommon().createBoxShape(toRp3d(clamped));
    rp3d::Collider* col = m_rigidBody->addCollider(box, rp3d::Transform::identity());

    m_tracked_box_shape = box;
    m_tracked_collider  = col;
    m_last_half_extents = clamped;
    if (getGameObject()) m_last_scale = getGameObject()->getTransform()->getScale();

    return col;
}

rp3d::Collider* PhysicsComponent::addBoxColliderFromScale()
{
    if (!getGameObject()) return nullptr;
    Vector3D s = getGameObject()->getTransform()->getScale();

    if (dynamic_cast<Plane*>(getGameObject()))
    {
        // Plane mesh vertices span [-1, 1] in X and Y, Z=0.
        // Local half-extents are (scale.x, scale.y, 0.05f).
        // With 90 deg X rotation, local Z (0.05f) becomes vertical height in world space.
        return addBoxCollider(Vector3D(s.m_x * 1.0f, s.m_y * 1.0f, 0.05f));
    }
    else
    {
        // Cube mesh vertices span [-0.2, 0.2] in X, Y, Z (half-extent = 0.2).
        return addBoxCollider(Vector3D(s.m_x * 0.2f, s.m_y * 0.2f, s.m_z * 0.2f));
    }
}

void PhysicsComponent::resizeBoxCollider(const Vector3D& halfExtents)
{
    if (!m_rigidBody || !m_tracked_collider || !m_tracked_box_shape) return;

    Vector3D clamped = halfExtents;
    if (clamped.m_x < 0.001f) clamped.m_x = 0.001f;
    if (clamped.m_y < 0.001f) clamped.m_y = 0.001f;
    if (clamped.m_z < 0.001f) clamped.m_z = 0.001f;

    m_rigidBody->removeCollider(m_tracked_collider);
    PhysicsSystem::get()->getPhysicsCommon().destroyBoxShape(m_tracked_box_shape);
    m_tracked_collider  = nullptr;
    m_tracked_box_shape = nullptr;

    rp3d::BoxShape* box = PhysicsSystem::get()->getPhysicsCommon().createBoxShape(toRp3d(clamped));
    m_tracked_collider  = m_rigidBody->addCollider(box, rp3d::Transform::identity());
    m_tracked_box_shape = box;
    m_last_half_extents = clamped;
}

void PhysicsComponent::syncColliderToScale()
{
    if (!getGameObject()) return;
    Vector3D s   = getGameObject()->getTransform()->getScale();
    m_last_scale = s;

    if (dynamic_cast<Plane*>(getGameObject()))
    {
        resizeBoxCollider(Vector3D(s.m_x * 1.0f, s.m_y * 1.0f, 0.05f));
    }
    else
    {
        resizeBoxCollider(Vector3D(s.m_x * 0.2f, s.m_y * 0.2f, s.m_z * 0.2f));
    }
}

// ---------------------------------------------------------------------------
Vector3D PhysicsComponent::getLinearVelocity() const
{
    if (!m_rigidBody) return Vector3D(0, 0, 0);
    return fromRp3d(m_rigidBody->getLinearVelocity());
}

Vector3D PhysicsComponent::getAngularVelocity() const
{
    if (!m_rigidBody) return Vector3D(0, 0, 0);
    return fromRp3d(m_rigidBody->getAngularVelocity());
}

// ---------------------------------------------------------------------------
// Converts an rp3d quaternion to intrinsic XYZ Euler angles (radians),
// matching the rotation order used by TransformComponent::updateWorldMatrix().
Vector3D PhysicsComponent::quaternionToEuler(const rp3d::Quaternion& q)
{
    float x = q.x, y = q.y, z = q.z, w = q.w;

    // Roll — rotation around X
    float sinr_cosp = 2.0f * (w * x + y * z);
    float cosr_cosp = 1.0f - 2.0f * (x * x + y * y);
    float ex = std::atan2f(sinr_cosp, cosr_cosp);

    // Pitch — rotation around Y (clamp for gimbal lock)
    float sinp = 2.0f * (w * y - z * x);
    float ey;
    if (std::fabsf(sinp) >= 1.0f)
        ey = std::copysignf(3.14159265358979f * 0.5f, sinp);
    else
        ey = std::asinf(sinp);

    // Yaw — rotation around Z
    float siny_cosp = 2.0f * (w * z + x * y);
    float cosy_cosp = 1.0f - 2.0f * (y * y + z * z);
    float ez = std::atan2f(siny_cosp, cosy_cosp);

    return Vector3D(ex, ey, ez);
}
