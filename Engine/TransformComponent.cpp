#include "TransformComponent.h"
#include "GameObject.h"

TransformComponent::TransformComponent() {
	/*updateWorldMatrix();*/
}

TransformComponent::~TransformComponent() {

}

void TransformComponent::setPosition(const Vector3D& position) {
	m_position = position;
	updateWorldMatrix();
}

void TransformComponent::setRotation(const Vector3D& rotation) {
	m_rotation = rotation;
	updateWorldMatrix();
}

void TransformComponent::setScale(const Vector3D& scale) {
	m_scale = scale;
	updateWorldMatrix();
}

void TransformComponent::updateWorldMatrix() {
	Matrix4x4 temp;

	m_localMatrix.setIdentity();

	temp.setIdentity();
	temp.setScale(m_scale);
	m_localMatrix *= temp;

	temp.setIdentity();
	temp.setRotationX(m_rotation.m_x);
	m_localMatrix *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rotation.m_y);
	m_localMatrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(m_rotation.m_z);
	m_localMatrix *= temp;

	temp.setIdentity();
	temp.setTranslation(m_position);
	m_localMatrix *= temp;

	m_worldMatrix.setMatrix(m_localMatrix);

	GameObject* parent = getGameObject()->getParent();
	if (parent) {
		Matrix4x4 p_matrix;

		parent->getTransform()->getWorldMatrix(p_matrix);

		m_worldMatrix *= p_matrix;
	}

	for (auto c : getGameObject()->getChildren()) {
		c->getTransform()->updateWorldMatrix();
	}
}

Vector3D TransformComponent::getWorldScale() {
	Vector3D x = m_worldMatrix.getXDirection();
	Vector3D y = m_worldMatrix.getYDirection();
	Vector3D z = m_worldMatrix.getZDirection();
	return Vector3D(x.magnitude(), y.magnitude(), z.magnitude());
}

Vector3D TransformComponent::getForwardDirection() {
	Matrix4x4 world;
	getWorldMatrix(world);
	return world.getZDirection();
}
