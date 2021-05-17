#include "GameEngine.h"

#include "OgreRenderWindow.h"

GameEngine::GameEngine()
	: GameMessageHandler(this)
	, _resourceHandler()
	, _graphic(this, &_resourceHandler)
{}

GameEngine::~GameEngine() {}
