#include "CellsEngine.h"

#include "OgreCamera.h"
#include "OgreWindow.h"
#include "OgreSceneManager.h"
#include "OgreSceneQuery.h"
#include <thread>

#include "logic/LogicEngine.h"
#include "logic/layout/MapLayout.h"
#include "logic/entity/Tower.h"
#include "logic/slot/effect_maker/SlowSlot.h"

CellsEngine::CellsEngine(std::string const &path_p)
	: GameEngine(path_p)
	, _logic(nullptr)
{}


void CellsEngine::init()
{
	_resourceHandler.addResource({"Cube", "new_cube.mesh", false, "", 0.5});
	_resourceHandler.addResource({"CubeGreen", "new_cube.mesh", false, "Green", 0.5});
	_resourceHandler.addResource({"CubeYellow", "new_cube.mesh", false, "Yellow", 0.5});
	_resourceHandler.addResource({"CubeRed", "new_cube.mesh", false, "Red", 0.5});
	_resourceHandler.addUISkin("cells/Skins.colibri.json");
	_resourceHandler.addResourcePath("cells/");
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

	std::list<Tile> tiles_l;
	long size_l = 8;
	for(long i = 0 ; i < size_l ; ++ i )
	{
		for(long j = 0 ; j < size_l ; ++ j )
		{
			tiles_l.push_back({i , j, true, (i%2 + j%2)%2 == 0?"CubeYellow" : "CubeRed"});
		}
	}
	MapLayout layout_l(tiles_l, {0.5, 0.5}, {6.5, 6.5}, { {0.5, 6.5}, {6.5, 6.5} });

	_logic = new LogicEngine(&layout_l, this);
	_logic->init();

	Tower * tower_l = nullptr;
	tower_l = new Tower({1.5, 5.5}, {1., 1.});
	tower_l->setAttackModifier(AttackModifier(1.,1.,2.,3.,AttackType::Arc, DamageType::Standard));
	tower_l->setResource("CubeGreen");
	tower_l->setMaxSlots(3);
	_logic->spawnTower(tower_l);

	tower_l = new Tower({1.5, 1.5}, {1., 1.});
	tower_l->setAttackModifier(AttackModifier(0.5,0.1,2.,3.,AttackType::Direct, DamageType::Standard));
	tower_l->setResource("CubeGreen");
	tower_l->setMaxSlots(5);
	tower_l->getSlots()[0] = new SlowSlot("SlowTest", 120, 0.2, 15., 4);
	_logic->spawnTower(tower_l);

	tower_l = new Tower({5.5, 1.5}, {1., 1.});
	tower_l->setAttackModifier(AttackModifier(1.5,2.,1.,3.,AttackType::Splash, DamageType::Standard));
	tower_l->setResource("CubeGreen");
	tower_l->setMaxSlots(7);
	_logic->spawnTower(tower_l);

	tower_l = new Tower({5.5, 5.5}, {1., 1.});
	tower_l->setAttackModifier(AttackModifier(3.5,2.,1.,3.,AttackType::Line, DamageType::Standard));
	tower_l->setResource("CubeGreen");
	tower_l->setMaxSlots(12);
	_logic->spawnTower(tower_l);

	while( !_graphic.getQuit() )
	{
		// handle game message
		MessageHandler<GameMessage>::handleAllMessages();

		if( !_graphic.getRenderWindow()->isVisible() )
		{
			//Don't burn CPU cycles unnecessary when we're minimized.
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

		_logic->run(timeSinceLast_l);

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

	delete _logic;
	_logic = nullptr;
}

void CellsEngine::visitSDLEvent(SDLEventGameMessage const &msg_p)
{
	SDL_Event const &evt(msg_p.getEvent());
	switch( evt.type )
	{
	case SDL_MOUSEBUTTONDOWN:
		if(_logic)
		{
			double x = double(evt.button.x) / double(_graphic.getRenderWindow()->getWidth());
			double y = double(evt.button.y) / double(_graphic.getRenderWindow()->getHeight());

			Ogre::RaySceneQuery* rayQuery = _graphic.getSceneManager()->createRayQuery(
			_graphic.getCamera()->getCameraToViewportRay(x, y));
			rayQuery->setSortByDistance(true, 1);

			Ogre::RaySceneQueryResult& result = rayQuery->execute();
			Ogre::RaySceneQueryResult::iterator it = result.begin();
			MobEntity * mobSelection_l;
			for ( ; it != result.end(); it++)
			{
				Ogre::Any mobData_l = it->movable->getUserObjectBindings().getUserAny("mob");
				Ogre::Any towerData_l = it->movable->getUserObjectBindings().getUserAny("tower");
				if(mobData_l.has_value())
				{
					_logic->setMobSelection(*Ogre::any_cast<MobEntity *>(&mobData_l));
					_logic->setTowerSelection(nullptr);
				}
				else if (towerData_l.has_value())
				{
					_logic->setTowerSelection(*Ogre::any_cast<Tower *>(&towerData_l));
					_logic->setMobSelection(nullptr);
				}
			}
		}
		break;
	case SDL_KEYDOWN:
		break;
	case SDL_KEYUP:
		if(evt.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			_logic->setTowerSelection(nullptr);
			_logic->setMobSelection(nullptr);
		}
		if(evt.key.keysym.scancode == SDL_SCANCODE_Q)
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

WaveLayout CellsEngine::getNextWave()
{
	WaveLayout layout_l;
	layout_l.mobLayout.push_back(
		{
			{3., 1., ArmorType::Standard, "Cube", {1.,1.}, 1., 5., 2.},		// Mob model
			10,																// number of spawn
			1.																// interval
		}
	);
	return layout_l;
}
