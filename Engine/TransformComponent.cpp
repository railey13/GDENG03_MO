#include "TransformComponent.h"

TransformComponent::TransformComponent() {
	updateWorldMatrix();
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

void TransformComponent::getWorldMatrix(Matrix4x4& mat) {
	mat.setMatrix(m_worldMatrix);
}

void TransformComponent::updateWorldMatrix() {
	Matrix4x4 temp;

	m_worldMatrix.setIdentity();

	temp.setIdentity();
	temp.setScale(m_scale);
	m_worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationX(m_rotation.m_x);
	m_worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rotation.m_y);
	m_worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(m_rotation.m_z);
	m_worldMatrix *= temp;

	temp.setIdentity();
	temp.setTranslation(m_position);
	m_worldMatrix *= temp;
}
