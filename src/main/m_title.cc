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

std::unique_ptr<TitleScreen> title;
constexpr int malpinxLogoTargetHeight = 144;

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
      logoStretch(malpinxLogoTargetHeight + 16 * 48),
      flash(0, 0, S_WIDTH, S_HEIGHT),
      ticks(0), drawCursor(false)
{
    menuSprites = LoadSpritesheet("menuspr");
    titleBackground = std::make_unique<Image>(LoadPIC("titlebg"));
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
    cursorMaxOptions = 9;
    if (cursorOption < 0 || cursorOption > cursorMaxOptions)
        cursorOption = 0;
    drawCursor = true;

    textLayer.writeString(menuFont, 2, 2, "OPTIONS");
    int y;  
    for (int i = 0; i <= cursorMaxOptions; ++i)
        drawOption(i);
}

const std::string difficultyNames[] = { "EASY", "NORMAL", "HARD" };

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
            "MUSIC                               ");
        textLayer.writeStringRightAlign(menuFont,
            rightX, y, musicEnabled ? "ON" : "OFF");
        return;
    case 2:
        textLayer.writeString(menuFont, leftX, y,
            "SOUND EFFECTS                       ");
        textLayer.writeStringRightAlign(menuFont,
            rightX, y, sfxEnabled ? "ON" : "OFF");
        return;
    case 3:
        textLayer.writeString(menuFont, leftX, y, 
            "SCALE                               ");
        textLayer.writeStringRightAlign(menuFont,
            rightX, y, std::to_string(backend->scale()));
        return;
    case 4:
        textLayer.writeString(menuFont, leftX, y, 
            "KEYBOARD CONTROL");
        return;
    case 5:
        textLayer.writeString(menuFont, leftX, y, 
            "GAME PAD CONTROL");
        return;
    case 6:
        textLayer.writeString(menuFont, leftX, y, 
            "MUSIC TEST                          ");
        textLayer.writeStringRightAlign(menuFont,
            rightX, y, std::to_string(testMusic));
        return;
    case 7:
        textLayer.writeString(menuFont, leftX, y, 
            "SOUND TEST                          ");
        textLayer.writeStringRightAlign(menuFont,
            rightX, y, std::to_string(testSound));
        return;
    case 8:
        textLayer.writeString(menuFont, leftX, y,
            "RESET TO DEFAULTS                   ");
        if (confirmReset)
            textLayer.writeString(menuFont, 24, y, "CONFIRM?");
        return;
    case 9:
        textLayer.writeString(menuFont, leftX, y, "BACK");
        return;
    }
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
    }
}

bool TitleScreen::cancelConfirm()
{
    if (mode == TitleMode::Options && confirmReset)
    {
        confirmReset = false;
        drawOption(8);
        return true;
    }
    return false;
}

void TitleScreen::selectMenu()
{
    switch (mode)
    {
    case TitleMode::MainMenu:
        PlayMenuSound(SoundEffect::MenuSelectOption);
        switch (cursorOption)
        {
        case 0:
            // TODO
        case 1:
            highScores();
            return;
        case 2:
            FadeOutSong();
            options();
            return;
        case 3:
            StopSong();
            QuitGame();
            return;
        }
        return;
    case TitleMode::Options:
        switch (cursorOption)
        {
        case 0:
        case 1:
        case 2:
            increaseSelectedOption();
            break;
        case 3:
            break;
        case 4:
            // keyb ctrl
            break;
        case 5:
            // gamepad ctrl
            break;
        case 6:
            StopSong();
            PlaySong(static_cast<MusicTrack>(testMusic));
            return;
        case 7:
            PlayMenuSound(testSound);
            return;
        case 8:
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
        case 9:
            exitMenu();
            return;
        }
        drawOption(cursorOption);
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
        case 2:
            increaseSelectedOption();
            break;
        case 3:
            backend->scale(std::max(1, backend->scale() - 1));
            break;
        case 6:
            testMusic = (testMusic - 1) & 31;
            break;
        case 7:
            testSound = (testSound - 1) & 255;
            break;
        case 4:
        case 5:
        case 8:
        case 9:
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
            musicEnabled = !musicEnabled;
            break;
        case 2:
            sfxEnabled = !sfxEnabled;
            break;
        case 3:
            backend->scale(std::min(8, backend->scale() + 1));
            break;
        case 6:
            testMusic = (testMusic + 1) & 31;
            break;
        case 7:
            testSound = (testSound + 1) & 255;
            break;
        case 4:
        case 5:
        case 8:
        case 9:
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
        break;
    case TitleMode::MainMenu:
    case TitleMode::HighScore:
    case TitleMode::Options:
        titleBackground->blit(fb);
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
        logoStretch -= 16;
        if (logoStretch <= malpinxLogoTargetHeight)
        {
            logoStretch = malpinxLogoTargetHeight;
            flash.flash(Color(S_MAXCLR, S_MAXCLR, S_MAXCLR));
            mainMenu();
        }
        break;
    case TitleMode::HighScore:
        if ((ticks % 4) == 0)
            flash.fade(1);
        break;
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
