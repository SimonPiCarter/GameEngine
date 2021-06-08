#pragma once

class LogicEngine;
class RichLabel;

class HeaderUI
{
public:
	HeaderUI(LogicEngine &engine_p);
	~HeaderUI();

	void update(double elapsedTime_p);
protected:
	LogicEngine &_engine;

	RichLabel * _labelLife;
	RichLabel * _labelTime;
	RichLabel * _labelResource;

	double _elapsedSecond;
};
