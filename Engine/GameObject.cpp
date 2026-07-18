#include "GameObject.h"

GameObject::GameObject() : m_name("GameObject") {
	m_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/white.png");
}

GameObject::~GameObject() {

}

void GameObject::setPosition(const Vector3D& position) {
	m_position = position;
}

void GameObject::setRotation(const Vector3D& rotation) {
	m_rotation = rotation;
}

void GameObject::setScale(const Vector3D& scale) {
	m_scale = scale;
}

void GameObject::setTexture(TexturePtr tex) {
	m_tex = tex;
}

