#pragma once
#include "AGameObject.h"
#include "Settings.h"

class GameCamera : public AGameObject{
public:
	GameCamera(void* shader_byte_code, size_t size_shader);
	~GameCamera();
	// Inherited via AGameObject
	void update(f32 deltaTime) override;
	void draw(VertexShaderPtr vs, PixelShaderPtr ps, Matrix4x4 view, Matrix4x4 proj) override;
public:
	Matrix4x4 getViewMatrix() const;
	Matrix4x4 getProjectionMatrix() const;
	Matrix4x4 getRotationMatrix() const;

	Vector3D getForwardDirection() const;

	void setAspect(f32 width, f32 height);
private:
	f32 m_aspect = (f32)Settings::WindowWidth / (f32)Settings::WindowHeight;

	f32 m_fov = 1.57f;
	f32 m_znear = 0.1f;
	f32 m_zfar = 100.0f;
private:
	VertexBufferPtr m_vb;
	IndexBufferPtr  m_ib;
	ConstantBufferPtr m_cb;
};

