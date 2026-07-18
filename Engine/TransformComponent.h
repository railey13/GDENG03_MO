#pragma once
#include "Prerequisites.h"
#include "Component.h"
#include "Vector3D.h"
#include "Matrix4x4.h"

class TransformComponent : public Component {
public:
	TransformComponent();
	~TransformComponent();
public:
	void setPosition(const Vector3D& position);

	void setRotation(const Vector3D& rotation);

	void setScale(const Vector3D& scale);

	void getWorldMatrix(Matrix4x4& mat);
protected:
	void updateWorldMatrix();
public:
	Vector3D m_position;
	Vector3D m_rotation;
	Vector3D m_scale = Vector3D(1, 1, 1);
private:
	Matrix4x4 m_worldMatrix;
};

