#pragma once
#include "Command.h"
#include "AppWindow.h"

class SaveTransformCommand : public Command {
public:
	SaveTransformCommand(AppWindow* receiver, GameObject* target) : receiver(receiver), object(target) {

	}
	// Inherited via Command
	
	//Save the transforms
	void execute() override {
		if (receiver) {
			storedPos = receiver->m_selectedGameObject->getTransform()->getPosition();
			storedRot = receiver->m_selectedGameObject->getTransform()->getRotation();
			storedScale = receiver->m_selectedGameObject->getTransform()->getScale();

			//list{ storedPos,storedRot,storedScale } {};

			receiver->m_positions.push_back(storedPos);

		}
	}

	//Get the old transforms
	void undo() override {
		if (receiver) {
			receiver->m_selectedGameObject->getTransform()->setPosition
				(receiver->m_positions[0]);
			//receiver->m_selectedGameObject->getTransform()->setRotation(list[1]);
			//receiver->m_selectedGameObject->getTransform()->setScale(list[2]);*/
		}
	}
private:
	AppWindow* receiver;
	GameObject* object = nullptr;
	
	Vector3D storedPos = Vector3D(0, 0, 0);
	Vector3D storedRot = Vector3D(0, 0, 0);
	Vector3D storedScale = Vector3D(0, 0, 0);
	//Vector3D list[3] = { storedPos,storedRot,storedScale };

};
