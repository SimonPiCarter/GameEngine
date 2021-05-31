#pragma once

class WaveEngine;

class LogicEngine
{
public:
	LogicEngine();

	void run();
protected:
	bool _quit;

	/// @brief tandem class can access logic data
	friend class WaveEngine;
};
