#include "engine/GraphicEngine.h"
#include "resource/Resource.h"
#include "entity/GraphicEntity.h"
#include "message/implem/NewGraphicEntityMessage.h"
#include "message/implem/MoveGraphicEntityMessage.h"
#include "message/implem/AnimateGraphicEntityMessage.h"

#include "OgreRenderWindow.h"

#include <chrono>
#include <iostream>
#include <thread>

#include "engine/GameEngine.h"

class DemoEngine : public GameEngine
{
public:
	DemoEngine() : GameEngine() {}

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

		Ogre::RenderWindow *renderWindow_l = _graphic.getRenderWindow();

		GraphicEntity entity1_l;
		GraphicEntity entity2_l;
		GraphicEntity entity3_l;
		{
			NewGraphicEntityMessage * msg_l = new NewGraphicEntityMessage(&entity1_l, "Cube", {0., 0., 0.}, {1., 1., 1.}, "root");
			_graphic.registerMessage(msg_l);
		}
		{
			NewGraphicEntityMessage * msg_l = new NewGraphicEntityMessage(&entity2_l, "Cube", {-2., -2., 0.}, {1., 1., 1.}, "root");
			_graphic.registerMessage(msg_l);
		}
		{
			NewGraphicEntityMessage * msg_l = new NewGraphicEntityMessage(&entity3_l, "Cube", {-4., -4., 0.}, {1., 1., 1.}, "root");
			_graphic.registerMessage(msg_l);
		}
		{
			AnimateGraphicEntityMessage * msg_l = new AnimateGraphicEntityMessage(&entity1_l, "my_animation", true, true);
			_graphic.registerMessage(msg_l);
		}

		std::chrono::time_point<std::chrono::system_clock> start_l = std::chrono::system_clock::now();

		double timeSinceLast = 1.0 / 60.0;

		double elapsedTime = 0.;

		while( !_graphic.getQuit() )
		{
			// handle game message
			handleAllMessages();

			// handle frame
			_graphic.handleFrame(timeSinceLast);

			if( !renderWindow_l->isVisible() )
			{
				//Don't burn CPU cycles unnecessary when we're minimized.
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}

			MoveGraphicEntityMessage * msg_l = new MoveGraphicEntityMessage(&entity3_l, {2.*timeSinceLast, 0., 0.});
			_graphic.registerMessage(msg_l);

			const std::chrono::time_point<std::chrono::system_clock> end_l = std::chrono::system_clock::now();

			timeSinceLast = (end_l - start_l) / std::chrono::milliseconds(1);
			timeSinceLast = std::min( 1.0, timeSinceLast/1000. ); //Prevent from going haywire.
			start_l = end_l;
		}

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
			break;
		case SDL_QUIT:
			_graphic.setQuit();
			break;
		default:
			break;
		}
	}

private:
};

int main(int argc, char **argv)
{
	DemoEngine engine_l;

	engine_l.init();
	engine_l.run();

	return 0;
}
