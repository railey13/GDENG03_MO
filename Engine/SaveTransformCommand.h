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

			receiver->m_selectedGameObject->m_positions.push_back(storedPos);
			receiver->m_selectedGameObject->m_rotations.push_back(storedRot);
			receiver->m_selectedGameObject->m_scales.push_back(storedScale);

		}
	}

	//Get the old transforms
	void undo() override {

		//if the counterIndex is less than 5, add it
		if (receiver->m_selectedGameObject->counterIndex <= 5)
		{
			//coutnerIndex increment
			receiver->m_selectedGameObject->counterIndex++;
		}


		//if the counterIndex is larger than the current size
		if (receiver->m_selectedGameObject->counterIndex > receiver->m_selectedGameObject->m_positions.size())
		{
			//decrease it
			receiver->m_selectedGameObject->counterIndex--;
		}

		//Apply changes
		
		if (receiver) {
				receiver->m_selectedGameObject->getTransform()->setPosition
				(receiver->m_selectedGameObject->m_positions[receiver->m_selectedGameObject->m_positions.size() - receiver->m_selectedGameObject->counterIndex]);

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
