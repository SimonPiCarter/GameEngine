#include "engine/GraphicEngine.h"
#include "resource/Resource.h"
#include "entity/GraphicEntity.h"
#include "message/entity/NewGraphicEntityMessage.h"
#include "message/entity/MoveGraphicEntityMessage.h"
#include "message/entity/AnimateGraphicEntityMessage.h"
#include "message/entity/RotateGraphicEntityMessage.h"
#include "message/light/NewLightMessage.h"
#include "message/scene/DestroySceneMessage.h"
#include "message/scene/MoveSceneMessage.h"
#include "message/scene/NewSceneMessage.h"

#include "OgreWindow.h"
#include "OgreSceneManager.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "engine/GameEngine.h"
#include "gui/CentralMenu.h"
#include "gui/RichLabel.h"

class QuitListener : public Listener
{
public:
	QuitListener(GraphicEngine &graphic_p)
		: _graphic(graphic_p) {}

	virtual void run()
	{
		_graphic.setQuit();
	}
private:
	GraphicEngine &_graphic;
};

class DestroyListener : public Listener
{
public:
	DestroyListener(bool &delete_p, GraphicEngine &graphic_p)
		: _delete(delete_p), _graphic(graphic_p) {}

	virtual void run()
	{
		if(!_delete)
			_graphic.registerMessage(new DestroySceneMessage("test"));
		_delete = true;
	}
private:
	bool &_delete;
	GraphicEngine &_graphic;
};

class ReturnListener : public Listener
{
public:
	ReturnListener(central_menu::Menu *&menu_p) : _menu(menu_p) {}
	virtual void run()
	{
		delete _menu;
		// to avoid double deletion
		_menu = nullptr;
	}
private:
	// pointer to reset menu to nullptr and delete it (hence pointer to pointer)
	central_menu::Menu *&_menu;
};

class DemoEngine : public GameEngine
{
public:
	DemoEngine(std::string const &path_p)
		: GameEngine(path_p)
		, _quit(_graphic)
		, _destroy(_deleted, _graphic)
		// we take reference to the pointer hence it is ok to give it before init
		, _return(_menu)
	{}

	virtual void init() override
	{
		_resourceHandler.addResource({"athena", "athene.mesh", true, ""});
		_resourceHandler.addResource({"Cube", "Cube.mesh", false, ""});
		_resourceHandler.addResource({"CubeGreen", "Cube.mesh", false, "Green"});
		_resourceHandler.addResource({"CubeYellow", "Cube.mesh", false, "Yellow"});
		_resourceHandler.addResource({"CubeRed", "Cube.mesh", false, "Red"});
		_graphic.initWindow("demo");
	}
	virtual void run() override
	{
		if(_graphic.getQuit())
		{
			//User cancelled config
			_graphic.tearDown();
		}

		_menu = new central_menu::Menu("Main",
		{
			{"destroy_scene", &_destroy},
			{"quit", &_quit},
			{"return", &_return}
		}, _graphic
		);

		std::vector<InfoLabel> content_l;
		content_l.push_back({"F5", 255, 255, 0});
		content_l.push_back({" : Menu\n", 255, 255, 255});
		RichLabel * label_l = new RichLabel(content_l, 20, 20, 400, 50, 10, _graphic);

		Ogre::Window *renderWindow_l = _graphic.getRenderWindow();

		GraphicEntity entity1_l;
		GraphicEntity entity2_l;
		GraphicEntity entity3_l;
		GraphicEntity light_l;
		_graphic.registerMessage(new NewSceneMessage("test", "root", {1., 2., 3.}));

		_graphic.registerMessage(new NewGraphicEntityMessage(&entity1_l, "Cube", {0., 0., 0.}, {1., 1., 1.}, "test"));
		_graphic.registerMessage(new NewGraphicEntityMessage(&entity2_l, "CubeGreen", {-2., -2., 0.}, {1., 1., 1.}, "test"));
		_graphic.registerMessage(new NewGraphicEntityMessage(&entity3_l, "CubeYellow", {-4., -4., 0.}, {1., 1., 1.}, "test"));
		_graphic.registerMessage(new AnimateGraphicEntityMessage(&entity1_l, "my_animation", true, true));
		_graphic.registerMessage(new NewLightMessage(&light_l, "root", {1., 1., 1.}, {-1, -1, -1}, LightType::Directional));

		std::chrono::time_point<std::chrono::system_clock> start_l = std::chrono::system_clock::now();

		double timeSinceLast = 1.0 / 60.0;

		double elapsedTime = 0.;

		while( !_graphic.getQuit() )
		{
			// handle game message
			handleAllMessages();

			_graphic.getSceneManager()->updateAllAnimations();

			// handle frame
			_graphic.handleFrame(timeSinceLast);

			if( !renderWindow_l->isVisible() )
			{
				//Don't burn CPU cycles unnecessary when we're minimized.
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}

			if(!_deleted)
			{
				_graphic.registerMessage(new MoveGraphicEntityMessage(&entity3_l, {2.*timeSinceLast, 0., 0.}));
				_graphic.registerMessage(new RotateGraphicEntityMessage(&entity2_l, {30.*timeSinceLast, 0., 0.}));
				_graphic.registerMessage(new MoveSceneMessage("test", {-0.5*timeSinceLast, 0., 0.}));
			}

			const std::chrono::time_point<std::chrono::system_clock> end_l = std::chrono::system_clock::now();

			timeSinceLast = (end_l - start_l) / std::chrono::milliseconds(1);
			timeSinceLast = std::min( 1.0, timeSinceLast/1000. ); //Prevent from going haywire.
			start_l = end_l;
		}

		delete _menu;

		_graphic.tearDown();
	}

	virtual void visitSDLEvent(SDLEventGameMessage const &msg_p) override
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
				if(!_deleted)
				{
					_graphic.registerMessage(new DestroySceneMessage("test"));
					_deleted = true;
				}
			}
			if (evt.key.keysym.scancode == SDL_SCANCODE_V)
			{
				if(!_deleted)
				{
					_visible = !_visible;
					_graphic.registerMessage(new VisibilitySceneMessage("test", _visible));
				}
			}
			if (evt.key.keysym.scancode == SDL_SCANCODE_F5)
			{
				if(!_menu)
				{
					_menu = new central_menu::Menu("Main",
					{
						{"destroy_scene", &_destroy},
						{"quit", &_quit},
						{"return", &_return}
					}, _graphic
					);
				}
			}
			break;
		case SDL_QUIT:
			_graphic.setQuit();
			break;
		default:
			break;
		}
	}

private:
	bool _deleted = false;
	bool _visible = true;

	central_menu::Menu * _menu;

	QuitListener _quit;
	DestroyListener _destroy;
	ReturnListener _return;

};

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cerr<<"expected call : "<<argv[0] << "path_to_data_folder"<<std::endl;
		return -1;
	}
	DemoEngine engine_l(argv[1]);

	engine_l.init();
	engine_l.run();

	return 0;
}
