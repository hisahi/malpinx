/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// sdl2/ibase.cc: implementation of base input library on SDL2

#include <iostream>
#include <SDL2/SDL.h>
#include "defs.hh"
#include "ibase.hh"
#include "input.hh"

using std::cerr;
using std::endl;

const Uint8* keyboardState;
SDL_GameController* controller;

struct MenuEventTickState {
    MenuEventTickState() : up(false), down(false), left(false), right(false),
        select(false), exit(false) {}
    bool up;
    bool down;
    bool left;
    bool right;
    bool select;
    bool exit;
} thisTickMenuEvents;

void ibase_init()
{
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER))
        cerr << "Could not initialize SDL game controller input" << endl;
    controller = NULL;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        if (SDL_IsGameController(i)) {
            controller = SDL_GameControllerOpen(i);
            if (!controller)
                cerr << "Could not open controller " << i
                        << ": " << SDL_GetError() << endl;
            else
                break;
        }
    }

    ibase_tick();
}

void ibase_quit()
{
    if (controller)
    {
        SDL_GameControllerClose(controller);
        controller = NULL;
    }
}

void ibase_before_events()
{
    thisTickMenuEvents = {};
}

void ibase_tick()
{
    keyboardState = SDL_GetKeyboardState(NULL);
}

bool ibase_keyb_down(int scanCode)
{
    return keyboardState[scanCode] != 0;
}

bool ibase_pad_down(int buttonCode)
{
    return controller && SDL_GameControllerGetButton(controller,
        static_cast<SDL_GameControllerButton>(buttonCode)) != 0;
}

int ibase_default_keyb(GameInput input)
{
    switch (input)
    {
    case GameInput::MoveUp:     return SDL_SCANCODE_UP;
    case GameInput::MoveDown:   return SDL_SCANCODE_DOWN;
    case GameInput::MoveLeft:   return SDL_SCANCODE_LEFT;
    case GameInput::MoveRight:  return SDL_SCANCODE_RIGHT;
    case GameInput::Fire:       return SDL_SCANCODE_Z;
    case GameInput::FireAux:    return SDL_SCANCODE_X;
    case GameInput::Bomb:       return SDL_SCANCODE_C;
    case GameInput::Pause:      return SDL_SCANCODE_P;
    }
    return 0;
}

int ibase_default_pad(GameInput input)
{
    switch (input)
    {
    case GameInput::MoveUp:     return SDL_CONTROLLER_BUTTON_DPAD_UP;
    case GameInput::MoveDown:   return SDL_CONTROLLER_BUTTON_DPAD_DOWN;
    case GameInput::MoveLeft:   return SDL_CONTROLLER_BUTTON_DPAD_LEFT;
    case GameInput::MoveRight:  return SDL_CONTROLLER_BUTTON_DPAD_RIGHT;
    case GameInput::Fire:       return SDL_CONTROLLER_BUTTON_A;
    case GameInput::FireAux:    return SDL_CONTROLLER_BUTTON_X;
    case GameInput::Bomb:       return SDL_CONTROLLER_BUTTON_B;
    case GameInput::Pause:      return SDL_CONTROLLER_BUTTON_START;
    }
    return 0;
}

void ibase_handle_event(const SDL_Event& evt)
{
    switch (evt.type)
    {
    case SDL_KEYDOWN:
    {
        SDL_Scancode scan = evt.key.keysym.scancode;
        switch (scan)
        {
        case SDL_SCANCODE_UP:
            thisTickMenuEvents.up = true;
            break;
        case SDL_SCANCODE_DOWN:
            thisTickMenuEvents.down = true;
            break;
        case SDL_SCANCODE_LEFT:
            thisTickMenuEvents.left = true;
            break;
        case SDL_SCANCODE_RIGHT:
            thisTickMenuEvents.right = true;
            break;
        case SDL_SCANCODE_RETURN:
        case SDL_SCANCODE_SPACE:
            thisTickMenuEvents.select = true;
            break;
        case SDL_SCANCODE_ESCAPE:
            thisTickMenuEvents.exit = true;
            break;
        }
        break;
    }
    case SDL_CONTROLLERBUTTONDOWN:
    {
        SDL_GameControllerButton button = 
                static_cast<SDL_GameControllerButton>(evt.cbutton.button);
        switch (button)
        {
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            thisTickMenuEvents.up = true;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            thisTickMenuEvents.down = true;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            thisTickMenuEvents.left = true;
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            thisTickMenuEvents.right = true;
            break;
        case SDL_CONTROLLER_BUTTON_A:
            thisTickMenuEvents.select = true;
            break;
        case SDL_CONTROLLER_BUTTON_B:
            thisTickMenuEvents.exit = true;
            break;
        }
        break;
    }
    case SDL_KEYUP:
    case SDL_CONTROLLERBUTTONUP:
    default:
        break;
    }
}

bool ibase_menu_down(MenuInput input)
{
    switch (input)
    {
    case MenuInput::Up:
        return thisTickMenuEvents.up;
    case MenuInput::Down:
        return thisTickMenuEvents.down;
    case MenuInput::Left:
        return thisTickMenuEvents.left;
    case MenuInput::Right:
        return thisTickMenuEvents.right;
    case MenuInput::Select:
        return thisTickMenuEvents.select;
    case MenuInput::Exit:
        return thisTickMenuEvents.exit;
    }
    return false;
}

std::string ibase_get_key_name(int scanCode)
{
    return std::string(SDL_GetScancodeName(
                static_cast<SDL_Scancode>(scanCode)));
}

std::string ibase_get_pad_button_name(int buttonCode)
{
    return std::string(SDL_GameControllerGetStringForButton(
                static_cast<SDL_GameControllerButton>(buttonCode)));
}
