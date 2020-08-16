/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// m_title.hh: header for m_title.cc

#ifndef M_MTITLE_HH
#define M_MTITLE_HH

#include <array>
#include "image.hh"
#include "input.hh"
#include "modes.hh"
#include "scores.hh"

enum class TitleMode
{
    InitAnimationScale,
    MainMenu,
    HighScore,
    Options,
    OptionsControlKeyboard,
    OptionsControlGamepad
};

class TitleScreen
{
public:
    TitleScreen();
    TitleScreen(const TitleScreen&) = delete;
    TitleScreen& operator=(TitleScreen) = delete;

    void draw(Image &fb);
    void tick();
    void mainMenu(int cursorAt = 0);
    void highScores();
    void updateHighScores();
    void updateEntryHighscore();
    void finishNameEntry();
    void nameEntry(DifficultyLevel diff, PlaybackMode pmode, int rank,
                    int stageNum, unsigned long score);
    void options(int cursorAt = 0);
    void controlOptions(bool gamepad);

    void drawOption(int option);
    void drawControlOption(int option);
    void decreaseSelectedOption();
    void increaseSelectedOption();
    bool cancelConfirm();

    void selectMenu();
    void exitMenu();

private:
    TitleMode mode;
    ColorWindow flash;
    std::unique_ptr<Image> titleBackground;
    std::unique_ptr<Image> gameLogo;
    TextLayer<8,8> textLayer;
    DifficultyLevel scoreDifficulty;
    PlaybackMode scorePmode;
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
    DifficultyLevel newScoreDifficulty;
    PlaybackMode newScorePmode;
    int highScoreIndex{-1};
    std::array<int, 3> nameEntryChars{0, 0, 0};
    int nameEntryCharIndex{-1};
    int nameEntryCharIndexCopy{-1};
    ScoreEntry highScoreEntry;
    int highScoreCharBlink{0};
};

#endif // M_MTITLE_HH
