#pragma once

class LogicEngine;
class RichLabelVessel;

class MobSelectionUI
{
public:
	MobSelectionUI(LogicEngine &engine_p);
	~MobSelectionUI();

	void update();
protected:
	LogicEngine &_engine;

	RichLabelVessel * _label;
};
