#include "Pot.h"
#include "iostream"

Pot::Pot() {
	RenderSystem* render = GraphicsEngine::get()->getRenderSystem();

	auto tex = createComponent<TextureComponent>();
	tex->setTexturePath("Assets/Textures/brick.png");

	mesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"Assets/Meshes/teapot.obj");

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	constant cc;
	cc.m_time = 0;
	m_cb = render->createConstantBuffer(&cc, sizeof(constant));

	m_name = "Pot";
	m_gameObjectType = GameObjectTypes::POT;
}

Pot::~Pot() {

}

void Pot::update(f32 deltaTime) {

}

void Pot::draw(VertexShaderPtr vs, PixelShaderPtr ps, Matrix4x4 view, Matrix4x4 proj) {
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

	context->setVertexBuffer(mesh->getVertexBuffer());
	context->setIndexBuffer(mesh->getIndexBuffer());

	context->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
}
