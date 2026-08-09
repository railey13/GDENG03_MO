#pragma once
#include "Prerequisites.h"

class Component {
public:
	Component();
	virtual ~Component();

	void release();

	GameObject* getGameObject() { return m_gameobject; }
	bool isActive() const { return m_active; }

	virtual void setActive(bool active);
private:
	size_t m_typeId = 0;
	GameObject* m_gameobject = nullptr;
	bool m_active = true;

	friend class GameObject;
};