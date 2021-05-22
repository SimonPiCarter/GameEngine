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

class DemoEngine : public GameEngine
{
public:
	DemoEngine(std::string const &path_p) : GameEngine(path_p) {}

	virtual void init() override
	{
		_resourceHandler.addResource({"athena", "athene.mesh", true});
		_resourceHandler.addResource({"Cube", "Cube.mesh", false});
		_graphic.initWindow("demo");
	}
	virtual void run() override
	{
		if(_graphic.getQuit())
		{
			//User cancelled config
			_graphic.tearDown();
		}

		QuitListener quit_l(_graphic);
		DestroyListener destroy_l(_deleted, _graphic);
		central_menu::Menu * menu_l = new central_menu::Menu("Main",
		{
			{"destroy_scene", &destroy_l},
			{"quit", &quit_l}
		}, _graphic
		);

		Ogre::Window *renderWindow_l = _graphic.getRenderWindow();

		GraphicEntity entity1_l;
		GraphicEntity entity2_l;
		GraphicEntity entity3_l;
		GraphicEntity light_l;
		_graphic.registerMessage(new NewSceneMessage("test", "root", {1., 2., 3.}));

		_graphic.registerMessage(new NewGraphicEntityMessage(&entity1_l, "Cube", {0., 0., 0.}, {1., 1., 1.}, "test"));
		_graphic.registerMessage(new NewGraphicEntityMessage(&entity2_l, "Cube", {-2., -2., 0.}, {1., 1., 1.}, "test"));
		_graphic.registerMessage(new NewGraphicEntityMessage(&entity3_l, "Cube", {-4., -4., 0.}, {1., 1., 1.}, "test"));
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

		delete menu_l;

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
