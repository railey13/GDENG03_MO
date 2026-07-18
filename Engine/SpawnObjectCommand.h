#pragma once
#include "Command.h"
#include "AppWindow.h"

class SpawnObjectCommand : public Command {
public:
	SpawnObjectCommand(AppWindow* receiver, GameObjectTypes type) : receiver(receiver), type(type) {

	}
	// Inherited via Command
	void execute() override {
		if (!receiver) return;

		if (object == nullptr) {
			object = receiver->SpawnGameObject(type);
		}
		else {
			receiver->m_objects.push_back(object);
		}
	}

	void undo() override {
		if (object) {
			receiver->RemoveObject(object);
		}
	}

private:
	AppWindow* receiver;
	GameObject* object = nullptr;
	GameObjectTypes type;
};