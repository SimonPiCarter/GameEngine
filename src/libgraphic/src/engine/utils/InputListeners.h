
#ifndef _Demo_InputListeners_H_
#define _Demo_InputListeners_H_

union SDL_Event;
struct SDL_MouseButtonEvent;

struct SDL_TextEditingEvent;
struct SDL_TextInputEvent;
struct SDL_KeyboardEvent;

struct SDL_JoyButtonEvent;
struct SDL_JoyAxisEvent;
struct SDL_JoyHatEvent;

class MouseListener
{
public:
    //Receives SDL_MOUSEMOTION and SDL_MOUSEWHEEL events
    virtual void mouseMoved( const SDL_Event &arg ) = 0;
    virtual void mousePressed( const SDL_MouseButtonEvent &arg, Ogre::uint8 id ) = 0;
    virtual void mouseReleased( const SDL_MouseButtonEvent &arg, Ogre::uint8 id ) = 0;
};

class KeyboardListener
{
public:
    virtual void textEditing( const SDL_TextEditingEvent& arg ) = 0;
    virtual void textInput( const SDL_TextInputEvent& arg ) = 0;
    virtual void keyPressed( const SDL_KeyboardEvent &arg ) = 0;
    virtual void keyReleased (const SDL_KeyboardEvent &arg ) = 0;
};

class JoystickListener
{
public:
    virtual void joyButtonPressed( const SDL_JoyButtonEvent &evt, int button ) = 0;
    virtual void joyButtonReleased( const SDL_JoyButtonEvent &evt, int button ) = 0;
    virtual void joyAxisMoved( const SDL_JoyAxisEvent &arg, int axis ) = 0;
    virtual void joyPovMoved( const SDL_JoyHatEvent &arg, int index ) = 0;
};

#endif
