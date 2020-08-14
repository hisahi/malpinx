/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// m_title.cc: code for the title screen

#include <memory>
#include "defs.hh"
#include "layer.hh"
#include "render.hh"
#include "songs.hh"
#include "m_title.hh"
#include "fonts.hh"
#include "gamedata.hh"
#include "sfx.hh"
#include "input.hh"
#include "formats.hh"
#include "malpinx.hh"
#include "config.hh"
#include "m_game.hh"
#include "logic.hh"
#include "strutil.hh"

std::unique_ptr<TitleScreen> title;

void InitTitleScreen(bool instant)
{
    PlaySong(MusicTrack::Title);
    title = std::make_unique<TitleScreen>();
    if (instant)
        title->mainMenu();
    else
        FadeReset();
}

TitleScreen::TitleScreen()
    : textLayer(), mode(TitleMode::InitAnimationScale),
      flash(0, 0, S_WIDTH, S_HEIGHT),
      logoStretchFrames(128),
      ticks(0), drawCursor(false),
      testMusic(0), testSound(0), confirmReset(false)
{
    menuSprites = LoadSpritesheet("menuspr");
    titleBackground = std::make_unique<Image>(LoadPIC("titlebg"));
    gameLogo = std::make_unique<Image>(LoadPIC("gamelogo"));
    logoHeight = gameLogo->height();
}

void TitleScreen::mainMenu(int cursorAt /*= 0*/)
{
    mode = TitleMode::MainMenu;
    textLayer.clear();
    
    cursorOptionHeight = 16;
    cursorOption = cursorAt;
    cursorX = 16 * 8 - 4;
    cursorY = 16 * 8 - 4 + cursorAt * cursorOptionHeight;
    cursorMaxOptions = 3;
    if (cursorOption < 0 || cursorOption > cursorMaxOptions)
        cursorOption = 0;
    drawCursor = true;

    textLayer.writeString(menuFont, 18, 16, "GAME");
    textLayer.writeString(menuFont, 18, 18, "SCORES");
    textLayer.writeString(menuFont, 18, 20, "OPTIONS");
    textLayer.writeString(menuFont, 18, 22, "EXIT");
    textLayer.writeString(menuFont, 2, 28, "(C) 2020      HISAHI.");
    textLayer.writeString(menuFont, 28, 28, VERSION);
}

void TitleScreen::highScores()
{
    mode = TitleMode::HighScore;
    textLayer.clear();
    
    drawCursor = false;

    textLayer.writeString(menuFont, 2, 2, "HIGH SCORES");
    int y;
    for (int i = 0; i < 10; ++i)
    {
        y = 4 + i * 2;
        textLayer.writeString(menuFont, 4, y, "TODO");
        textLayer.writeString(menuFont, 24, y, "000000000000");
    }
}

void TitleScreen::options(int cursorAt /*= 0*/)
{
    mode = TitleMode::Options;
    textLayer.clear();
    
    drawCursor = true;
    cursorOptionHeight = 16;
    cursorOption = cursorAt;
    cursorX = 2 * 8 - 4;
    cursorY = 4 * 8 - 4 + cursorAt * cursorOptionHeight;
    cursorMaxOptions = 11;
    if (cursorOption < 0 || cursorOption > cursorMaxOptions)
        cursorOption = 0;
    drawCursor = true;

    textLayer.writeString(menuFont, 2, 2, "OPTIONS");
    for (int i = 0; i <= cursorMaxOptions; ++i)
        drawOption(i);
}

void TitleScreen::controlOptions(bool gamepad)
{
    mode = gamepad ? TitleMode::OptionsControlGamepad
                    : TitleMode::OptionsControlKeyboard;
    textLayer.clear();
    
    drawCursor = true;
    cursorOptionHeight = 16;
    cursorOption = 0;
    cursorX = 2 * 8 - 4;
    cursorY = 4 * 8 - 4;
    cursorMaxOptions = gameInputs.size();
    drawCursor = true;

    textLayer.writeString(menuFont, 2, 2,
        gamepad ? "GAMEPAD OPTIONS" : "KEYBOARD OPTIONS");
    for (int i = 0; i <= cursorMaxOptions; ++i)
        drawControlOption(i);
}

const std::string difficultyNames[] = { "EASY", "NORMAL", "HARD", "BIZARRE" };
const std::string playbackModeNames[] = { "STANDARD", "INFINITE" };

void TitleScreen::drawOption(int option)
{
    int y = 4 + option * 2;
    int leftX = 4;
    int rightX = 33;
    switch (option)
    {
    case 0:
        textLayer.writeString(menuFont, leftX, y,
            "DIFFICULTY                          ");
        textLayer.writeStringRightAlign(menuFont,
            rightX, y, difficultyNames[static_cast<int>(difficulty)]);
        return;
    case 1:
        textLayer.writeString(menuFont, leftX, y,
            "MODE                                ");
        textLayer.writeStringRightAlign(menuFont,
            rightX, y, playbackModeNames[static_cast<int>(pmode)]);
        return;
    case 2:
        textLayer.writeString(menuFont, leftX, y, 
            "CONTINUES                           ");
        textLayer.writeStringRightAlign(menuFont,
            rightX, y, std::to_string(startContinues));
        return;
    case 3:
        textLayer.writeString(menuFont, leftX, y,
            "MUSIC                               ");
        textLayer.writeStringRightAlign(menuFont,
            rightX, y, musicEnabled ? "ON" : "OFF");
        return;
    case 4:
        textLayer.writeString(menuFont, leftX, y,
            "SOUND EFFECTS                       ");
        textLayer.writeStringRightAlign(menuFont,
            rightX, y, sfxEnabled ? "ON" : "OFF");
        return;
    case 5:
        textLayer.writeString(menuFont, leftX, y, 
            "SCALE                               ");
        textLayer.writeStringRightAlign(menuFont,
            rightX, y, std::to_string(backend->scale()));
        return;
    case 6:
        textLayer.writeString(menuFont, leftX, y, 
            "KEYBOARD CONTROL");
        return;
    case 7:
        textLayer.writeString(menuFont, leftX, y, 
            "GAME PAD CONTROL");
        return;
    case 8:
        textLayer.writeString(menuFont, leftX, y, 
            "MUSIC TEST                          ");
        textLayer.writeStringRightAlign(menuFont,
            rightX, y, std::to_string(testMusic));
        return;
    case 9:
        textLayer.writeString(menuFont, leftX, y, 
            "SOUND TEST                          ");
        textLayer.writeStringRightAlign(menuFont,
            rightX, y, std::to_string(testSound));
        return;
    case 10:
        textLayer.writeString(menuFont, leftX, y,
            "RESET TO DEFAULTS                   ");
        if (confirmReset)
            textLayer.writeString(menuFont, 24, y, "CONFIRM?");
        return;
    case 11:
        textLayer.writeString(menuFont, leftX, y, "BACK");
        return;
    }
}

const std::string controlNames[] = { 
    "UP", "DOWN", "LEFT", "RIGHT", "FIRE", "SIGMA",
    "FASTER", "SLOWER", "PAUSE"
};

void TitleScreen::drawControlOption(int option)
{
    int y = 4 + option * 2;
    int leftX = 4;
    int rightX = 33;
    if (option == gameInputs.size())
    {
        textLayer.writeString(menuFont, leftX, y, "BACK");
        return;
    }
    std::string inputName = controlNames[option];
    GameInput input = static_cast<GameInput>(option);
    std::string control;
    if (waitingControl && static_cast<int>(waitingControlFor) == option)
        control = "...";
    else if (mode == TitleMode::OptionsControlKeyboard)
        control = FormatKeyboardCode(gameInputKeyb.control(input));
    else if (mode == TitleMode::OptionsControlGamepad)
        control = FormatGamepadCode(gameInputPad.control(input));
    textLayer.writeString(menuFont, leftX, y,
                        leftAlignPad(inputName, 36));
    textLayer.writeStringRightAlign(menuFont, rightX, y + 1,
                        rightAlignPad(control, 36));
}

void TitleScreen::exitMenu()
{
    switch (mode)
    {
    case TitleMode::MainMenu:
        return;
    case TitleMode::HighScore:
        mainMenu(1);
        return;
    case TitleMode::Options:
        if (cancelConfirm())
            return;
        StopSong();
        SaveConfig();
        mainMenu(2);
        PlaySong(MusicTrack::Title);
        return;
    case TitleMode::OptionsControlKeyboard:
        if (waitingControl)
        {
            waitingControl = false;
            drawControlOption(cursorOption);
            return;
        }
        options(6);
        return;
    case TitleMode::OptionsControlGamepad:
        if (waitingControl)
        {
            waitingControl = false;
            drawControlOption(cursorOption);
            return;
        }
        options(7);
        return;
    }
}

bool TitleScreen::cancelConfirm()
{
    if (mode == TitleMode::Options && confirmReset)
    {
        confirmReset = false;
        drawOption(10);
        return true;
    }
    return false;
}

void TitleScreen::selectMenu()
{
    switch (mode)
    {
    case TitleMode::MainMenu:
        switch (cursorOption)
        {
        case 0:
            FadeOutSong();
            JumpMode(GameMode::Game, []() {
                title = nullptr;
                StartNewGame();
            });
            break;
        case 1:
            highScores();
            break;
        case 2:
            FadeOutSong();
            options();
            break;
        case 3:
            StopSong();
            QuitGame();
            return;
        }
        PlayMenuSound(SoundEffect::MenuSelectOption);
        return;
    case TitleMode::HighScore:
        PlayMenuSound(SoundEffect::MenuSelectOption);
        exitMenu();
        break;
    case TitleMode::Options:
        switch (cursorOption)
        {
        case 0:
        case 1:
        case 3:
        case 4:
            increaseSelectedOption();
            break;
        case 2:
        case 5:
            break;
        case 6:
            PlayMenuSound(SoundEffect::MenuSelectOption);
            controlOptions(false);
            return;
        case 7:
            PlayMenuSound(SoundEffect::MenuSelectOption);
            controlOptions(true);
            return;
        case 8:
            StopSong();
            PlaySong(static_cast<MusicTrack>(testMusic));
            return;
        case 9:
            PlayMenuSound(testSound);
            return;
        case 10:
            if (confirmReset)
            {
                confirmReset = false;
                RestoreConfigDefaults();
                for (int i = 0; i <= cursorMaxOptions; ++i)
                    drawOption(i);
                break;
            }
            confirmReset = true;
            drawOption(8);
            break;
        case 11:
            exitMenu();
            return;
        }
        drawOption(cursorOption);
        PlayMenuSound(SoundEffect::MenuSelectOption);
        return;
    case TitleMode::OptionsControlKeyboard:
    case TitleMode::OptionsControlGamepad:
        if (cursorOption == gameInputs.size())
            exitMenu();
        else
        {
            waitingControl = true;
            waitingControlFor = static_cast<GameInput>(cursorOption);
            drawControlOption(cursorOption);
            ibase_clear_next();
        }
        PlayMenuSound(SoundEffect::MenuSelectOption);
        return;
    }
}

void TitleScreen::decreaseSelectedOption()
{
    switch (mode)
    {
    case TitleMode::Options:
        switch (cursorOption)
        {
        case 0:
        {
            int newDl = static_cast<int>(difficulty) - 1;
            if (newDl < 0) newDl = maxDifficultyLevel;
            difficulty = static_cast<DifficultyLevel>(newDl);
            break;
        }
        case 1:
        {
            int newPm = static_cast<int>(pmode) - 1;
            if (newPm < 0) newPm = maxPlaybackMode;
            pmode = static_cast<PlaybackMode>(newPm);
            break;
        }
        case 2:
            if (startContinues > 0)
                --startContinues;
            break;
        case 3:
        case 4:
            increaseSelectedOption();
            break;
        case 5:
            backend->scale(std::max(1, backend->scale() - 1));
            break;
        case 8:
            testMusic = (testMusic - 1) & 31;
            StopSong();
            break;
        case 9:
            testSound = (testSound - 1) & 255;
            StopSounds();
            break;
        case 6:
        case 7:
        case 10:
        case 11:
            break;
        }
        drawOption(cursorOption);
        PlayMenuSound(SoundEffect::MenuSelectOption);
        return;
    }
}

void TitleScreen::increaseSelectedOption()
{
    switch (mode)
    {
    case TitleMode::Options:
        switch (cursorOption)
        {
        case 0:
        {
            int newDl = static_cast<int>(difficulty) + 1;
            if (newDl > maxDifficultyLevel) newDl = 0;
            difficulty = static_cast<DifficultyLevel>(newDl);
            break;
        }
        case 1:
        {
            int newPm = static_cast<int>(pmode) + 1;
            if (newPm > maxPlaybackMode) newPm = 0;
            pmode = static_cast<PlaybackMode>(newPm);
            break;
        }
        case 2:
            startContinues = std::min(startContinues + 1, 9);
            break;
        case 3:
            musicEnabled = !musicEnabled;
            break;
        case 4:
            sfxEnabled = !sfxEnabled;
            break;
        case 5:
            backend->scale(std::min(8, backend->scale() + 1));
            break;
        case 8:
            testMusic = (testMusic + 1) & 31;
            break;
        case 9:
            testSound = (testSound + 1) & 255;
            break;
        case 6:
        case 7:
        case 10:
        case 11:
            break;
        }
        drawOption(cursorOption);
        PlayMenuSound(SoundEffect::MenuSelectOption);
        return;
    }
}

void TitleScreen::draw(Image &fb)
{
    switch (mode)
    {
    case TitleMode::InitAnimationScale:
    {
        fb.subtract(Color(1, 1, 1));
        int yScale = 256 - (logoStretchFrames << 1);
        int logoWidth = gameLogo->width();
        int y = -128;
        int yt;
        for (;;)
        {
            yt = ((y - 32) * yScale) >> 8;
            if (y >= 0 && yt >= 0)
            {
                if (yt >= logoHeight || y >= S_HEIGHT)
                    break;
                gameLogo->blit(fb, 32, y, 0, yt, logoWidth, 1);
            }
            ++y;
        }
        break;
    }
    case TitleMode::MainMenu:
        titleBackground->blit(fb);
        gameLogo->blit(fb, 32, 32);
        goto rest;
    case TitleMode::HighScore:
    case TitleMode::Options:
    case TitleMode::OptionsControlKeyboard:
    case TitleMode::OptionsControlGamepad:
        titleBackground->blit(fb);
    rest:
        textLayer.blit(fb);
        flash.blit(fb);
        if (drawCursor)
            menuSprites.blit(fb, 0, cursorX, cursorY);
        break;
    }
}

void TitleScreen::tick()
{
    ++ticks;
    if (menuInput.exit)
        exitMenu();
    switch (mode)
    {
    case TitleMode::InitAnimationScale:
        if (--logoStretchFrames <= 0 || menuInput.select)
        {
            flash.flash(Color(S_MAXCLR, S_MAXCLR, S_MAXCLR));
            mainMenu();
        }
        break;
    case TitleMode::HighScore:
        if ((ticks % 4) == 0)
            flash.fade(1);
        if (menuInput.select)
            selectMenu();
        break;
    case TitleMode::OptionsControlKeyboard:
        if (waitingControl)
        {
            int newKey;
            if (ibase_next_keyb_input(newKey))
            {
                PlayMenuSound(SoundEffect::MenuSelectOption);
                waitingControl = false;
                gameInputKeyb.control(waitingControlFor) = newKey;
                drawControlOption(cursorOption);
                return;
            }
        }
    case TitleMode::OptionsControlGamepad:
        if (waitingControl)
        {
            int newButton;
            if (ibase_next_pad_input(newButton))
            {
                PlayMenuSound(SoundEffect::MenuSelectOption);
                waitingControl = false;
                gameInputPad.control(waitingControlFor) = newButton;
                drawControlOption(cursorOption);
                return;
            }
        }
    case TitleMode::Options:
    case TitleMode::MainMenu:
        if ((ticks % 4) == 0)
            flash.fade(1);
        if (drawCursor)
        {
            if (menuInput.up)
            {
                cancelConfirm();
                --cursorOption;
                cursorY -= cursorOptionHeight;
                if (cursorOption < 0)
                {
                    cursorOption = cursorMaxOptions;
                    cursorY += (cursorMaxOptions + 1) * cursorOptionHeight;
                }
                PlayMenuSound(SoundEffect::MenuChangeOption);
            }
            if (menuInput.down)
            {
                cancelConfirm();
                ++cursorOption;
                cursorY += cursorOptionHeight;
                if (cursorOption > cursorMaxOptions)
                {
                    cursorOption = 0;
                    cursorY -= (cursorMaxOptions + 1) * cursorOptionHeight;
                }
                PlayMenuSound(SoundEffect::MenuChangeOption);
            }
            if (menuInput.left)
                decreaseSelectedOption();
            if (menuInput.right)
                increaseSelectedOption();
            if (menuInput.select)
                selectMenu();
        }
        break;
    }
}

void UnloadTitleScreen()
{
    title = nullptr;
}

void DrawTitleFrame(Image &fb)
{
    title->draw(fb);
}

void RunTitleFrame()
{
    title->tick();
}
