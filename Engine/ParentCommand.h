#pragma once
#include "Command.h"
#include "AppWindow.h"
#include "GameObject.h"

class ParentCommand : public Command {
public:
	ParentCommand(GameObject* child, GameObject* parent) : m_child(child), m_newParent(parent) {
		m_oldParent = m_child->getParent();
	}

	// Inherited via Command
	void execute() override {
		if (m_child) {
			m_child->setParent(m_newParent);
		}
	}

	void undo() override {
		m_child->setParent(m_oldParent);
	}

private:
	GameObject* m_child = nullptr;
	GameObject* m_oldParent = nullptr;
	GameObject* m_newParent = nullptr;
};