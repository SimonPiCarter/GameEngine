#pragma once

class LogicEngine;
class RichLabelVessel;

class HeaderUI
{
public:
	HeaderUI(LogicEngine &engine_p);
	~HeaderUI();

	void update();
protected:
	LogicEngine &_engine;

	RichLabelVessel * _labelLife;
	RichLabelVessel * _labelTime;
	RichLabelVessel * _labelResource;
};
