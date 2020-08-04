/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// m_title.hh: header for m_title.cc

#ifndef M_MTITLE_HH
#define M_MTITLE_HH

#include "image.hh"
#include "input.hh"

enum class TitleMode
{
    InitAnimationScale,
    MainMenu,
    HighScore,
    Options,
    OptionsControlKeyboard,
    OptionsControlGamepad
};

struct TitleScreen
{
    TitleScreen();
    void draw(Image &fb);
    void tick();
    void mainMenu(int cursorAt = 0);
    void highScores();
    void options(int cursorAt = 0);
    void controlOptions(bool gamepad);

    void drawOption(int option);
    void drawControlOption(int option);
    void decreaseSelectedOption();
    void increaseSelectedOption();
    bool cancelConfirm();

    void selectMenu();
    void exitMenu();

    TitleMode mode;
    ColorWindow flash;
    std::unique_ptr<Image> titleBackground;
    std::unique_ptr<Image> gameLogo;
    TextLayer<8,8> textLayer;
    int logoHeight;
    int logoStretchFrames;
    int ticks;
    bool drawCursor;
    int cursorX, cursorY, cursorOptionHeight;
    int cursorOption, cursorMaxOptions;
    Spritesheet menuSprites;
    int testMusic;
    int testSound;
    bool confirmReset;
    bool waitingControl;
    GameInput waitingControlFor;
};

#endif // M_MTITLE_HH
