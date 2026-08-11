#include "Bunny.h"
#include "iostream"

Bunny::Bunny() {
	RenderSystem* render = GraphicsEngine::get()->getRenderSystem();

	mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets/Meshes/bunny.obj");

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	constant cc;
	cc.m_time = 0;
	m_cb = render->createConstantBuffer(&cc, sizeof(constant));

	m_name = "Bunny";
	m_gameObjectType = GameObjectTypes::BUNNY;
}

Bunny::~Bunny() {

}

void Bunny::update(f32 deltaTime) {

}

void Bunny::draw(VertexShaderPtr vs, PixelShaderPtr ps, Matrix4x4 view, Matrix4x4 proj) {
	constant cc;
	Matrix4x4 temp;

	cc.m_time = 0;

	getTransform()->getWorldMatrix(cc.m_world);

	cc.m_view = view;
	cc.m_proj = proj;

	RenderSystem* render = GraphicsEngine::get()->getRenderSystem();

	m_cb->update(render->getImmediateDeviceContext(), &cc);

	DeviceContextPtr context = render->getImmediateDeviceContext();

	context->setConstantBuffer(m_cb);

	TextureComponent* tex = getComponent<TextureComponent>();

	if (!tex)
		context->setTexture(ps, m_tex);
	else
		context->setTexture(ps, tex->getTexture());

	context->setVertexBuffer(mesh->getVertexBuffer());
	context->setIndexBuffer(mesh->getIndexBuffer());

	context->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
}
