#pragma once
#include "Command.h"
#include "AppWindow.h"
#include "CameraHandler.h"

class DeleteObjectCommand : public Command {
public:
	DeleteObjectCommand(AppWindow* receiver, GameObject* target) : receiver(receiver), object(target){

	}
	// Inherited via Command
	void execute() override {
		if (receiver && object) {
			std::vector<GameObject*> objects = receiver->getGameObjects();

			for (ui32 i = 0; i < objects.size(); i++) {
				if (objects[i] == object) {
					m_index = i;
					break;
				}
			}

			receiver->RemoveObject(object);
			m_inScene = false;

			if (receiver->m_selectedGameObject == object) {
				receiver->m_selectedGameObject = nullptr;
			}
		}
	}

	void undo() override {
		if (receiver && object) {
			if (m_index >= receiver->m_objects.size()) {
				receiver->m_objects.push_back(object);
			}
			else {
				receiver->m_objects.insert(receiver->m_objects.begin() + m_index, object);
			}
			object->setActive(true);

			if (GameCamera* cam = dynamic_cast<GameCamera*>(object)) {
				CameraHandler::get()->setGameCamera(cam);
				receiver->gamecamera = true;
			}
			
			m_inScene = true;
		}
	}

	~DeleteObjectCommand() override {
		if (object && !m_inScene) {
			delete object;
		}
	}
private:
	AppWindow* receiver;
	GameObject* object = nullptr;
	bool m_inScene = false;
	size_t m_index = 0;
};