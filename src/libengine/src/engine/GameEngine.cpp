#include "GameEngine.h"

GameEngine::GameEngine()
	: GameMessageHandler(this)
	, _resourceHandler()
	, _graphic(this, &_resourceHandler)
{}

GameEngine::~GameEngine() {}
