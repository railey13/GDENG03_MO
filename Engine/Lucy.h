#pragma once
#include "GameObject.h"
#include "Mesh.h"
class Lucy : public GameObject {
public:
	Lucy();
	~Lucy();
public:
	// Inherited via GameObject
	void update(f32 deltaTime) override;
	void draw(VertexShaderPtr vs, PixelShaderPtr ps, Matrix4x4 view, Matrix4x4 proj) override;
private:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;

	MeshPtr mesh;
};

