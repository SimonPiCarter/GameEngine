#include "BlocEngine.h"

#include "OgreWindow.h"
#include "OgreSceneManager.h"
#include <thread>
#include <cstdlib>

#include "bloc/Bloc.h"
#include "bloc/BlocModel.h"
#include "utils/DataReader.h"

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

	std::list<std::list<pattern_t>> data_l = readData(_graphic.getResourcePath()+"/bloc/models.txt");

	std::vector<BlocModel *> models_l;
	std::vector<std::string> resources_l = {"CubeGreen", "CubeYellow", "CubeRed"};

	for(std::list<pattern_t> const & modelData_l : data_l)
	{
		std::vector<BlocForm> forms_l;
		for(pattern_t const & pattern_l : modelData_l)
		{
			forms_l.push_back(BlocForm(pattern_l));
		}
		for(std::string const &res_l : resources_l)
		{
			models_l.push_back(new BlocModel(forms_l, res_l));
		}
	}
	this->BlocMessageHandler::registerMessage(new SpawnBlocMessage(*models_l[0], {5,5,0}));

	_graphic.registerMessage(new NewSceneMessage("main", "root", {-5.,2.,-20.}));
	_graphic.registerMessage(new NewLightMessage(&light_l, "main", {1., 1., 1.}, {-1, -1, -1}, LightType::Directional));

	std::vector<GraphicEntity *> borders_l;
	for(long i = -1 ; i <= 9 ; ++i)
	{
		borders_l.push_back(new GraphicEntity());
		_graphic.registerMessage(new NewGraphicEntityMessage(borders_l.back(), "Cube", {double(i), 5, 19}, {0.5,0.5,0.5}, "main"));
	}
	for(size_t i = 0 ; i < 19 ; ++i)
	{
		borders_l.push_back(new GraphicEntity());
		_graphic.registerMessage(new NewGraphicEntityMessage(borders_l.back(), "Cube", {-1, 5, double(i)}, {0.5,0.5,0.5}, "main"));
		borders_l.push_back(new GraphicEntity());
		_graphic.registerMessage(new NewGraphicEntityMessage(borders_l.back(), "Cube", {9, 5, double(i)}, {0.5,0.5,0.5}, "main"));
	}

	_graphic.registerMessage(new RotateSceneMessage("main", {90, 0, 0}));


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

		double move_l = timeSinceLast_l * (_speed ? 10. : 1.);
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
				_graphic.registerMessage(new MoveSceneMessage(
					"currentBloc", {
						double(_currentBloc->getPosition()[0]),
						double(_currentBloc->getPosition()[1]),
						double(std::min<unsigned long>(MAP_SIZE-1, _currentBloc->getPosition()[2] - 1))}
					, false));
				_currentBloc->updateLevel(_currentBloc->getPosition()[2]);

				// Freeze current bloc
				this->BlocMessageHandler::registerMessage(new FreezeBlocMessage(_currentBloc));
				_currentBloc = nullptr;
				// Spawn next one
				this->BlocMessageHandler::registerMessage(new SpawnBlocMessage(*models_l[rand()%models_l.size()], {5,5,0}));
				// Reset speed
				_speed = false;
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

	for(GraphicEntity* ent_l : borders_l)
	{
		delete ent_l;
	}
	for(BlocModel *model_l : models_l)
	{
		delete model_l;
	}
	_graphic.tearDown();
}

void BlocEngine::visitSDLEvent(SDLEventGameMessage const &msg_p)
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
		if (evt.key.keysym.scancode == SDL_SCANCODE_RIGHT)
		{
			if(_currentBloc)
			{
				if(_map.checkPosition(_currentBloc->getForm(), {_currentBloc->getPosition()[0]+1, _currentBloc->getPosition()[1], _currentBloc->getPosition()[2]}))
				{
					_graphic.registerMessage(new MoveSceneMessage("currentBloc", {1.,0.,0.}));
					_currentBloc->setX(_currentBloc->getPosition()[0]+1);
				}
			}
		}
		if (evt.key.keysym.scancode == SDL_SCANCODE_LEFT)
		{
			if(_currentBloc)
			{
				if(_map.checkPosition(_currentBloc->getForm(), {_currentBloc->getPosition()[0]-1, _currentBloc->getPosition()[1], _currentBloc->getPosition()[2]}))
				{
					_graphic.registerMessage(new MoveSceneMessage("currentBloc", {-1.,0.,0.}));
					_currentBloc->setX(_currentBloc->getPosition()[0]-1);
				}
			}
		}
		if (evt.key.keysym.scancode == SDL_SCANCODE_R)
		{
			if(_currentBloc)
			{
				this->BlocMessageHandler::registerMessage(new RotateBlocMessage(_currentBloc));
			}
		}
		if (evt.key.keysym.scancode == SDL_SCANCODE_S)
		{
			_speed = !_speed;
		}
		break;
	case SDL_QUIT:
		_graphic.setQuit();
		break;
	default:
		break;
	}
}
