#include "GameObject.h"

GameObject::GameObject() : m_name("GameObject") {
	m_tex = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"Assets/Textures/white.png");
	m_transform = createComponent<TransformComponent>();
}

GameObject::~GameObject() {

}

TransformComponent* GameObject::getTransform() const {
	return m_transform;
}

void GameObject::createComponentInternal(Component* component, size_t id) {
	auto compPtr = std::unique_ptr<Component>(component);
	m_components.emplace(id, std::move(compPtr));
	component->m_typeId = id;
	component->m_gameobject = this;
}

Component* GameObject::getComponentInternal(size_t id) {
	auto it = m_components.find(id);

	if (it == m_components.end()) return nullptr;

	return it->second.get();
}

void GameObject::removeComponent(size_t id) {
	m_components.erase(id);
}

void GameObject::setTexture(TexturePtr tex) {
	m_tex = tex;
}

