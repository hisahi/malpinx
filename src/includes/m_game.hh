/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// m_game.hh: header for m_game.cc

#ifndef M_MGAME_HH
#define M_MGAME_HH

#include <array>
#include <vector>
#include <memory>
#include <string>
#include "defs.hh"
#include "image.hh"
#include "sprite.hh"
#include "layer.hh"

enum class ExplosionSize;
struct Stage;
class PlayerSprite;

struct ShooterAssets
{
    std::shared_ptr<Spritesheet> playerShip;
    std::shared_ptr<Spritesheet> powerupSprites;
    std::shared_ptr<Spritesheet> bulletSprites;
};

enum class HUDElement
{
    Stage,
    Score,
    HighScore,
    WeaponIcons,
    WeaponLevels,
    WeaponName,
    Speed,
    Lives
};

struct ScreenPopup
{
    constexpr static int LENGTH = 256;
    int visibility{0};
    int ticks{0};
    bool permanent{false};
    Image back{S_WIDTH, 32};
    Image front{S_WIDTH, 32};

    void blit(Image &fb);
    void showString(std::string text);
    void showStage(int num);
    void showComplete();
    void showGameOver();
};

struct Shooter
{
    std::vector<std::unique_ptr<Sprite>> spriteLayer0;
    std::vector<std::unique_ptr<Sprite>> spriteLayer1;
    std::vector<std::unique_ptr<Sprite>> spriteLayer2;
    std::vector<std::unique_ptr<Sprite>> spriteLayer3;
    std::vector<std::unique_ptr<Sprite>> spriteLayer4;

    std::unique_ptr<Stage> stage;
    TextLayer<8,8> hud;
    TextLayer<16,16> text;

    int stageNum{1};
    ShooterAssets assets;
    LayerScroll scroll;
    std::unique_ptr<ScreenPopup> popup;
    Image gameArea{S_WIDTH, S_GHEIGHT};
    Image pauseBuffer{S_WIDTH, S_HEIGHT};
    bool paused{false};
    bool continueScreen{false};
    char lives{3};
    //std::array<signed char, 6> weaponLevels{{0,-1,-1,-1,-1,-1}};
    std::array<signed char, 6> weaponLevels{{0,0,0,0,0,0}};
    char speed{2};
    Spritesheet menuSprites;
    char continues;
    int xspeed{1};
    int yspeed{0};
    int maximumScrollY{0};
    FadeWindow fade{0, 0, S_WIDTH, S_GHEIGHT};

    void blit(Image &fb);
    // returns whether sprite is now dead
    void updateSprites(const int layer,
                        std::vector<std::unique_ptr<Sprite>> &sprites);
    PlayerSprite *getPlayer();
    void killPlayer();
    void controlTick();
    bool pauseTick();
    void tick();
    void spawnPlayer(bool respawn);
    void drawHUD();
    void updateHUD(HUDElement element);
    void updateYScroll();
    void addScore(int points);
    void loadStage(int stageNum);
    void endStage();
    void unloadStage();
    int nextSpriteID() { return _nextSpriteID++; }
    void explode(int centerX, int centerY, ExplosionSize size, bool quiet);
    void collectOneUp();
    void collectWeapon(int index);
    void tryContinue();
    void useContinue();
    void gameOver();
    void pauseGame();
    void unpauseGame();
    void endGame();
    int activeWeapon{0};
    int stageFade{0};
private:
    int nextStage{1};
    unsigned long score;
    unsigned long highScore;
    int pauseCursor;
    int _nextSpriteID{0};
    PlayerSprite *_player;
    int _respawnTicks;
    bool _isGameOver{false};
};

extern std::shared_ptr<Shooter> stg;

void StartNewGame();

#endif // M_MGAME_HH
