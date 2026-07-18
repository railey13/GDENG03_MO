#pragma once
#include "UI.h"

class MainMenuBarUI : public UI{
public:
	MainMenuBarUI();
	~MainMenuBarUI();
	
	// Inherited via UI
	void draw() override;
private:
	friend class SpawnObjectCommand;
};

