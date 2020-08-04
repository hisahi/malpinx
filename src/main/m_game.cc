/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// m_game.cc: core code for main game

#include <iostream>
#include <memory>
#include "image.hh"
#include "m_game.hh"
#include "formats.hh"
#include "player.hh"
#include "input.hh"
#include "sfx.hh"
#include "fonts.hh"
#include "modes.hh"
#include "logic.hh"
#include "songs.hh"
#include "strutil.hh"
#include "config.hh"

std::shared_ptr<Shooter> stg;

static void LoadAssets()
{
    stg->assets.playerShip = std::make_shared<Spritesheet>(
                                    LoadSpritesheet("ship"));
    stg->menuSprites = LoadSpritesheet("menuspr");
}

static void MakePlayerShip()
{
    stg->spriteLayer1.push_back(
        stg->assets.playerShip->makeUniqueSprite<PlayerSprite>(
            stg->nextSpriteID(), 0, 48, 128,
            SPRITE_COLLIDE_SPRITES | SPRITE_COLLIDE_FG | SPRITE_NOSCROLL,
            stg->assets.playerShip, stg));
}

void StartNewGame()
{
    stg = std::make_shared<Shooter>();
    LoadAssets();
    MakePlayerShip();
    stg->continues = startContinues;
    stg->drawHUD();
    stg->stage = std::make_unique<Stage>(*stg);
}

void UnloadGame()
{
    stg = nullptr;
}

void Shooter::blit(Image &fb)
{
    if (paused)
    {
        pauseBuffer.blit(fb);
        menuSprites.blit(fb, 0, 124, 124 + 16 * pauseCursor);
        return;
    }
    fb.clear();
    for (auto &bl : stage->backgroundLayers)
        bl->blit(fb, stg->scroll);
    hud.blit(fb);
    for (auto &sprite : spriteLayer0)
        if (!sprite->hasFlag(SPRITE_NODRAW))
            sprite->blit(fb);
    for (auto &fl : stage->terrainLayers)
        fl->blit(fb, stg->scroll);
    for (auto &sprite : spriteLayer1)
        if (!sprite->hasFlag(SPRITE_NODRAW))
            sprite->blit(fb);
    for (auto &sprite : spriteLayer2)
        if (!sprite->hasFlag(SPRITE_NODRAW))
            sprite->blit(fb);
    for (auto &sprite : spriteLayer3)
        if (!sprite->hasFlag(SPRITE_NODRAW))
            sprite->blit(fb);
    for (auto &bl : stage->foregroundLayers)
        bl->blit(fb, stg->scroll);
    text.blit(fb);
}

inline bool Shooter::tickSprite(Sprite &sprite)
{
    sprite.tick();
    if (sprite.hasFlag(SPRITE_COLLIDE_SPRITES))
        sprite.computeCollisionGrid();
    if (!sprite.hasFlag(SPRITE_NOSCROLL))
        sprite.move(-xspeed, -yspeed);
    if (!sprite.hasFlag(SPRITE_SURVIVE_OFF_SCREEN)
            && sprite.offScreenDistance() > 16)
        sprite.kill();
    return sprite.isDead();
}

inline void Shooter::updateSprites(
        std::vector<std::unique_ptr<Sprite>> &sprites)
{
    for (auto it = sprites.begin(); it != sprites.end(); ++it)
        if (tickSprite(**it))
            it = sprites.erase(it);
}

void Shooter::addScore(int points)
{
    if ((score += points) > highScore)
    {
        highScore = score;
        updateHUD(HUDElement::HighScore);
    }
    updateHUD(HUDElement::Score);
}

void Shooter::drawHUD()
{
    std::string clear = std::string(TextLayer<8,8>::Columns, ' ');
    for (int y = 0; y < 4; ++y)
        hud.writeString(menuFont, 0, y, clear);
    hud.writeString(menuFont, 1, 0, "STAGE");
    hud.writeString(menuFont, 1, 2, "HI");
    hud.writeString(menuFont, 1, 3, "SC");
    hud.writeString(menuFont, 28, 1, "SPEED");
    hud.writeString(hudFont, 34, 2, "\x42");
    updateHUD(HUDElement::Stage);
    updateHUD(HUDElement::Score);
    updateHUD(HUDElement::HighScore);
    updateHUD(HUDElement::WeaponIcons);
    updateHUD(HUDElement::WeaponLevels);
    updateHUD(HUDElement::WeaponName);
    updateHUD(HUDElement::Speed);
    updateHUD(HUDElement::Lives);
}

static const std::array<std::string, 6> weaponNames = {
    "  PULSE   ",
    "  SPRAY   ",
    "  CURVE   ",
    "  BEAM    ",
    "  FLAK    ",
    "  TRACK   "
};

void Shooter::updateHUD(HUDElement element)
{
    switch (element)
    {
    case HUDElement::Stage:
        hud.writeString(menuFont, 7, 0, std::to_string(stageNum));
        break;
    case HUDElement::Score:
        hud.writeString(menuFont, 5, 3,
                        rightAlignPad(std::to_string(score), 8));
        break;
    case HUDElement::HighScore:
        hud.writeString(menuFont, 5, 2,
                        rightAlignPad(std::to_string(highScore), 8));
        break;
    case HUDElement::WeaponIcons:
    {
        std::string upper = "            ";
        std::string lower = "            ";
        for (int i = 0; i < weaponLevels.size(); ++i)
        {
            if (weaponLevels[i] >= 0)
            {
                upper[i*2] = 0x24 + i*2;
                upper[i*2+1] = 0x25 + i*2;
                lower[i*2] = 0x34 + i*2;
                lower[i*2+1] = 0x35 + i*2;
            }
        }
        hud.writeString(hudFont, 14, 0, upper);
        hud.writeString(hudFont, 14, 1, lower);
        hud.writeString(hudFont, 14 + activeWeapon * 2, 0, "\x06\x07");
        hud.writeString(hudFont, 14 + activeWeapon * 2, 1, "\x16\x17");
        break;
    }
    case HUDElement::WeaponLevels:
        for (int i = 0; i < weaponLevels.size(); ++i)
        {
            if (weaponLevels[i] >= 0)
            {
                hud.writeChar(hudFont, 14 + i * 2, 2, '\x43');
                hud.writeChar(menuFont, 15 + i * 2, 2, '1' + weaponLevels[i]);
            }
        }
        break;
    case HUDElement::WeaponName:
        hud.writeString(menuFont, 15, 3, weaponNames[activeWeapon]);
        break;
    case HUDElement::Speed:
        for (int i = 1; i <= 4; ++i)
            hud.writeChar(hudFont, 33 + i, 1, speed >= i ? '\x41' : ' ');
        break;
    case HUDElement::Lives:
        hud.writeString(menuFont, 36, 2,
                        rightAlignPad(std::to_string(lives + 1), 2));
        break;
    }
}

void Shooter::pauseGame()
{
    fb_back.blit(pauseBuffer);
    pauseBuffer.subtract(Color(8, 8, 8));
    hud.writeString(menuFont, 18, 10, "PAUSE");
    hud.writeString(menuFont, 18, 16, "CONTINUE");
    hud.writeString(menuFont, 18, 18, "EXIT");
    hud.blit(pauseBuffer);
    pauseCursor = 0;
    PauseSound();
}

void Shooter::unpauseGame()
{
    std::string clear = std::string(TextLayer<8,8>::Columns, ' ');
    for (int y = 4; y < TextLayer<8,8>::Rows; ++y)
        hud.writeString(menuFont, 0, y, clear);
    ResumeSound();
}

void Shooter::endGame()
{
    StopSong();
    StopSounds();
    JumpMode(GameMode::TitleScreen, []() 
    {   
        InitTitleScreen(false);
        UnloadGame();
    });
}

void Shooter::tick()
{
    if (gameInputEdge.pause || menuInput.exit)
    {
        if (paused = !paused)
            pauseGame();
        else
            unpauseGame();
        PlayMenuSound(SoundEffect::Pause);
    }
    if (paused)
    {
        if (pauseCursor > 0 && menuInput.up)
        {
            --pauseCursor;
            PlayMenuSound(SoundEffect::MenuChangeOption);
        }
        if (pauseCursor < 1 && menuInput.down)
        {
            ++pauseCursor;
            PlayMenuSound(SoundEffect::MenuChangeOption);
        }
        if (menuInput.select)
        {
            PlayMenuSound(SoundEffect::MenuSelectOption);
            switch (pauseCursor)
            {
            case 0:
                PlayMenuSound(SoundEffect::Pause);
                paused = false;
                unpauseGame();
                break;
            case 1:
                StartFadeOut([this]() { endGame(); });
                break;
            }
        }
        return;
    }
    if (gameInputEdge.speedUp && speed < 4)
    {
        ++speed;
        updateHUD(HUDElement::Speed);
        PlayMenuSound(SoundEffect::SpeedChange);
    }
    if (gameInputEdge.speedDown && speed > 1)
    {
        --speed;
        updateHUD(HUDElement::Speed);
        PlayMenuSound(SoundEffect::SpeedChange);
    }
    if (gameInputEdge.weaponUp)
    {
        int oldWeapon = activeWeapon;
        do
        {
            activeWeapon = (activeWeapon + 1) % weaponLevels.size();
        }
        while (weaponLevels[activeWeapon] < 0);
        if (oldWeapon != activeWeapon)
        {
            updateHUD(HUDElement::WeaponIcons);
            updateHUD(HUDElement::WeaponName);
            PlaySound(SoundEffect::WeaponChange);
        }
    }
    if (gameInputEdge.weaponDown)
    {
        int oldWeapon = activeWeapon;
        do
        {
            if (--activeWeapon < 0)
                activeWeapon = weaponLevels.size() - 1;
        }
        while (weaponLevels[activeWeapon] < 0);
        if (oldWeapon != activeWeapon)
        {
            updateHUD(HUDElement::WeaponIcons);
            updateHUD(HUDElement::WeaponName);
            PlaySound(SoundEffect::WeaponChange);
        }
    }
    stage->spawnSprites(scroll);
    updateSprites(spriteLayer0);
    updateSprites(spriteLayer1);
    updateSprites(spriteLayer2);
    updateSprites(spriteLayer3);
    scroll.x += xspeed;
    scroll.y += yspeed;
}

void RenderGame(Image &fb)
{
    stg->blit(fb);
}

void DoGameTick()
{
    stg->tick();
}
