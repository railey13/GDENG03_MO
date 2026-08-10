#pragma once
#include "GameObject.h"

class Sphere : public GameObject {
public:
	Sphere(void* shader_byte_code, size_t size_shader);
	~Sphere();
public:
	// Inherited via GameObject
	void update(f32 deltaTime) override;
	void draw(VertexShaderPtr vs, PixelShaderPtr ps, Matrix4x4 view, Matrix4x4 proj) override;
private:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	ui32 m_stackCount = 16;
	ui32 m_sliceCount = 24;
	f32 radius = 0.3f;
};