#include "Capsule.h"
#include "iostream"
#include <vector>

#define _USE_MATH_DEFINES 
#include <math.h>

Capsule::Capsule(void* shader_byte_code, size_t size_shader) {
	std::vector<vertex> verts;
	std::vector<ui32> indices;

	ui32 sliceCount = 24;
	ui32 capStackCount = 8;
	f32 radius = 0.15f;
	f32 cylinderHeight = 0.3f;
	f32 halfHeight = cylinderHeight * 0.5f;

	f32 thetaStep = 2.0f * (f32)M_PI / sliceCount;

	vertex top;
	top.position = Vector3D(0, halfHeight + radius, 0);
	top.texcoord = Vector2D(0.5f, 0.0f);
	verts.push_back(top);

	for (ui32 i = 1; i <= capStackCount; i++) {
		f32 phi = (f32)i / (f32)capStackCount * ((f32)M_PI * 0.5f);
		f32 y = halfHeight + radius * cosf(phi);
		f32 ringRadius = radius * sinf(phi);

		for (ui32 j = 0; j <= sliceCount; j++) {
			f32 theta = j * thetaStep;
			Vector3D p = Vector3D(
				ringRadius * cosf(theta),
				y,
				ringRadius * sinf(theta)
			);
			vertex v;
			v.position = p;
			v.texcoord = Vector2D((f32)j / sliceCount, (f32)i / (f32)(capStackCount * 2 + 2));
			verts.push_back(v);
		}
	}

	for (ui32 j = 0; j <= sliceCount; j++) {
		f32 theta = j * thetaStep;
		Vector3D p = Vector3D(radius * cosf(theta), -halfHeight, radius * sinf(theta));
		vertex v;
		v.position = p;
		v.texcoord = Vector2D((f32)j / sliceCount, 0.5f);
		verts.push_back(v);
	}

	for (ui32 i = 1; i < capStackCount; i++) {
		f32 phi = (f32)M_PI * 0.5f + (f32)i / (f32)capStackCount * ((f32)M_PI * 0.5f);
		f32 y = -halfHeight + radius * cosf(phi);
		f32 ringRadius = radius * sinf(phi);

		for (ui32 j = 0; j <= sliceCount; j++) {
			f32 theta = j * thetaStep;
			Vector3D p = Vector3D(
				ringRadius * cosf(theta),
				y,
				ringRadius * sinf(theta)
			);
			vertex v;
			v.position = p;
			v.texcoord = Vector2D((f32)j / sliceCount, 0.5f + (f32)i / (f32)(capStackCount * 2));
			verts.push_back(v);
		}
	}

	vertex bottom;
	bottom.position = Vector3D(0, -halfHeight - radius, 0);
	bottom.texcoord = Vector2D(0.5f, 1.0f);
	verts.push_back(bottom);

	ui32 ringVertexCount = sliceCount + 1;
	ui32 ringCount = capStackCount + 1 + (capStackCount - 1);

	for (ui32 i = 1; i <= sliceCount; i++) {
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	ui32 baseIndex = 1;
	for (ui32 i = 0; i < ringCount - 1; i++) {
		for (ui32 j = 0; j < sliceCount; j++) {
			indices.push_back(baseIndex + i * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices.push_back(baseIndex + i * ringVertexCount + j + 1);
			indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
		}
	}

	ui32 southPoleIndex = (ui32)verts.size() - 1;
	baseIndex = southPoleIndex - ringVertexCount;
	for (ui32 i = 0; i < sliceCount; i++) {
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

	m_name = "Capsule";
}

Capsule::~Capsule() {

}

void Capsule::update(f32 deltaTime) {

}

void Capsule::draw(VertexShaderPtr vs, PixelShaderPtr ps, Matrix4x4 view, Matrix4x4 proj) {
	constant cc;
	Matrix4x4 temp;

	cc.m_time = 0;

	getTransform()->getWorldMatrix(cc.m_world);

	cc.m_view = view;
	cc.m_proj = proj;

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);

	DeviceContextPtr context = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();

	context->setConstantBuffer(m_cb);

	TextureComponent* tex = getComponent<TextureComponent>();

	if (!tex)
		context->setTexture(ps, m_tex);
	else
		context->setTexture(ps, tex->getTexture());

	context->setVertexBuffer(m_vb);
	context->setIndexBuffer(m_ib);

	context->drawIndexedTriangleList(m_ib->getSizeIndexList(), 0, 0);
}
