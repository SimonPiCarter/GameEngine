#pragma once

#include "engine/GameEngine.h"
#include "logic/layout/WaveLayout.h"

class LogicEngine;
class WaveGenerator;

class CellsEngine : public GameEngine
{
public:
	CellsEngine(std::string const &path_p);
	~CellsEngine();

	virtual void init() override;
	virtual void run() override;

	void runGraphic();
	void runLogic();

	GraphicEngine & getGraphic() { return _graphic; }

	virtual void visitSDLEvent(SDLEventGameMessage const &msg_p) override;

	WaveGenerator * getWaveGenerator() { return _waveGenerator; }

protected:
	LogicEngine *_logic;
	WaveGenerator * _waveGenerator;
};
