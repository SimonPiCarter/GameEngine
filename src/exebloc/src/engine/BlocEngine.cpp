#include "BlocEngine.h"

#include "OgreWindow.h"
#include "OgreSceneManager.h"
#include <thread>

#include "bloc/Bloc.h"
#include "bloc/BlocModel.h"

BlocEngine::BlocEngine(std::string const &path_p)
	: GameEngine(path_p)
	, BlocMessageHandler(this)
	, _map(this)
{}

void BlocEngine::init()
{
	_resourceHandler.addResource({"Cube", "Cube.mesh", false, ""});
	_resourceHandler.addResource({"CubeGreen", "Cube.mesh", false, "Green"});
	_resourceHandler.addResource({"CubeYellow", "Cube.mesh", false, "Yellow"});
	_resourceHandler.addResource({"CubeRed", "Cube.mesh", false, "Red"});
	_graphic.initWindow("demo");
}
#include <iostream>

void BlocEngine::run()
{
	if(_graphic.getQuit())
	{
		//User cancelled config
		_graphic.tearDown();
	}

	std::vector<InfoLabel> content_l;
	content_l.push_back({"F5", 255, 255, 0});
	content_l.push_back({" : Menu\n", 255, 255, 255});
	RichLabel * label_l = new RichLabel(content_l, 20, 20, 400, 50, 10, _graphic);

	Ogre::Window *renderWindow_l = _graphic.getRenderWindow();

	GraphicEntity light_l;

	std::array<std::array<std::array<bool, 3>, 3>, 3> form_l =
	{{
		{{ {false, false, false}, {false, false, false}, {false, true, true} }},
		{{ {false, true, false}, {false, true, false}, {false, true, false} }},
		{{ {true, true, false}, {false, false, false}, {false, false, false} }}
	}};
	BlocModel model_l(form_l, "CubeYellow");
	this->BlocMessageHandler::registerMessage(new SpawnBlocMessage(model_l, {0,0,0}));

	_graphic.registerMessage(new NewLightMessage(&light_l, "root", {1., 1., 1.}, {-1, -1, -1}, LightType::Directional));

	std::chrono::time_point<std::chrono::system_clock> start_l = std::chrono::system_clock::now();

	double timeSinceLast_l = 1.0 / 60.0;

	while( !_graphic.getQuit() )
	{
		// handle game message
		MessageHandler<BlocMessage>::handleAllMessages();
		MessageHandler<GameMessage>::handleAllMessages();

		_graphic.getSceneManager()->updateAllAnimations();

		// handle frame
		_graphic.handleFrame(timeSinceLast_l);

		if( !renderWindow_l->isVisible() )
		{
			//Don't burn CPU cycles unnecessary when we're minimized.
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		double move_l = timeSinceLast_l;
		// Update bloc
		if(_currentBloc)
		{
			// update freefall movement of bloc
			_currentBloc->updateLevel(_currentBloc->getLevel()+move_l);
			_graphic.registerMessage(new MoveSceneMessage("currentBloc", {0.,0.,move_l}));

			// check for freeze
			if(_map.checkFreeze(_currentBloc))
			{
				// adjust back bloc
				_graphic.registerMessage(new MoveSceneMessage("currentBloc", {0.,0.,_currentBloc->getPosition()[2] - 1 - _currentBloc->getLevel()}));
				_currentBloc->updateLevel(_currentBloc->getPosition()[2] - 1);

				// Freeze current bloc
				this->BlocMessageHandler::registerMessage(new FreezeBlocMessage(_currentBloc));
				_currentBloc = nullptr;
				// Spawn next one
				this->BlocMessageHandler::registerMessage(new SpawnBlocMessage(model_l, {0,0,0}));
			}
		}
		if(_map.checkLose())
		{
			// End game
		}

		const std::chrono::time_point<std::chrono::system_clock> end_l = std::chrono::system_clock::now();

		timeSinceLast_l = (end_l - start_l) / std::chrono::milliseconds(1);
		timeSinceLast_l = std::min( 1.0, timeSinceLast_l/1000. ); //Prevent from going haywire.
		start_l = end_l;
	}

	_graphic.tearDown();
}

void BlocEngine::visitSDLEvent(SDLEventGameMessage const &msg_p)
{
	SDL_Event const &evt(msg_p.getEvent());
	switch( evt.type )
	{
	case SDL_KEYUP:
		if(evt.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			_graphic.setQuit();
		}
		if (evt.key.keysym.scancode == SDL_SCANCODE_Q)
		{
			// NA
		}
		break;
	case SDL_QUIT:
		_graphic.setQuit();
		break;
	default:
		break;
	}
}
