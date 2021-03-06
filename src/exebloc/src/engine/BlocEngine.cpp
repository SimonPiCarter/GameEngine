#include "BlocEngine.h"

#include "OgreWindow.h"
#include "OgreSceneManager.h"
#include <thread>
#include <cstdlib>

#include "bloc/Bloc.h"
#include "bloc/BlocModel.h"
#include "utils/DataReader.h"

std::string getScene(BlocType type_p)
{
	switch(type_p)
	{
		case BlocType::CURRENT_BLOC:
			return "currentBloc";
		case BlocType::NEXT_BLOC:
			return "nextBloc";
	}
	throw std::logic_error("unknown type");
}

BlocEngine::BlocEngine(std::string const &path_p)
	: GameEngine(path_p)
	, BlocMessageHandler(this)
	, _map(this)
{}

void BlocEngine::init()
{
	_resourceHandler.addResource({"Cube", "Cube.mesh", false, "", 0.5});
	_resourceHandler.addResource({"CubeGreen", "Cube.mesh", false, "Green", 0.5});
	_resourceHandler.addResource({"CubeYellow", "Cube.mesh", false, "Yellow", 0.5});
	_resourceHandler.addResource({"CubeRed", "Cube.mesh", false, "Red", 0.5});
	_graphic.initWindow("demo");
}

void BlocEngine::run()
{
	if(_graphic.getQuit())
	{
		//User cancelled config
		_graphic.tearDown();
	}

	std::vector<InfoLabel> contentMenu_l;
	contentMenu_l.push_back({"F5", 255, 255, 0});
	contentMenu_l.push_back({" : Menu\n", 255, 255, 255});
	RichLabel * label_l = new RichLabel(contentMenu_l, 20, 20, 400, 50, 10, false, _graphic);

	std::vector<InfoLabel> contentScore_l;
	contentScore_l.push_back({"Score : ", 255, 255, 0});
	contentScore_l.push_back({std::to_string(long(_score))+"\n", 255, 255, 255});
	_labelScore = new RichLabel(contentScore_l, 20, 50, 400, 50, 10, false, _graphic);

	GraphicEntity light_l;

	std::list<std::list<pattern_t>> data_l = readData(_graphic.getResourcePath()+"/bloc/models.txt");

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
			_model.push_back(new BlocModel(forms_l, res_l));
		}
	}
	this->BlocMessageHandler::registerMessage(new SpawnBlocMessage(*_model[0], {5,5,0}));

	_graphic.registerMessage(new NewSceneMessage("main", "root", {-5.,2.,-20.}));
	_graphic.registerMessage(new NewLightMessage(&light_l, "main", {1., 1., 1.}, {-1, -1, -1}, LightType::Directional));

	/// BORDERS
	std::vector<GraphicEntity *> borders_l;
	for(long i = -1 ; i <= 9 ; ++i)
	{
		borders_l.push_back(new GraphicEntity());
		_graphic.registerMessage(new NewGraphicEntityMessage(borders_l.back(), "Cube", {double(i), 5, 19}, "main"));
	}
	for(size_t i = 0 ; i < 19 ; ++i)
	{
		borders_l.push_back(new GraphicEntity());
		_graphic.registerMessage(new NewGraphicEntityMessage(borders_l.back(), "Cube", {-1, 5, double(i)}, "main"));
		borders_l.push_back(new GraphicEntity());
		_graphic.registerMessage(new NewGraphicEntityMessage(borders_l.back(), "Cube", {9, 5, double(i)}, "main"));
	}

	// NEXT BLOC
	_nextModel = _model[rand()%_model.size()];
	this->BlocMessageHandler::registerMessage(new SpawnBlocMessage(*_nextModel, {15,10,0}, BlocType::NEXT_BLOC));

	_graphic.registerMessage(new RotateSceneMessage("main", {90, 0, 0}));



	std::thread logicThread_l(&BlocEngine::runLogic, this);
	runGraphic();
	logicThread_l.join();

	for(GraphicEntity* ent_l : borders_l)
	{
		delete ent_l;
	}
	for(BlocModel *model_l : _model)
	{
		delete model_l;
	}
	_graphic.registerMessage(new DestroyWindowMessage(label_l->getWindow()));
	_graphic.registerMessage(new DestroyWindowMessage(_labelScore->getWindow()));
	delete label_l;
	delete _labelScore;
	_labelScore = nullptr;
	_graphic.tearDown();
}

void BlocEngine::runGraphic()
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

void BlocEngine::runLogic()
{
	std::chrono::time_point<std::chrono::system_clock> start_l = std::chrono::system_clock::now();

	double timeSinceLast_l = 1.0 / 60.0;

	while( !_graphic.getQuit() )
	{
		// handle game message
		MessageHandler<BlocMessage>::handleAllMessages();
		MessageHandler<GameMessage>::handleAllMessages();

		if( !_graphic.getRenderWindow()->isVisible() )
		{
			//Don't burn CPU cycles unnecessary when we're minimized.
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		double move_l = timeSinceLast_l * (_speed ? 10. : 1.);
		// Update bloc
		if(getCurrentBloc())
		{
			// update freefall movement of bloc
			getCurrentBloc()->updateLevel(getCurrentBloc()->getLevel()+move_l);
			_graphic.registerMessage(new MoveSceneMessage(getScene(BlocType::CURRENT_BLOC), {0.,0.,move_l}));

			// check for freeze
			if(_map.checkFreeze(getCurrentBloc()))
			{
				// adjust back bloc
				_graphic.registerMessage(new MoveSceneMessage(
					getScene(BlocType::CURRENT_BLOC), {
						double(getCurrentBloc()->getPosition()[0]),
						double(getCurrentBloc()->getPosition()[1]),
						double(std::min<unsigned long>(MAP_SIZE-1, getCurrentBloc()->getPosition()[2] - 1))}
					, false));
				getCurrentBloc()->updateLevel(getCurrentBloc()->getPosition()[2]);

				// Freeze current bloc
				this->BlocMessageHandler::registerMessage(new FreezeBlocMessage(getCurrentBloc()));
				setBloc(BlocType::CURRENT_BLOC, nullptr);
				// Spawn next one
				this->BlocMessageHandler::registerMessage(new SpawnBlocMessage(*this->_nextModel, {5,5,0}));
				// update next bloc
				this->_nextModel = _model[rand()%_model.size()];
				_graphic.registerMessage(new DestroySceneMessage(getScene(BlocType::NEXT_BLOC)));
				delete getNextBloc();
				setBloc(BlocType::NEXT_BLOC, nullptr);
				this->BlocMessageHandler::registerMessage(new SpawnBlocMessage(*_nextModel, {15,10,0}, BlocType::NEXT_BLOC));
				// Reset speed
				_speed = false;
			}
		}
		if(getNextBloc())
		{
			_graphic.registerMessage(new RotateSceneMessage(getScene(BlocType::NEXT_BLOC), {0.,0.,timeSinceLast_l*45.}));
		}
		if(_map.checkLose())
		{
			// End game
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
			if(getCurrentBloc())
			{
				if(_map.checkPosition(getCurrentBloc()->getForm(), {getCurrentBloc()->getPosition()[0]+1, getCurrentBloc()->getPosition()[1], getCurrentBloc()->getPosition()[2]}))
				{
					_graphic.registerMessage(new MoveSceneMessage(getScene(BlocType::CURRENT_BLOC), {1.,0.,0.}));
					getCurrentBloc()->setX(getCurrentBloc()->getPosition()[0]+1);
				}
			}
		}
		if (evt.key.keysym.scancode == SDL_SCANCODE_LEFT)
		{
			if(getCurrentBloc())
			{
				if(_map.checkPosition(getCurrentBloc()->getForm(), {getCurrentBloc()->getPosition()[0]-1, getCurrentBloc()->getPosition()[1], getCurrentBloc()->getPosition()[2]}))
				{
					_graphic.registerMessage(new MoveSceneMessage(getScene(BlocType::CURRENT_BLOC), {-1.,0.,0.}));
					getCurrentBloc()->setX(getCurrentBloc()->getPosition()[0]-1);
				}
			}
		}
		if (evt.key.keysym.scancode == SDL_SCANCODE_R)
		{
			if(getCurrentBloc())
			{
				this->BlocMessageHandler::registerMessage(new RotateBlocMessage(getCurrentBloc()));
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

void BlocEngine::updateLabelScore()
{
	if(_labelScore)
	{
		std::vector<InfoLabel> contentScore_l;
		contentScore_l.push_back({"Score : ", 255, 255, 0});
		contentScore_l.push_back({std::to_string(long(_score))+"\n", 255, 255, 255});
		_labelScore->updateText(contentScore_l);
	}
}
