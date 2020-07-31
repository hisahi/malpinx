/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// m_title.hh: header for m_title.cc

#ifndef M_MTITLE_HH
#define M_MTITLE_HH

#include "layer.hh"

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

    void drawOption(int option);
    void decreaseSelectedOption();
    void increaseSelectedOption();
    bool cancelConfirm();

    void selectMenu();
    void exitMenu();

    TitleMode mode;
    ColorWindow flash;
    std::unique_ptr<Image> titleBackground;
    TextLayer<8,8> textLayer;
    int logoStretch;
    int ticks;
    bool drawCursor;
    int cursorX, cursorY, cursorOptionHeight;
    int cursorOption, cursorMaxOptions;
    Spritesheet menuSprites;
    int testMusic;
    int testSound;
    bool confirmReset;
};

#endif // M_MTITLE_HH
