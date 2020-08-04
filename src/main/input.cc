/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// input.cc: code for handling input from base input library

#include <string>
#include <vector>
#include "input.hh"
#include "ibase.hh"
#include "config.hh"

MenuInputState menuInput;
GameInputState gameInput;
static GameInputState gameInputLast;
GameInputState gameInputEdge;
GameInputControls gameInputKeyb;
GameInputControls gameInputPad;
const std::vector<MenuInput> menuInputs = {
    MenuInput::Up, MenuInput::Down, MenuInput::Left, MenuInput::Right,
    MenuInput::Select, MenuInput::Exit
};
const std::vector<GameInput> gameInputs = {
    GameInput::MoveUp, GameInput::MoveDown,
    GameInput::MoveLeft, GameInput::MoveRight,
    GameInput::Fire, GameInput::Drone,
    GameInput::WeaponUp, GameInput::WeaponDown,
    GameInput::SpeedUp, GameInput::SpeedDown, GameInput::Pause
};

static std::string controlName(GameInput input) 
{
    switch (input)
    {
    case GameInput::MoveUp:     return "Up";
    case GameInput::MoveDown:   return "Down";
    case GameInput::MoveLeft:   return "Left";
    case GameInput::MoveRight:  return "Right";
    case GameInput::Fire:       return "Fire";
    case GameInput::Drone:      return "Drone";
    case GameInput::WeaponUp:   return "WeaponUp";
    case GameInput::WeaponDown: return "WeaponDown";
    case GameInput::SpeedUp:    return "SpeedUp";
    case GameInput::SpeedDown:  return "SpeedDown";
    case GameInput::Pause:      return "Pause";
    default:                    return "";
    }
}

void ReadInputControls(ConfigFile &cfg)
{
    for (const GameInput& input : gameInputs)
    {
        gameInputKeyb.control(input) = cfg.get("ControlKey" + controlName(input),
                        ibase_default_keyb(input));
        gameInputPad.control(input) = cfg.get("ControlPad" + controlName(input),
                        ibase_default_pad(input));
    }
}

void SaveInputControls(ConfigFile &cfg)
{
    for (const GameInput& input : gameInputs)
    {
        cfg.set("ControlKey" + controlName(input),
                    gameInputKeyb.control(input));
        cfg.set("ControlPad" + controlName(input),
                    gameInputPad.control(input));
    }
}

void UpdateInput()
{
    for (const MenuInput& input : menuInputs)
        menuInput.value(input) = ibase_menu_down(input);
    gameInputLast = gameInput;
    for (const GameInput& input : gameInputs)
    {
        gameInput.value(input) = ibase_keyb_down(gameInputKeyb.control(input))
                              || ibase_pad_down(gameInputPad.control(input));
        gameInputEdge.value(input) = gameInput.value(input)
                                & !gameInputLast.value(input);
    }
}

std::string FormatKeyboardCode(int code)
{
    return ibase_get_key_name(code);
}

std::string FormatGamepadCode(int code)
{
    return ibase_get_pad_button_name(code);
}
