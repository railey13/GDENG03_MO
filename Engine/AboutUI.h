#pragma once
#include "UI.h"

class AboutUI : public UI {
public:
	AboutUI();
	~AboutUI();

	// Inherited via UI
	void draw() override; 

};

