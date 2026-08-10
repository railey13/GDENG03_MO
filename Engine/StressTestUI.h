#pragma once
#include "UI.h"

class StressTestUI : public UI {
public:
	StressTestUI();
	~StressTestUI();

	// Inherited via UI
	void draw() override;
};
