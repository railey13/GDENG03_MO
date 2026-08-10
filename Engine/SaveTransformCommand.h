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

			receiver->m_positions.push_back(storedPos);
			receiver->m_rotations.push_back(storedRot);
			receiver->m_scales.push_back(storedScale);

		}
	}

	//Get the old transforms
	void undo() override {
		if (receiver) {
			receiver->m_selectedGameObject->getTransform()->setPosition
				(receiver->m_positions[0]);
			receiver->m_selectedGameObject->getTransform()->setRotation
				(receiver->m_rotations[0]);
			receiver->m_selectedGameObject->getTransform()->setScale
			(receiver->m_scales[0]);
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
