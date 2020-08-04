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

struct Stage;

struct ShooterAssets
{
    std::shared_ptr<Spritesheet> playerShip;
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

struct Shooter
{
    std::vector<std::unique_ptr<Sprite>> spriteLayer0;
    std::vector<std::unique_ptr<Sprite>> spriteLayer1;
    std::vector<std::unique_ptr<Sprite>> spriteLayer2;
    std::vector<std::unique_ptr<Sprite>> spriteLayer3;

    std::unique_ptr<Stage> stage;
    TextLayer<8,8> hud;
    TextLayer<16,16> text;

    int stageNum{1};
    ShooterAssets assets;
    LayerScroll scroll;
    int xspeed{1};
    int yspeed{0};
    Image pauseBuffer{S_WIDTH, S_HEIGHT};
    bool paused;
    unsigned long score;
    unsigned long highScore;
    char lives{3};
    std::array<signed char, 6> weaponLevels{{0,-1,-1,-1,-1,-1}};
    char speed{2};
    int activeWeapon{0};
    int pauseCursor;
    int continues;
    int _nextSpriteID{0};
    Spritesheet menuSprites;

    void blit(Image &fb);
    // returns whether sprite is now dead
    bool tickSprite(Sprite &sprite);
    void updateSprites(std::vector<std::unique_ptr<Sprite>> &sprites);
    void tick();
    void drawHUD();
    void updateHUD(HUDElement element);
    void addScore(int points);
    void loadStage(int stageNum);
    int nextSpriteID() { return _nextSpriteID++; }
    void pauseGame();
    void unpauseGame();
    void endGame();
};

extern std::shared_ptr<Shooter> stg;

void StartNewGame();

#endif // M_MGAME_HH
