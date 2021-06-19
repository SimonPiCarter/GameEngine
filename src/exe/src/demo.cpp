#include "engine/GraphicEngine.h"
#include "resource/Resource.h"
#include "entity/GraphicEntity.h"
#include "message/entity/NewGraphicEntityMessage.h"
#include "message/entity/MoveGraphicEntityMessage.h"
#include "message/entity/AnimateGraphicEntityMessage.h"
#include "message/entity/RotateGraphicEntityMessage.h"
#include "message/gui/CustomGuiMessage.h"
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
#include "gui/PopUpScreen.h"
#include "gui/RichLabel.h"

#include "ColibriGui/Layouts/ColibriLayoutLine.h"

//////////////////////////////////
//////////////////////////////////
// CENTRAL MENU START
//////////////////////////////////
//////////////////////////////////

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
	// ref is mandatory because we set up pointer after listener is created
	ReturnListener(central_menu::Menu *& menu_p) : _menu(menu_p) {}
	virtual void run()
	{
		_menu->setHidden(true);
	}
private:
	// pointer to reset menu to nullptr and delete it (hence pointer to pointer)
	central_menu::Menu *& _menu;
};

//////////////////////////////////
//////////////////////////////////
// CENTRAL MENU END
//////////////////////////////////
//////////////////////////////////

//////////////////////////////////
//////////////////////////////////
// POPUP SCREEN START
//////////////////////////////////
//////////////////////////////////

class DemoPopUp : public PopUpScreen
{
public:
	DemoPopUp() : PopUpScreen() {}

	virtual void setContent()
	{
		Colibri::LayoutLine *layoutV = new Colibri::LayoutLine( _manager );
		layoutV->m_vertical = true;

		Colibri::Button *button1_l = _manager->createWidget<Colibri::Button>( _window );
		button1_l->m_minSize = Ogre::Vector2( 350, 64 );
		button1_l->getLabel()->setText( "button1" );
		button1_l->sizeToFit();
		Colibri::Button *button2_l = _manager->createWidget<Colibri::Button>( _window );
		button2_l->m_minSize = Ogre::Vector2( 350, 64 );
		button2_l->getLabel()->setText( "button2" );
		button2_l->sizeToFit();

		layoutV->addCell(button1_l);
		layoutV->addCell(button2_l);
		button1_l->m_expand[0] = true;
		button2_l->m_expand[0] = true;

		Colibri::LayoutLine *layoutH = new Colibri::LayoutLine( _manager );
		layoutH->m_vertical = false;

		Colibri::Button *button3_l = _manager->createWidget<Colibri::Button>( _window );
		button3_l->m_minSize = Ogre::Vector2( 350, 64 );
		button3_l->getLabel()->setText( "button3" );
		button3_l->sizeToFit();
		Colibri::Button *button4_l = _manager->createWidget<Colibri::Button>( _window );
		button4_l->m_minSize = Ogre::Vector2( 350, 64 );
		button4_l->getLabel()->setText( "button4" );
		button4_l->sizeToFit();
		Colibri::Button *button5_l = _manager->createWidget<Colibri::Button>( _window );
		button5_l->m_minSize = Ogre::Vector2( 350, 64 );
		button5_l->getLabel()->setText( "button5" );
		button5_l->sizeToFit();

		layoutH->addCell(button3_l);
		layoutH->addCell(button4_l);
		layoutH->addCell(button5_l);

		Colibri::LayoutLine *layoutH2 = new Colibri::LayoutLine( _manager );
		layoutH2->m_vertical = false;

		Colibri::Button *button7_l = _manager->createWidget<Colibri::Button>( _window );
		button7_l->m_minSize = Ogre::Vector2( 100, 64 );
		button7_l->getLabel()->setText( "button7" );
		button7_l->sizeToFit();
		Colibri::Button *button6_l = _manager->createWidget<Colibri::Button>( _window );
		button6_l->m_minSize = Ogre::Vector2( 100, 64 );
		button6_l->getLabel()->setText( "button6" );
		button6_l->sizeToFit();
		Colibri::Button *button8_l = _manager->createWidget<Colibri::Button>( _window );
		button8_l->m_minSize = Ogre::Vector2( 100, 64 );
		button8_l->getLabel()->setText( "button8" );
		button8_l->sizeToFit();

		layoutH2->addCell(button6_l);
		layoutH2->addCell(button7_l);
		layoutH2->addCell(button8_l);
		layoutH2->m_gridLocation = Colibri::GridLocations::Center;
		Colibri::LayoutBase::setMarginToAllCells(layoutH2->getCells(), Ogre::Vector2(20,0));

		layoutH2->m_hardMaxSize = _manager->getCanvasSize();
		layoutH2->layout();

		layoutH->m_hardMaxSize = _manager->getCanvasSize();
		layoutH->layout();

		layoutV->addCell(layoutH);
		layoutV->addCell(layoutH2);

		layoutV->setAdjustableWindow(_window);
		layoutV->m_hardMaxSize = _manager->getCanvasSize();
		layoutV->layout();
	}

private:
};

//////////////////////////////////
//////////////////////////////////
// POPUP SCREEN END
//////////////////////////////////
//////////////////////////////////

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
		_resourceHandler.addResource({"athena", "athene.mesh", true, "", 1.});
		_resourceHandler.addResource({"Cube", "Cube.mesh", false, "", 1.});
		_resourceHandler.addResource({"CubeGreen", "Cube.mesh", false, "Green", 1.});
		_resourceHandler.addResource({"CubeYellow", "Cube.mesh", false, "Yellow", 1.});
		_resourceHandler.addResource({"CubeRed", "Cube.mesh", false, "Red", 1.});
		_resourceHandler.addUISkin("/Materials/ColibriGui/Skins/Custom/Skins.colibri.json");
		_graphic.initWindow("demo");
	}
	virtual void run() override
	{
		if(_graphic.getQuit())
		{
			//User cancelled config
			_graphic.tearDown();
		}

		std::vector<InfoLabel> content3_l;
		content3_l.push_back({"Return to game", 255, 255, 255});
		RichLabel * label3_l = new RichLabel(content3_l, 500, 70, 500, 50, 10, true, _graphic);

		std::vector<ButtonData> data_l;
		data_l.push_back({"destroy_scene", &_destroy, nullptr, ""});
		data_l.push_back({"quit", &_quit, nullptr, ""});
		data_l.push_back({"return", &_return, label3_l, "Custom_Button"});
		_menu = new central_menu::Menu("Main", data_l, _graphic);

		_popup = new DemoPopUp();
		_popup->init(data_l, _graphic);

		std::vector<InfoLabel> content_l;
		content_l.push_back({"F5", 255, 255, 0});
		content_l.push_back({" : Menu\n", 255, 255, 255});
		RichLabel * label_l = new RichLabel(content_l, 20, 0, 400, 50, 10, false, _graphic);


		std::vector<InfoLabel> content2_l;
		content2_l.push_back({" : Menu\nLorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", 255, 255, 255});
		RichLabel * label2_l = new RichLabel(content2_l, 20, 20, 400, 400, 10, true, _graphic);


		Ogre::Window *renderWindow_l = _graphic.getRenderWindow();

		GraphicEntity entity1_l;
		GraphicEntity entity2_l;
		GraphicEntity entity3_l;
		GraphicEntity light_l;
		_graphic.registerMessage(new NewSceneMessage("test", "root", {1., 2., 3.}));

		_graphic.registerMessage(new NewGraphicEntityMessage(&entity1_l, "Cube", {0., 0., 0.}, "test"));
		_graphic.registerMessage(new NewGraphicEntityMessage(&entity2_l, "CubeGreen", {-2., -2., 0.}, "test"));
		_graphic.registerMessage(new NewGraphicEntityMessage(&entity3_l, "CubeYellow", {-4., -4., 0.}, "test"));
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

		_graphic.registerMessage(new DestroyWindowMessage(label_l->getWindow()));
		_graphic.registerMessage(new DestroyWindowMessage(label2_l->getWindow()));
		_graphic.registerMessage(new DestroyWindowMessage(label3_l->getWindow()));
		_graphic.registerMessage(new DestroyWindowMessage(_menu->getWindow()));

		delete label_l;
		delete label2_l;
		delete label3_l;
		delete _menu;
		delete _popup;

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
				_menu->setHidden(false);
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
	PopUpScreen * _popup;

	QuitListener _quit;
	DestroyListener _destroy;
	ReturnListener _return;

};

#undef main

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
