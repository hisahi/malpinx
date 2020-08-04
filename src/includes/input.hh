/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// input.hh: includes for input.cc

#ifndef M_INPUT_HH
#define M_INPUT_HH

#include <string>
#include <vector>
#include <defs.hh>
#include <cfg.hh>

enum class MenuInput
{
    Up,
    Down,
    Left,
    Right,
    Select,
    Exit
};

enum class GameInput
{
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Fire,
    Drone,
    WeaponUp,
    WeaponDown,
    SpeedUp,
    SpeedDown,
    Pause,
};

struct MenuInputState
{
    bool up;
    bool down;
    bool left;
    bool right;
    bool select;
    bool exit;

    bool &value(MenuInput inp)
    {
        switch (inp)
        {
        case MenuInput::Up:     return up;
        case MenuInput::Down:   return down;
        case MenuInput::Left:   return left;
        case MenuInput::Right:  return right;
        case MenuInput::Select: return select;
        case MenuInput::Exit:   return exit;
        }
        return exit;
    }
};

struct GameInputState
{
    bool up;
    bool down;
    bool left;
    bool right;
    bool fire;
    bool drone;
    bool weaponUp;
    bool weaponDown;
    bool speedUp;
    bool speedDown;
    bool pause;

    bool &value(GameInput inp)
    {
        switch (inp)
        {
        case GameInput::MoveUp:     return up;
        case GameInput::MoveDown:   return down;
        case GameInput::MoveLeft:   return left;
        case GameInput::MoveRight:  return right;
        case GameInput::Fire:       return fire;
        case GameInput::Drone:      return drone;
        case GameInput::WeaponUp:   return weaponUp;
        case GameInput::WeaponDown: return weaponDown;
        case GameInput::SpeedUp:    return speedUp;
        case GameInput::SpeedDown:  return speedDown;
        case GameInput::Pause:      return pause;
        }
        return pause;
    }
};

struct GameInputControls
{
    int up;
    int down;
    int left;
    int right;
    int fire;
    int drone;
    int weaponUp;
    int weaponDown;
    int speedUp;
    int speedDown;
    int pause;

    int &control(GameInput inp)
    {
        switch (inp)
        {
        case GameInput::MoveUp:     return up;
        case GameInput::MoveDown:   return down;
        case GameInput::MoveLeft:   return left;
        case GameInput::MoveRight:  return right;
        case GameInput::Fire:       return fire;
        case GameInput::Drone:      return drone;
        case GameInput::WeaponUp:   return weaponUp;
        case GameInput::WeaponDown: return weaponDown;
        case GameInput::SpeedUp:    return speedUp;
        case GameInput::SpeedDown:  return speedDown;
        case GameInput::Pause:      return pause;
        }
        return pause;
    }
};

extern MenuInputState menuInput;
extern GameInputState gameInput;
extern GameInputState gameInputEdge;
extern GameInputControls gameInputKeyb;
extern GameInputControls gameInputPad;
extern const std::vector<MenuInput> menuInputs;
extern const std::vector<GameInput> gameInputs;

void ReadInputControls(ConfigFile &cfg);
void SaveInputControls(ConfigFile &cfg);
void RestoreInputDefaults(ConfigFile &cfg);

void UpdateInput();
std::string FormatKeyboardCode(int code);
std::string FormatGamepadCode(int code);

#endif // M_INPUT_HH
