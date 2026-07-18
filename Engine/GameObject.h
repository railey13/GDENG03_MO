#pragma once
#include "Prerequisites.h"
#include "Vector3D.h"
#include "Vector2D.h"
#include "Matrix4x4.h"

#include "DeviceContext.h"
#include "GraphicsEngine.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "GameObjectTypes.h"

#include <string>

struct vertex {
	Vector3D position;
	Vector2D texcoord;
};

__declspec(align(16))
struct constant {
	Matrix4x4 m_world;
	Matrix4x4 m_view;
	Matrix4x4 m_proj;
	f32 m_time;
	f32 padding[3];
};

class GameObject {
public:
	GameObject();
	virtual ~GameObject();
public:
	virtual void update(f32 deltaTime) = 0;
	virtual void draw(VertexShaderPtr vs, PixelShaderPtr ps, Matrix4x4 view, Matrix4x4 proj) = 0;
public:
	void setPosition(const Vector3D& position);
	void setRotation(const Vector3D& rotation);
	void setScale(const Vector3D& scale);

	void setTexture(TexturePtr tex);
public:
	Vector3D m_position = Vector3D(0,0,0);
	Vector3D m_rotation;
	Vector3D m_scale = Vector3D(1,1,1);

	std::string m_name;
protected:
	TexturePtr m_tex = nullptr;
};

