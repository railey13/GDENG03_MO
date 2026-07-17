#include "Sphere.h"
#include "iostream"
#include <vector>

#define _USE_MATH_DEFINES 
#include <math.h>

Sphere::Sphere(void* shader_byte_code, size_t size_shader) {
	std::vector<vertex> verts;
	std::vector<ui32> indices;

	vertex top;
	top.position = Vector3D(0, radius, 0);
	top.texcoord = Vector2D(0.5f, 0.0f);
	verts.push_back(top);

	f32 phiStep = (f32)M_PI / m_stackCount;
	f32 thetaStep = 2.0f * (f32)M_PI / m_sliceCount;

	for (ui32 i = 1; i <= m_stackCount - 1; i++) {
		f32 phi = i * phiStep;
		for (ui32 j = 0; j <= m_sliceCount; j++) {
			f32 theta = j * thetaStep;

			f32 cosPhi = cosf(phi);
			f32 cosTheta = cosf(theta);

			f32 sinPhi = sinf(phi);
			f32 sinTheta = sinf(theta);

			Vector3D p = Vector3D(
				radius * sinPhi * cosTheta,
				radius * cosPhi,
				radius * sinPhi * sinTheta
			);

			vertex v;
			v.position = p;
			v.texcoord = Vector2D(
				(f32)j / (f32)m_sliceCount,
				(f32)i / (f32)m_stackCount
			);
			verts.push_back(v);
		}
	}

	vertex bottom;
	bottom.position = Vector3D(0, -radius, 0);
	top.texcoord = Vector2D(0.5f, 1.0f);
	verts.push_back(bottom);

	for (ui32 i = 1; i <= m_sliceCount; i++) {
		indices.push_back(0);
		indices.push_back(i+1);
		indices.push_back(i);
	}

	ui32 baseIndex = 1;
	ui32 ringVertexCount = m_sliceCount + 1;	
	
	for (ui32 i = 0; i < m_stackCount - 2; i++) {
		for (ui32 j = 0; j < m_sliceCount; j++) {
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	ui32 southPoleIndex = verts.size() - 1;
	baseIndex = southPoleIndex - ringVertexCount;

	for (ui32 i = 0; i < m_sliceCount; i++) {
		indices.push_back(southPoleIndex);
		indices.push_back(baseIndex + i);
		indices.push_back(baseIndex + i + 1);
	}


	ui32 size_list = (ui32)verts.size();

	ui32 size_index_list = (ui32)indices.size();

	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(verts.data(), sizeof(vertex), size_list, shader_byte_code, size_shader);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(indices.data(), size_index_list);

	constant cc;
	cc.m_time = 0;
	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(constant));

	m_name = "Sphere";
}

Sphere::~Sphere() {

}

void Sphere::update(f32 deltaTime) {

}

void Sphere::draw(VertexShaderPtr vs, PixelShaderPtr ps, Matrix4x4 view, Matrix4x4 proj) {
	constant cc;
	Matrix4x4 temp;

	cc.m_time = 0;

	cc.m_world.setIdentity();

	temp.setIdentity();
	temp.setScale(m_scale);

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationZ(m_rotation.m_z);

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rotation.m_y);

	cc.m_world *= temp;

	temp.setIdentity();
	temp.setRotationX(m_rotation.m_x);

	cc.m_world *= temp;

	temp.setTranslation(m_position);

	cc.m_world *= temp;

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
