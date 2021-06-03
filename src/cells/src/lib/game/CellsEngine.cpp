#include "CellsEngine.h"

#include "OgreWindow.h"
#include "OgreSceneManager.h"
#include <thread>

CellsEngine::CellsEngine(std::string const &path_p)
	: GameEngine(path_p)
{}


void CellsEngine::init()
{
	_graphic.initWindow("cells");
}

void CellsEngine::run()
{
	std::thread logicThread_l(&CellsEngine::runLogic, this);
	runGraphic();
	logicThread_l.join();
}

void CellsEngine::runGraphic()
{
	std::chrono::time_point<std::chrono::system_clock> start_l = std::chrono::system_clock::now();
	double timeSinceLast_l = 1.0 / 60.0;

	while( !_graphic.getQuit() )
	{
		if( !_graphic.getRenderWindow()->isVisible() )
		{
			//Don't burn CPU cycles unnecessary when we're minimized.
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		_graphic.getSceneManager()->updateAllAnimations();

		// handle frame
		_graphic.handleFrame(timeSinceLast_l);

		std::chrono::time_point<std::chrono::system_clock> end_l = std::chrono::system_clock::now();
		timeSinceLast_l = (end_l - start_l) / std::chrono::milliseconds(1);
		timeSinceLast_l = std::min( 1.0, timeSinceLast_l/1000. ); //Prevent from going haywire.
		start_l = end_l;
	}
}

void CellsEngine::runLogic()
{
	std::chrono::time_point<std::chrono::system_clock> start_l = std::chrono::system_clock::now();

	double timeSinceLast_l = 1.0 / 60.0;

	while( !_graphic.getQuit() )
	{
		// handle game message
		MessageHandler<GameMessage>::handleAllMessages();

		if( !_graphic.getRenderWindow()->isVisible() )
		{
			//Don't burn CPU cycles unnecessary when we're minimized.
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		const std::chrono::time_point<std::chrono::system_clock> end_l = std::chrono::system_clock::now();

		timeSinceLast_l = (end_l - start_l) / std::chrono::milliseconds(1);
		timeSinceLast_l = std::min( 1.0, timeSinceLast_l/1000. ); //Prevent from going haywire.
		start_l = end_l;
		//Don't burn CPU cycles too fast
		if(timeSinceLast_l < 50)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(50-long(timeSinceLast_l)));
		}
	}
}
void CellsEngine::visitSDLEvent(SDLEventGameMessage const &msg_p)
{
	SDL_Event const &evt(msg_p.getEvent());
	switch( evt.type )
	{
	case SDL_KEYDOWN:
		break;
	case SDL_KEYUP:
		if(evt.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			_graphic.setQuit();
		}
		break;
	case SDL_QUIT:
		_graphic.setQuit();
		break;
	default:
		break;
	}
}
