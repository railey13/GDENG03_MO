#pragma once
#include "Prerequisites.h"

class Component {
public:
	Component();
	virtual ~Component();

	void release();
private:
	size_t m_typeId = 0;
	GameObject* m_gameobject = nullptr;

	friend class GameObject;
};