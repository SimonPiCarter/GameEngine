#include "GraphicEngine.h"

#include "OgreRoot.h"
#include "OgreException.h"
#include "OgreConfigFile.h"
#include "OgreWindow.h"
#include "OgreCamera.h"
#include "OgreFileSystemLayer.h"
#include "OgreWindowEventUtilities.h"
#include "OgrePlatformInformation.h"
#include "Compositor/OgreCompositorManager2.h"
#include "Compositor/OgreCompositorWorkspace.h"
#include "Compositor/OgreCompositorShadowNode.h"

#include "OgreHlmsUnlit.h"
#include "OgreHlmsPbs.h"
#include "OgreHlmsManager.h"
#include "OgreArchiveManager.h"

#include "ColibriGui/ColibriManager.h"
#include "ColibriGui/Text/ColibriShaperManager.h"
#include "ColibriGui/Text/ColibriShaper.h"
#include "ColibriGui/Ogre/CompositorPassColibriGuiProvider.h"
#include "ColibriGui/Ogre/OgreHlmsColibri.h"
#include "hb.h"

#include <SDL_syswm.h>
#include <fstream>

#include "resource/Resource.h"


GraphicEngine::GraphicEngine(GameMessageHandler * gameMessageHandler_p, ResourceHandler const *resourceHandler_p) :
	GraphicMessageHandler(this),
	_gameMessageHandler(gameMessageHandler_p),
	_resourceHandler(resourceHandler_p),
	_root(nullptr),
	_renderWindow(nullptr),
	_sceneManager(nullptr),
	_camera(nullptr),
	_overlaySystem(nullptr),
	_workspace(nullptr),
	_sdlWindow(nullptr),
	_inputHandler(nullptr),
	_colibriManager(nullptr),
	_pluginsFolder("./"),
	_resourcePath(resourceHandler_p->getRootPath()),
	_writeAccessFolder(""),
	_quit( false ),
	_alwaysAskForConfig( true ),
	_backgroundColour(Ogre::ColourValue(0.2f, 0.4f, 0.6f))
{
	if( isWriteAccessFolder(_pluginsFolder, "Ogre.log"))
	{
		_writeAccessFolder = _pluginsFolder;
	}
	else
	{
		Ogre::FileSystemLayer filesystemLayer_l( OGRE_VERSION_NAME );
		_writeAccessFolder = filesystemLayer_l.getWritablePath("");
	}
	for(size_t i = 0 ; i < _shadowMaps.max_size() ; ++ i )
	{
		_shadowMaps[i] = nullptr;
	}
}

bool GraphicEngine::isWriteAccessFolder(const std::string &folderPath_p, const std::string &fileToSave_p)
{
	if( !Ogre::FileSystemLayer::createDirectory(folderPath_p) )
		return false;

	std::ofstream of((folderPath_p + fileToSave_p).c_str(),
						std::ios::out | std::ios::binary | std::ios::app);
	if(!of)
	{
		return false;
	}

	return true;
}

void GraphicEngine::initWindow(const std::string &windowTitle_p)
{
	if( SDL_Init( SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK |
					SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS ) != 0 )
	{
		OGRE_EXCEPT( Ogre::Exception::ERR_INTERNAL_ERROR, "Cannot initialize SDL2!",
						"GraphicEngine::initialize" );
	}

	Ogre::String pluginPath_l;
	// only use plugins.cfg if not static
	pluginPath_l = _pluginsFolder + "plugins.cfg";

	_root = OGRE_NEW Ogre::Root( pluginPath_l,
									_writeAccessFolder + "ogre.cfg",
									_writeAccessFolder + "Ogre.log" );

	_staticPluginLoader.install( _root );

	if( _alwaysAskForConfig || !_root->restoreConfig() )
	{
		if( !_root->showConfigDialog() )
		{
			_quit = true;
			return;
		}
	}
	_root->getRenderSystem()->setConfigOption( "sRGB Gamma Conversion", "Yes" );
	_root->initialise(false);

	Ogre::ConfigOptionMap& cfgOpts_l = _root->getRenderSystem()->getConfigOptions();

	int width_l   = 1280;
	int height_l  = 720;

	Ogre::ConfigOptionMap::iterator options_l = cfgOpts_l.find( "Video Mode" );
	if( options_l != cfgOpts_l.end() )
	{
		//Ignore leading space
		const Ogre::String::size_type start_l = options_l->second.currentValue.find_first_of("012356789");
		//Get the width and height
		Ogre::String::size_type widthEnd_l = options_l->second.currentValue.find(' ', start_l);
		// we know that the height starts 3 characters after the width and goes until the next space
		Ogre::String::size_type heightEnd_l = options_l->second.currentValue.find(' ', widthEnd_l+3);
		// Now we can parse out the values
		width_l   = Ogre::StringConverter::parseInt( options_l->second.currentValue.substr( 0, widthEnd_l ) );
		height_l  = Ogre::StringConverter::parseInt( options_l->second.currentValue.substr(
														widthEnd_l+3, heightEnd_l ) );
	}

	Ogre::NameValuePairList params_l;
	bool fullscreen_l = Ogre::StringConverter::parseBool( cfgOpts_l["Full Screen"].currentValue );

	int screen_l = 0;
	int posX_l = SDL_WINDOWPOS_CENTERED_DISPLAY(screen_l);
	int posY_l = SDL_WINDOWPOS_CENTERED_DISPLAY(screen_l);

	if(fullscreen_l)
	{
		posX_l = SDL_WINDOWPOS_UNDEFINED_DISPLAY(screen_l);
		posY_l = SDL_WINDOWPOS_UNDEFINED_DISPLAY(screen_l);
	}

	_sdlWindow = SDL_CreateWindow(
				windowTitle_p.c_str(),    // window title
				posX_l,               // initial x position
				posY_l,               // initial y position
				width_l,              // width, in pixels
				height_l,             // height, in pixels
				SDL_WINDOW_SHOWN
					| (fullscreen_l ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_RESIZABLE );

	//Get the native whnd
	SDL_SysWMinfo wmInfo_l;
	SDL_VERSION( &wmInfo_l.version );

	if( SDL_GetWindowWMInfo( _sdlWindow, &wmInfo_l ) == SDL_FALSE )
	{
		OGRE_EXCEPT( Ogre::Exception::ERR_INTERNAL_ERROR,
						"Couldn't get WM Info! (SDL2)",
						"GraphicEngine::initialize" );
	}

	Ogre::String winHandle_l;
	switch( wmInfo_l.subsystem )
	{
	#ifdef WIN32
	case SDL_SYSWM_WINDOWS:
		// Windows code
		winHandle_l = Ogre::StringConverter::toString( (uintptr_t)wmInfo_l.info.win.window );
		break;
	#elif __MACOSX__
	case SDL_SYSWM_COCOA:
		//required to make OGRE play nice with our window
		params_l.insert( std::make_pair("macAPICocoaUseNSView", "true") );

		winHandle_l  = Ogre::StringConverter::toString(WindowContentViewHandle(wmInfo_l));
		break;
	#else
	case SDL_SYSWM_X11:
		winHandle_l = Ogre::StringConverter::toString( (uintptr_t)wmInfo_l.info.x11.window );
		break;
	#endif
	default:
		OGRE_EXCEPT( Ogre::Exception::ERR_NOT_IMPLEMENTED,
						"Unexpected WM! (SDL2)",
						"GraphicEngine::initialize" );
		break;
	}

	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		params_l.insert( std::make_pair("externalWindowHandle",  winHandle_l) );
	#else
		params_l.insert( std::make_pair("parentWindowHandle",  winHandle_l) );
	#endif

	params_l.insert( std::make_pair("title", windowTitle_p) );
	params_l.insert( std::make_pair("gamma", "true") );
	params_l.insert( std::make_pair("FSAA", cfgOpts_l["FSAA"].currentValue) );
	params_l.insert( std::make_pair("vsync", cfgOpts_l["VSync"].currentValue) );

	_renderWindow = Ogre::Root::getSingleton().createRenderWindow( windowTitle_p, width_l, height_l,
																	fullscreen_l, &params_l );

	_overlaySystem = OGRE_NEW Ogre::v1::OverlaySystem();

	setupResources();
	loadResources();
	chooseSceneManager();
	createCamera();
	_workspace = setupCompositor();

	_mapSceneNode["root"] = _sceneManager->getRootSceneNode( Ogre::SCENE_DYNAMIC );

	_inputHandler = new SdlInputHandler( _sdlWindow, this,
	                                     this, nullptr );

	_colibriManager->setCanvasSize( Ogre::Vector2( 1920.0f, 1080.0f ),
								Ogre::Vector2( _renderWindow->getWidth(), _renderWindow->getHeight() ) );

	_colibriManager->setOgre( getRoot(),
								getRoot()->getRenderSystem()->getVaoManager(),
								getSceneManager() );
	_colibriManager->loadSkins( (getResourcePath() +
								"/Materials/ColibriGui/Skins/DarkGloss/Skins.colibri.json").c_str() );
	_colibriManager->loadSkins( (getResourcePath() +
								"/Materials/ColibriGui/Skins/Custom/Skins.colibri.json").c_str() );

#if OGRE_PROFILING
        Ogre::Profiler::getSingleton().setEnabled( true );
    #if OGRE_PROFILING == OGRE_PROFILING_INTERNAL
        Ogre::Profiler::getSingleton().endProfile( "" );
    #endif
    #if OGRE_PROFILING == OGRE_PROFILING_INTERNAL_OFFLINE
        Ogre::Profiler::getSingleton().getOfflineProfiler().setDumpPathsOnShutdown(
                    _writeAccessFolder + "ProfilePerFrame",
                    _writeAccessFolder + "ProfileAccum" );
    #endif
#endif
}

void GraphicEngine::tearDown()
{
	//saveHlmsDiskCache();

	if( _sceneManager )
		_sceneManager->removeRenderQueueListener( _overlaySystem );

	delete _colibriManager;
	_colibriManager = nullptr;
	OGRE_DELETE _overlaySystem;
	_overlaySystem = nullptr;

	delete _inputHandler;
	_inputHandler = nullptr;

	OGRE_DELETE _root;
	_root = nullptr;
	if( _sdlWindow )
	{
		// Restore desktop resolution on exit
		SDL_SetWindowFullscreen( _sdlWindow, 0 );
		SDL_DestroyWindow( _sdlWindow );
		_sdlWindow = nullptr;
	}

	SDL_Quit();
}

void GraphicEngine::setupResources()
{
	struct ShaperSettings
	{
		const char *locale;
		const char *fullpath;
		hb_script_t script;
		Colibri::HorizReadingDir::HorizReadingDir horizReadingDir;
		bool useKerning;
		bool allowsVerticalLayout;
		ShaperSettings( const char *_locale, const char *_fullpath, hb_script_t _script,
						bool _useKerning=false,
						Colibri::HorizReadingDir::HorizReadingDir _horizReadingDir=
				Colibri::HorizReadingDir::LTR,
						bool _allowsVerticalLayout=false ) :
			locale( _locale ),
			fullpath( _fullpath ),
			script( _script ),
			horizReadingDir( _horizReadingDir ),
			useKerning( _useKerning ),
			allowsVerticalLayout( _allowsVerticalLayout )
		{

		}
	};

	ShaperSettings shaperSettings[1] =
	{
		ShaperSettings( "en", "Fonts/DejaVuSerif.ttf", HB_SCRIPT_LATIN, true ),
	};

	_colibriManager = new Colibri::ColibriManager( &_colibriLogListener,
													&_colibriListener );
	Colibri::ShaperManager *shaperManager = _colibriManager->getShaperManager();

	for( size_t i=0; i<sizeof( shaperSettings ) / sizeof( shaperSettings[0] ); ++i )
	{
		Colibri::Shaper *shaper;
		const std::string fullPath = _resourcePath + shaperSettings[i].fullpath;
		shaper = shaperManager->addShaper( shaperSettings[i].script, fullPath.c_str(),
											shaperSettings[i].locale );
		if( shaperSettings[i].useKerning )
			shaper->addFeatures( Colibri::Shaper::KerningOn );
	}

	size_t defaultFont = 0; //"en"
	shaperManager->setDefaultShaper( defaultFont + 1u,
										shaperSettings[defaultFont].horizReadingDir,
										shaperSettings[defaultFont].allowsVerticalLayout );

	if( defaultFont == 1 )
		_colibriManager->setSwapRTLControls( true );

	Ogre::CompositorPassColibriGuiProvider *compoProvider =
			OGRE_NEW Ogre::CompositorPassColibriGuiProvider( _colibriManager );
	Ogre::CompositorManager2 *compositorManager = _root->getCompositorManager2();
	compositorManager->setCompositorPassProvider( compoProvider );

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_resourcePath, "FileSystem", "Popular");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_resourcePath+"/export2", "FileSystem", "Popular");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_resourcePath+"/compositors", "FileSystem", "Popular");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_resourcePath+"/Particle", "FileSystem", "Popular");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_resourcePath+"/Materials/Common/GLSL", "FileSystem", "Popular");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_resourcePath+"/Materials/ColibriGui/Skins/Custom", "FileSystem", "Popular");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_resourcePath+"/Materials/ColibriGui/Skins/DarkGloss", "FileSystem", "Popular");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_resourcePath+"/Materials/ColibriGui/Skins/Debug", "FileSystem", "Popular");
}

//-----------------------------------------------------------------------------------
    void GraphicEngine::registerHlms()
    {
        Ogre::String rootHlmsFolder = _resourcePath;

        if( rootHlmsFolder.empty() )
            rootHlmsFolder = "./";
        else if( *(rootHlmsFolder.end() - 1) != '/' )
            rootHlmsFolder += "/";

        //At this point rootHlmsFolder should be a valid path to the Hlms data folder

        Ogre::HlmsUnlit *hlmsUnlit = 0;
        Ogre::HlmsPbs *hlmsPbs = 0;
		Ogre::HlmsColibri *hlmsColibri = 0;

        //For retrieval of the paths to the different folders needed
        Ogre::String mainFolderPath;
        Ogre::StringVector libraryFoldersPaths;
        Ogre::StringVector::const_iterator libraryFolderPathIt;
        Ogre::StringVector::const_iterator libraryFolderPathEn;

        Ogre::ArchiveManager &archiveManager = Ogre::ArchiveManager::getSingleton();

		{
			//Create & Register HlmsColibri
			//Get the path to all the subdirectories used by HlmsColibri
			Ogre::HlmsColibri::getDefaultPaths( mainFolderPath, libraryFoldersPaths );
			Ogre::Archive *archiveUnlit = archiveManager.load( rootHlmsFolder + mainFolderPath,
																"FileSystem", true );
			Ogre::ArchiveVec archiveUnlitLibraryFolders;
			libraryFolderPathIt = libraryFoldersPaths.begin();
			libraryFolderPathEn = libraryFoldersPaths.end();
			while( libraryFolderPathIt != libraryFolderPathEn )
			{
				Ogre::Archive *archiveLibrary =
						archiveManager.load( rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true );
				archiveUnlitLibraryFolders.push_back( archiveLibrary );
				++libraryFolderPathIt;
			}

			//Create and register the unlit Hlms
			hlmsColibri = OGRE_NEW Ogre::HlmsColibri( archiveUnlit, &archiveUnlitLibraryFolders );
			Ogre::Root::getSingleton().getHlmsManager()->registerHlms( hlmsColibri );
		}

        {
            //Create & Register HlmsPbs
            //Do the same for HlmsPbs:
            Ogre::HlmsPbs::getDefaultPaths( mainFolderPath, libraryFoldersPaths );
            Ogre::Archive *archivePbs = archiveManager.load( rootHlmsFolder + mainFolderPath,
                                                             "FileSystem", true );

            //Get the library archive(s)
            Ogre::ArchiveVec archivePbsLibraryFolders;
            libraryFolderPathIt = libraryFoldersPaths.begin();
            libraryFolderPathEn = libraryFoldersPaths.end();
            while( libraryFolderPathIt != libraryFolderPathEn )
            {
                Ogre::Archive *archiveLibrary =
                        archiveManager.load( rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true );
                archivePbsLibraryFolders.push_back( archiveLibrary );
                ++libraryFolderPathIt;
            }

            //Create and register
            hlmsPbs = OGRE_NEW Ogre::HlmsPbs( archivePbs, &archivePbsLibraryFolders );
            Ogre::Root::getSingleton().getHlmsManager()->registerHlms( hlmsPbs );
        }


        Ogre::RenderSystem *renderSystem = _root->getRenderSystem();
        if( renderSystem->getName() == "Direct3D11 Rendering Subsystem" )
        {
            //Set lower limits 512kb instead of the default 4MB per Hlms in D3D 11.0
            //and below to avoid saturating AMD's discard limit (8MB) or
            //saturate the PCIE bus in some low end machines.
            bool supportsNoOverwriteOnTextureBuffers;
            renderSystem->getCustomAttribute( "MapNoOverwriteOnDynamicBufferSRV",
                                              &supportsNoOverwriteOnTextureBuffers );

            if( !supportsNoOverwriteOnTextureBuffers )
            {
                hlmsPbs->setTextureBufferDefaultSize( 512 * 1024 );
                hlmsUnlit->setTextureBufferDefaultSize( 512 * 1024 );
            }
        }
    }
//-----------------------------------------------------------------------------------
void GraphicEngine::loadResources()
{
	registerHlms();

	//loadHlmsDiskCache();

	// Initialise, parse scripts etc
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups( true );
}
//-----------------------------------------------------------------------------------
void GraphicEngine::chooseSceneManager()
{
#if OGRE_DEBUG_MODE
        //Debugging multithreaded code is a PITA, disable it.
        const size_t numThreads = 1;
#else
        //getNumLogicalCores() may return 0 if couldn't detect
        const size_t numThreads = std::max<size_t>( 1, Ogre::PlatformInformation::getNumLogicalCores() );
#endif
        // Create the SceneManager, in this case a generic one
        _sceneManager = _root->createSceneManager( Ogre::ST_GENERIC,
                                                   numThreads,
                                                   "ExampleSMInstance" );

	_sceneManager->addRenderQueueListener(_overlaySystem);
	_sceneManager->getRenderQueue()->setSortRenderQueue(
				Ogre::v1::OverlayManager::getSingleton().mDefaultRenderQueueId,
				Ogre::RenderQueue::StableSort );

	//Set sane defaults for proper shadow mapping
	_sceneManager->setShadowDirectionalLightExtrusionDistance( 500.0f );
	_sceneManager->setShadowFarDistance( 500.0f );
}
//-----------------------------------------------------------------------------------
void GraphicEngine::createCamera(void)
{
	_camera = _sceneManager->createCamera( "Main Camera" );

	// Position it at 500 in Z direction
	_camera->setPosition( Ogre::Vector3( 0, 5, 15 ) );
	// Look back along -Z
	_camera->lookAt( Ogre::Vector3( 0, 0, 0 ) );
	_camera->setNearClipDistance( 0.2f );
	_camera->setFarClipDistance( 1000.0f );
	_camera->setAutoAspectRatio( true );
}
//-----------------------------------------------------------------------------------
Ogre::CompositorWorkspace* GraphicEngine::setupCompositor(void)
{
	Ogre::CompositorManager2 *compositorManager = _root->getCompositorManager2();

	const Ogre::String workspaceName_l("workspace");
	if( !compositorManager->hasWorkspaceDefinition( workspaceName_l ) )
	{
		compositorManager->createBasicWorkspaceDef( workspaceName_l, _backgroundColour,
													Ogre::IdString() );
	}

	return compositorManager->addWorkspace( _sceneManager, _renderWindow->getTexture(), _camera,
											workspaceName_l, true );
}

void GraphicEngine::handleWindowEvent(const SDL_Event& evt_p)
{
	switch( evt_p.window.event )
	{
		/*case SDL_WINDOWEVENT_MAXIMIZED:
			SDL_SetWindowBordered( mSdlWindow, SDL_FALSE );
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
		case SDL_WINDOWEVENT_RESTORED:
			SDL_SetWindowBordered( mSdlWindow, SDL_TRUE );
			break;*/
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			int w,h;
			SDL_GetWindowSize( _sdlWindow, &w, &h );
			_renderWindow->windowMovedOrResized();
			break;
		case SDL_WINDOWEVENT_RESIZED:
			_renderWindow->windowMovedOrResized();
			break;
		case SDL_WINDOWEVENT_CLOSE:
			_quit = true;
			break;
		//case SDL_WINDOWEVENT_SHOWN:
		//	_renderWindow->setVisible(true);
		//	break;
		//case SDL_WINDOWEVENT_HIDDEN:
		//	_renderWindow->setVisible(false);
		//	break;
	}
}

//-----------------------------------------------------------------------------------
void GraphicEngine::handleFrame(double elapsedTime_p)
{
	handleAllMessages();
	run(elapsedTime_p);
}


//-----------------------------------------------------------------------------------
void GraphicEngine::run(double elapsedTime_p)
{
	Ogre::WindowEventUtilities::messagePump();

	Ogre::CompositorShadowNode *shadowNode_l = _workspace->findShadowNode("StaticShadowMapsShadowNode");

	for(size_t i = 0 ; i < _engine->getShadowMaps().max_size() ; ++ i)
	{
		if(_engine->getShadowMaps()[i])
		{
			shadowNode_l->setStaticShadowMapDirty(i, true);
		}
	}

	SDL_Event evt;
	while( SDL_PollEvent( &evt ) )
	{
		switch( evt.type )
		{
		case SDL_WINDOWEVENT:
			handleWindowEvent( evt );
			break;
		default:
			break;
		}
		// give back event to event handler
		_gameMessageHandler->registerMessage(new SDLEventGameMessage(evt));

		_inputHandler->_handleSdlEvents(evt);
	}

	static bool tried = false;
	if( !tried )
	{
		_inputHandler->setGrabMousePointer( false );
		_inputHandler->setMouseVisible( true );
		_inputHandler->setMouseRelative( false );
		tried = true;
	}

	_colibriManager->update(elapsedTime_p);

	const bool isTextInputActive_l = SDL_IsTextInputActive();

	if( _colibriManager->focusedWantsTextInput() && !isTextInputActive_l )
		SDL_StartTextInput();
	else if( !_colibriManager->focusedWantsTextInput() && isTextInputActive_l )
		SDL_StopTextInput();

	if( isTextInputActive_l )
	{
		static SDL_Rect oldRect = { 0, 0, 0, 0 };
		//We tried to update this only inside ColibriGuiGameState::keyPressed
		//but it didn't work well in Linux with fcitx
		Ogre::Vector2 imeOffset = _colibriManager->getImeLocation();
		SDL_Rect rect;
		rect.x = imeOffset.x;
		rect.y = imeOffset.y;
		rect.w = 0;
		rect.h = 0;
		if( oldRect.x != rect.x || oldRect.y != rect.y )
			SDL_SetTextInputRect( &rect );
	}

	if( _renderWindow->isVisible() )
		_quit |= !_root->renderOneFrame();
}

void GraphicEngine::mouseMoved( const SDL_Event &arg )
{
	float width  = static_cast<float>( getRenderWindow()->getWidth() );
	float height = static_cast<float>( getRenderWindow()->getHeight() );

	if( arg.type == SDL_MOUSEMOTION )
	{
		Ogre::Vector2 mousePos( arg.motion.x / width, arg.motion.y / height );
		_colibriManager->setMouseCursorMoved( mousePos * _colibriManager->getCanvasSize() );
	}
	else if( arg.type == SDL_MOUSEWHEEL )
	{
		Ogre::Vector2 mouseScroll( arg.wheel.x, -arg.wheel.y );
		_colibriManager->setScroll( mouseScroll * 50.0f *
									_colibriManager->getCanvasSize() *
									_colibriManager->getPixelSize() );
	}
}
//-----------------------------------------------------------------------------------
void GraphicEngine::mousePressed( const SDL_MouseButtonEvent &arg, Ogre::uint8 id )
{
	float width  = static_cast<float>( getRenderWindow()->getWidth() );
	float height = static_cast<float>( getRenderWindow()->getHeight() );

	Ogre::Vector2 mousePos( arg.x / width, arg.y / height );
	_colibriManager->setMouseCursorMoved( mousePos * _colibriManager->getCanvasSize() );
	_colibriManager->setMouseCursorPressed( true, false );
}
//-----------------------------------------------------------------------------------
void GraphicEngine::mouseReleased( const SDL_MouseButtonEvent &arg, Ogre::uint8 id )
{
	_colibriManager->setMouseCursorReleased();
}
//-----------------------------------------------------------------------------------
void GraphicEngine::textEditing( const SDL_TextEditingEvent &arg )
{
	_colibriManager->setTextEdit( arg.text, arg.start, arg.length );
}
//-----------------------------------------------------------------------------------
void GraphicEngine::textInput( const SDL_TextInputEvent &arg )
{
	_colibriManager->setTextInput( arg.text );
}
//-----------------------------------------------------------------------------------
void GraphicEngine::keyPressed( const SDL_KeyboardEvent &arg )
{
	const bool isTextInputActive = SDL_IsTextInputActive();
	const bool isTextMultiline = _colibriManager->isTextMultiline();

	if( (arg.keysym.sym == SDLK_w && !isTextInputActive) || arg.keysym.sym == SDLK_UP )
		_colibriManager->setKeyDirectionPressed( Colibri::Borders::Top );
	else if( (arg.keysym.sym == SDLK_s && !isTextInputActive) || arg.keysym.sym == SDLK_DOWN )
		_colibriManager->setKeyDirectionPressed( Colibri::Borders::Bottom );
	else if( (arg.keysym.sym == SDLK_a && !isTextInputActive) || arg.keysym.sym == SDLK_LEFT )
		_colibriManager->setKeyDirectionPressed( Colibri::Borders::Left );
	else if( (arg.keysym.sym == SDLK_d && !isTextInputActive) || arg.keysym.sym == SDLK_RIGHT )
		_colibriManager->setKeyDirectionPressed( Colibri::Borders::Right );
	else if( ((arg.keysym.sym == SDLK_RETURN ||
				arg.keysym.sym == SDLK_KP_ENTER) && !isTextMultiline) ||
				(arg.keysym.sym == SDLK_SPACE && !isTextInputActive) )
	{
		_colibriManager->setKeyboardPrimaryPressed();
	}
	else if( isTextInputActive )
	{
		if( (arg.keysym.sym == SDLK_RETURN || arg.keysym.sym == SDLK_KP_ENTER) && isTextMultiline )
			_colibriManager->setTextSpecialKeyPressed( SDLK_RETURN, arg.keysym.mod );
		else
		{
			_colibriManager->setTextSpecialKeyPressed( arg.keysym.sym & ~SDLK_SCANCODE_MASK,
														arg.keysym.mod );
		}
	}
}
//-----------------------------------------------------------------------------------
void GraphicEngine::keyReleased( const SDL_KeyboardEvent &arg )
{
	if( (arg.keysym.mod & ~(KMOD_NUM|KMOD_CAPS)) != 0 )
	{
		return;
	}

	const bool isTextInputActive = SDL_IsTextInputActive();
	const bool isTextMultiline = _colibriManager->isTextMultiline();

	if( (arg.keysym.sym == SDLK_w && !isTextInputActive) || arg.keysym.sym == SDLK_UP )
		_colibriManager->setKeyDirectionReleased( Colibri::Borders::Top );
	else if( (arg.keysym.sym == SDLK_s && !isTextInputActive) || arg.keysym.sym == SDLK_DOWN )
		_colibriManager->setKeyDirectionReleased( Colibri::Borders::Bottom );
	else if( (arg.keysym.sym == SDLK_a && !isTextInputActive) || arg.keysym.sym == SDLK_LEFT )
		_colibriManager->setKeyDirectionReleased( Colibri::Borders::Left );
	else if( (arg.keysym.sym == SDLK_d && !isTextInputActive) || arg.keysym.sym == SDLK_RIGHT )
		_colibriManager->setKeyDirectionReleased( Colibri::Borders::Right );
	else if( ((arg.keysym.sym == SDLK_RETURN ||
				arg.keysym.sym == SDLK_KP_ENTER) && !isTextMultiline) ||
				(arg.keysym.sym == SDLK_SPACE && !isTextInputActive) )
	{
		_colibriManager->setKeyboardPrimaryReleased();
	}
	else if( isTextInputActive )
	{
		if( (arg.keysym.sym == SDLK_RETURN || arg.keysym.sym == SDLK_KP_ENTER) && isTextMultiline )
			_colibriManager->setTextSpecialKeyReleased( SDLK_RETURN, arg.keysym.mod );
		else
		{
			_colibriManager->setTextSpecialKeyReleased( arg.keysym.sym & ~SDLK_SCANCODE_MASK,
														arg.keysym.mod );
		}
	}
}
