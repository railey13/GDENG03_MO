#include "Component.h"
#include "GameObject.h"

Component::Component() {

}

Component::~Component() {

}

void Component::release() {
	m_gameobject->removeComponent(m_typeId);
}