#include "GameEngine.h"

GameEngine::GameEngine(std::string const &resourcePath_p)
	: GameMessageHandler(this)
	, _resourceHandler(resourcePath_p)
	, _graphic(this, &_resourceHandler)
{}

GameEngine::~GameEngine() {}
