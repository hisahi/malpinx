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
#include "modes.hh"

enum class ExplosionSize;
enum class PowerupType;
struct Stage;
class PlayerSprite;

constexpr int DEFAULT_LIVES = 3;

struct ShooterAssets
{
    std::shared_ptr<Spritesheet> playerShip;
    std::shared_ptr<Spritesheet> powerupSprites;
    std::shared_ptr<Spritesheet> bulletSprites;
    std::shared_ptr<Spritesheet> sigma;
    std::shared_ptr<Spritesheet> enemy01;
    std::shared_ptr<Spritesheet> enemy02;
    std::shared_ptr<Spritesheet> enemy03;
    std::shared_ptr<Spritesheet> enemy04;
    std::shared_ptr<Spritesheet> enemy05;
    std::shared_ptr<Spritesheet> enemy06;
    std::shared_ptr<Spritesheet> boss1a;
};

enum class HUDElement
{
    Stage,
    Score,
    HighScore,
    WeaponName,
    SigmaCount,
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
    void clear();
    void showString(std::string text);
    void showStage(int num);
    void showComplete();
    void showGameOver();
};

struct Shooter
{
    Shooter() = default;
    Shooter(const Shooter&) = delete;
    Shooter(Shooter&&) = default;
    Shooter& operator=(Shooter) = delete;

    std::vector<std::unique_ptr<Sprite>> spriteLayer0;
    std::vector<std::unique_ptr<Sprite>> spriteLayer1;
    std::vector<std::unique_ptr<Sprite>> spriteLayer2;
    std::vector<std::unique_ptr<Sprite>> spriteLayer3;
    std::vector<std::unique_ptr<Sprite>> spriteLayer4;
    std::unique_ptr<PlayerSprite> player;

    std::unique_ptr<Stage> stage;
    TextLayer<8,8> hud;
    TextLayer<8,8> menu;
    TextLayer<16,16> text;

    int stageNum{1};
    ShooterAssets assets;
    LayerScroll scroll;
    Fix xSpeed;
    std::unique_ptr<ScreenPopup> popup;
    Image gameArea{S_WIDTH, S_GHEIGHT};
    Image pauseBuffer{S_WIDTH, S_HEIGHT};
    bool paused{false};
    bool continueScreen{false};
    bool usedContinue{false};
    char lives{DEFAULT_LIVES};
    std::array<signed char, 4> weaponLevels{{0,-1,-1,-1}};
    char playerSpeed{2};
    Spritesheet menuSprites;
    char continues;
    Fix maximumScrollY{0};
    int lastPlayerY;
    int sigmas{1};
    FadeWindow fade{0, 0, S_WIDTH, S_GHEIGHT};
    DifficultyLevel difficulty;
    PlaybackMode pmode;

    void blit(Image &fb);
    void blitPlayer(Image &fb, int oy);
    void updateSprites(const int layer,
                        std::vector<std::unique_ptr<Sprite>> &sprites);
    void killPlayer();
    void controlTick();
    bool pauseTick();
    void tick();
    void spawnPlayer(bool respawn);
    void respawnPlayer();
    void drawHUD();
    void updateHUD(HUDElement element);
    void updateYScroll();
    void addScore(int points);
    void loadStage(int stageNum);
    void endStage();
    void unloadStage();
    int nextSpriteID() { return _nextSpriteID++; }
    void nextBonus(Fix x, Fix y);
    void explode(Fix centerX, Fix centerY, ExplosionSize size, bool quiet);
    void spawnScore(Fix x, Fix y, int score);
    inline bool isPlayerAlive() { return static_cast<bool>(player); }
    Fix2D vecToPlayer(Fix x, Fix y);
    void spawnPowerup(Fix x, Fix y, PowerupType type);
    bool collectOneUp();
    bool collectWeapon(int index);
    bool collectDrone();
    bool collectSigma();
    bool useSigma();
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
    int _nextBonus{0};
    unsigned long score;
    unsigned long highScore;
    int pauseCursor;
    int totalFrames{0};
    int _nextSpriteID{0};
    int _respawnTicks;
    bool _isGameOver{false};
};

extern std::shared_ptr<Shooter> stg;

void StartNewGame();

#endif // M_MGAME_HH
