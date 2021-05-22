#pragma once

/// @brief This class is to be extended and given in ButtonData
/// to activate code when the button is pressed
class Listener
{
public:
	~Listener() {}

	virtual void run() = 0;
};
