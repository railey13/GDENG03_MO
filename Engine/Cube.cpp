#include "Cube.h"
#include "iostream"

Cube::Cube(void* shader_byte_code, size_t size_shader) {
	Vector3D position_list[] = {
		{Vector3D(-0.2f, -0.2f, -0.2f)}, // POS1
		{Vector3D(-0.2f, 0.2f, -0.2f)},	// POS2
		{Vector3D(0.2f, 0.2f, -0.2f)},	// POS3 
		{Vector3D(0.2f, -0.2f, -0.2f)}, // POS4

		{Vector3D(0.2f, -0.2f, 0.2f)},
		{Vector3D(0.2f, 0.2f, 0.2f)},
		{Vector3D(-0.2f, 0.2f, 0.2f)},
		{Vector3D(-0.2f, -0.2f, 0.2f)},
	};

	Vector2D texcoord_list[] = {
		{Vector2D(0.0f, 0.0f)},
		{Vector2D(0.0f, 1.0f)},
		{Vector2D(1.0f, 0.0f)},
		{Vector2D(1.0f, 1.0f)}
	};
	
	
	vertex list[] = {
		// FRONT SIDE
		{position_list[0], texcoord_list[1]},
		{position_list[1], texcoord_list[0]},
		{position_list[2], texcoord_list[2]},
		{position_list[3], texcoord_list[3]},
		// BACK SIDE
		{position_list[4], texcoord_list[1]},
		{position_list[5], texcoord_list[0]},
		{position_list[6], texcoord_list[2]},
		{position_list[7], texcoord_list[3]},
		// TOP SIDE
		{position_list[1], texcoord_list[1]},
		{position_list[6], texcoord_list[0]},
		{position_list[5], texcoord_list[2]},
		{position_list[2], texcoord_list[3]},
		// BOTTOM SIDE
		{position_list[7], texcoord_list[1]},
		{position_list[0], texcoord_list[0]},
		{position_list[3], texcoord_list[2]},
		{position_list[4], texcoord_list[3]},
		// RIGHT SIDE
		{position_list[3], texcoord_list[1]},
		{position_list[2], texcoord_list[0]},
		{position_list[5], texcoord_list[2]},
		{position_list[4], texcoord_list[3]},
		// LEFT SIDE
		{position_list[7], texcoord_list[1]},
		{position_list[6], texcoord_list[0]},
		{position_list[1], texcoord_list[2]},
		{position_list[0], texcoord_list[3]},
	};

	ui32 size_list = (ui32)ARRAYSIZE(list);

	ui32 index_list[] = {
		// FRONT SIDE
		0,1,2, // FIRST TRIANGLE
		2,3,0, // SECOND TRIANGLE
		// BACK SIDE
		4,5,6,
		6,7,4,
		// TOP SIDE
		8,9,10,
		10,11,8,
		// BOTTOM SIDE
		12,13,14,
		14,15,12,
		// RIGHT SIDE
		16,17,18,
		18,19,16,
		// LEFT SIDE
		20,21,22,
		22,23,20
	};

	ui32 size_index_list = (ui32)ARRAYSIZE(index_list);

	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(index_list, size_index_list);

	constant cc;
	cc.m_time = 0;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

	m_name = "Cube";
	m_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/orange.png");
}

Cube::~Cube() {

}

void Cube::update(f32 deltaTime) {

}

void Cube::draw(VertexShaderPtr vs, PixelShaderPtr ps, Matrix4x4 view, Matrix4x4 proj) {
	constant cc;
	Matrix4x4 temp;

	cc.m_time = 0;

	getTransform()->getWorldMatrix(cc.m_world);

	cc.m_view = view;
	cc.m_proj = proj;

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

	DeviceContextPtr context = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();

	context->setConstantBuffer(m_cb);

	context->setTexutre(ps, m_tex);

	context->setVertexBuffer(m_vb);
	context->setIndexBuffer(m_ib);

	context->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
}
