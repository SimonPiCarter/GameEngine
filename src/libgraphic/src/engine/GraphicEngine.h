#pragma once

#include "utils/StaticPluginLoader.h"
#include "OgrePrerequisites.h"
#include "OgreColourValue.h"
#include "OgreOverlayPrerequisites.h"

#include "Threading/OgreUniformScalableTask.h"
#include "OgreOverlaySystem.h"
#include "OgreOverlayManager.h"

#include <SDL.h>
#include <unordered_map>

#include "message/GraphicMessageHandler.h"
#include "message/game/GameMessageHandler.h"
#include "entity/GraphicEntity.h"

class ResourceHandler;

class GraphicEngine : public GraphicMessageHandler
{
public:
	GraphicEngine(GameMessageHandler *gameMessageHandler_p, ResourceHandler const *resourceHandler_p);

	void initWindow(const std::string &windowTitle_p);
	void tearDown();

	void handleFrame(double elapsedTime_p);
	void run(double elapsedTime_p);

	void setQuit(void)                                      { _quit = true; }
	bool getQuit(void) const                                { return _quit; }

	ResourceHandler const * getResourceHandler() const      { return _resourceHandler; }
	Ogre::Root* getRoot(void) const                         { return _root; }
	Ogre::Window* getRenderWindow(void) const         { return _renderWindow; }
	Ogre::SceneManager* getSceneManager(void) const         { return _sceneManager; }
	Ogre::Camera* getCamera(void) const                     { return _camera; }
	Ogre::CompositorWorkspace* getCompositorWorkspace(void) const { return _workspace; }
	Ogre::v1::OverlaySystem* getOverlaySystem(void) const   { return _overlaySystem; }

	const Ogre::String& getPluginsFolder(void) const        { return _pluginsFolder; }
	const Ogre::String& getResourcePath(void) const         { return _resourcePath; }
	const Ogre::String& getWriteAccessFolder(void) const    { return _writeAccessFolder; }

	std::unordered_map<std::string, Ogre::SceneNode *> & getMapSceneNode() { return _mapSceneNode; }
	std::unordered_map<std::string, Ogre::SceneNode *> const & getMapSceneNode() const { return _mapSceneNode; }

protected:
	GameMessageHandler * const _gameMessageHandler;
	ResourceHandler const * const _resourceHandler;
	std::unordered_map<std::string, Ogre::SceneNode *> _mapSceneNode;

	/// internal stuff

	std::string const _pluginsFolder;
	std::string const _resourcePath;
	std::string _writeAccessFolder;

	bool const _alwaysAskForConfig;
	bool _quit;

	Ogre::Root *_root;
	Ogre::Window *_renderWindow;
	Ogre::SceneManager *_sceneManager;
	Ogre::Camera *_camera;
	Ogre::ColourValue _backgroundColour;

	Ogre::v1::OverlaySystem *_overlaySystem;

	Ogre::CompositorWorkspace *_workspace;

	SDL_Window *_sdlWindow;

	StaticPluginLoader _staticPluginLoader;

	bool isWriteAccessFolder(const std::string &folderPath, const std::string &fileToSave);

	void registerHlms();
	virtual void setupResources();
	/// Optional override method where you can perform resource group loading
	/// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	virtual void loadResources();
	virtual void chooseSceneManager();
	virtual void createCamera();
	/// Virtual so that advanced samples such as Sample_Compositor can override this
	/// method to change the default behavior if setupCompositor() is overridden, be
	/// aware @_backgroundColour will be ignored
	virtual Ogre::CompositorWorkspace* setupCompositor();

	void handleWindowEvent(const SDL_Event& evt_p);
};
