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
#include "explode.hh"
#include "maths.hh"

std::shared_ptr<Shooter> stg;
static constexpr int STAGE_COUNT = 6;

static void LoadAssets()
{
    stg->assets.playerShip = std::make_shared<Spritesheet>(
                                    LoadSpritesheet("ship"));
    stg->assets.powerupSprites = std::make_shared<Spritesheet>(
                                    LoadSpritesheet("powerups"));
    stg->assets.bulletSprites = std::make_shared<Spritesheet>(
                                    LoadSpritesheet("bullets"));
    stg->menuSprites = LoadSpritesheet("menuspr");
}

void StartNewGame()
{
    stg = std::make_shared<Shooter>();
    LoadAssets();
    stg->spawnPlayer(false);
    stg->continues = startContinues;
    stg->drawHUD();
    stg->popup = std::make_unique<ScreenPopup>();
    stg->fade.fadeOut(S_MAXCLR);
}

void UnloadGame()
{
    stg = nullptr;
}

inline void ScreenPopup::blit(Image &fb)
{
    if (!ticks) return;
    if (!permanent && ticks < ScreenPopup::LENGTH / 2)
        visibility = ticks;
    else
        visibility = ScreenPopup::LENGTH - ticks;
    if (visibility)
    {
        back.blit(front);
        int n = std::max(0, S_MAXCLR - visibility);
        front.subtract(Color(n, n, n));
        front.blitAdditive(fb, 0, 80, 0, 0, S_WIDTH, 32);
    }
    if (!--ticks)
        visibility = 0;
}

void ScreenPopup::showString(std::string text)
{
    ticks = ScreenPopup::LENGTH;
    visibility = 0;
    back.clear();
    int sx = (S_WIDTH / 2) - text.length() * 8;
    for (char &c : text)
    {
        bigFont.blitFast(back, c, sx, 0);
        sx += 16;
    }
    permanent = false;
}

void ScreenPopup::showStage(int num) 
{
    if (num == STAGE_COUNT) // FINAL STAGE
        showString(" \x09\x0b\x0e\x06\x0c \x12\x13\x06\x0a\x08");
    else
        showString(" \x12\x13\x06\x0a\x08 \x01" + static_cast<char>(num));
}

void ScreenPopup::showComplete()
{
    // COMPLETE
    showString(" \x07\x0f\x0d\x10\x0c\x08\x13\x08");
}

void ScreenPopup::showGameOver()
{
    // GAME OVER
    showString(" \x0a\x06\x0d\x08 \x0f\x14\x08\x11");
    permanent = true;
}

void Shooter::blit(Image &fb)
{
    if (paused || continueScreen || _isGameOver)
    {
        pauseBuffer.blit(fb);
        menuSprites.blit(fb, 0, 124, 124 + 16 * pauseCursor);
        return;
    }
    fb.clear();
    if (stage)
    {
        for (auto &bl : stage->backgroundLayers)
            bl->blit(gameArea, stg->scroll);
        hud.blit(fb);
        for (auto &sprite : spriteLayer0)
            if (!sprite->hasFlag(SPRITE_NODRAW))
                sprite->blit(gameArea, 0, -stg->scroll.y);
        for (auto &fl : stage->terrainLayers)
            fl->blit(gameArea, stg->scroll);
        for (auto &sprite : spriteLayer1)
            if (!sprite->hasFlag(SPRITE_NODRAW))
                sprite->blit(gameArea, 0, -stg->scroll.y);
        for (auto &sprite : spriteLayer2)
            if (!sprite->hasFlag(SPRITE_NODRAW))
                sprite->blit(gameArea, 0, -stg->scroll.y);
        for (auto &sprite : spriteLayer3)
            if (!sprite->hasFlag(SPRITE_NODRAW))
                sprite->blit(gameArea, 0, -stg->scroll.y);
        for (auto &sprite : spriteLayer4)
            if (!sprite->hasFlag(SPRITE_NODRAW))
                sprite->blit(gameArea, 0, -stg->scroll.y);
        for (auto &bl : stage->foregroundLayers)
            bl->blit(gameArea, stg->scroll);
        fade.blit(gameArea);
        gameArea.blitFast(fb, 0, S_HUDHEIGHT, 0, 0, S_WIDTH, S_GHEIGHT);
    } else
        hud.blit(fb);
    text.blit(fb);
    popup->blit(fb);
}

static inline bool tickSprite(std::unique_ptr<Sprite> &psprite)
{
    Sprite &sprite = *psprite;
    sprite.tick();
    if (sprite.hasFlag(SPRITE_COLLIDE_SPRITES))
        sprite.computeCollisionGrid();
    if (!sprite.hasFlag(SPRITE_NOSCROLL))
        sprite.move(-stg->xspeed, 0);
    if (!sprite.hasFlag(SPRITE_SURVIVE_OFF_SCREEN)
            && sprite.offScreenDistance() > 16)
        sprite.kill();
    return sprite.isDead();
}

void Shooter::spawnPlayer(bool respawn)
{
    spriteLayer2.push_back(
        assets.playerShip->makeUniqueSprite<PlayerSprite>(
            nextSpriteID(), 0, 48, 96,
            SPRITE_COLLIDE_SPRITES | SPRITE_COLLIDE_FG | SPRITE_NOSCROLL,
            assets.playerShip, stg));
    _player = static_cast<PlayerSprite *>(spriteLayer2.back().get());
    if (respawn)
        _player->respawned();
}

inline void Shooter::updateSprites(const int layer,
        std::vector<std::unique_ptr<Sprite>> &sprites)
{
    sprites.erase(
            std::remove_if(sprites.begin(), sprites.end(), tickSprite),
            sprites.end());
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
    "  BEAM    ",
    "  CURVE   ",
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
                        rightAlignPad(std::to_string(lives), 2));
        break;
    }
}

void Shooter::loadStage(int stageNum)
{
    if (stageNum == STAGE_COUNT)
        return;
    stage = std::make_unique<Stage>(
                LoadStage("stage" + std::to_string(stageNum), *this));
    int py = stage->spawnLevelY;
    _player->updateY(py, stage->levelHeight);
    scroll.y = clamp(py - (S_GHEIGHT - 16) / 2, 0, maximumScrollY);
    popup->showStage(stageNum);
    stageFade = 1;
    maximumScrollY = stage->levelHeight - S_GHEIGHT;
    switch (stageNum)
    {
    case 1:
        PlaySong(MusicTrack::Stage1); break;
    case 2:
        PlaySong(MusicTrack::Stage2); break;
    case 3:
        PlaySong(MusicTrack::Stage3); break;
    case 4:
        PlaySong(MusicTrack::Stage4); break;
    case 5:
        PlaySong(MusicTrack::Stage5); break;
    case 6:
        PlaySong(MusicTrack::FinalStage); break;
    }
}

void Shooter::endStage()
{
    if (stageNum == STAGE_COUNT)
    {
        // game complete
        endGame();
    }
    stageFade = -1;
}

void Shooter::unloadStage()
{
    ++stageNum;
    stage = nullptr;
    gameArea.clear();
    updateHUD(HUDElement::Stage);
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

void Shooter::tryContinue()
{
    fb_back.blit(pauseBuffer);
    pauseBuffer.subtract(Color(8, 8, 8));
    if (!continues)
    {
        gameOver();
        return;
    }
    hud.writeString(menuFont, 16, 10, "GAME OVER");
    hud.writeString(menuFont, 18, 16, "CONTINUE");
    hud.writeString(menuFont, 18, 18, "EXIT");
    hud.writeString(menuFont, 15, 22, "CREDITS");
    hud.writeStringRightAlign(menuFont, 25, 22, std::to_string(continues));
    hud.blit(pauseBuffer);
    continueScreen = true;
    PauseSound();
}

void Shooter::unpauseGame()
{
    std::string clear = std::string(TextLayer<8,8>::Columns, ' ');
    for (int y = 4; y < TextLayer<8,8>::Rows; ++y)
        hud.writeString(menuFont, 0, y, clear);
    ResumeSound();
}

void Shooter::useContinue()
{
    unpauseGame();
    lives = 3;
    updateHUD(HUDElement::Lives);
    continueScreen = false;
    --continues;
    _respawnTicks = 1;
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

inline bool Shooter::pauseTick()
{
    if (!continueScreen && !_isGameOver &&
            (gameInputEdge.pause || menuInput.exit))
    {
        if (paused = !paused)
            pauseGame();
        else
            unpauseGame();
        PlayMenuSound(SoundEffect::Pause);
    }
    if (paused || continueScreen)
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
            if (paused)
            {
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
            else if (continueScreen)
            {
                switch (pauseCursor)
                {
                case 0:
                    useContinue();
                    break;
                case 1:
                    gameOver();
                    break;
                }
            }
        }
    }
    else if (_isGameOver)
    {
        if (!IsSongPlaying())
            StartFadeOut([this]() { endGame(); });
    }
    return paused || continueScreen || _isGameOver;
}

inline void Shooter::controlTick()
{
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
            if (_player) _player->onWeaponChange();
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
            if (_player) _player->onWeaponChange();
        }
    }
}

inline void Shooter::updateYScroll()
{
    int py = _player->y() - scroll.y;
    int oldScrollY = scroll.y;
    if (py < 96 && scroll.y > 0)
    {
        int dist = py;
        int speed = std::min((103 - dist) / 8, 6);
        scroll.y = std::max(0, scroll.y - speed);
    }
    else if (py > S_GHEIGHT - 96 && scroll.y < maximumScrollY)
    {
        int dist = S_GHEIGHT - py;
        int speed = std::min((103 - dist) / 8, 6);
        scroll.y = std::min(maximumScrollY, scroll.y + speed);
    }
    yspeed = scroll.y - oldScrollY;
}

void Shooter::tick()
{
    if (!stage)
        loadStage(stageNum);
    if (pauseTick()) return;
    controlTick();
    if (stageFade > 0)
        if (fade.fadeIn(1))
            stageFade = 0;
    if (_respawnTicks && !--_respawnTicks)
    {
        if (!lives) tryContinue();
        --lives;
        updateHUD(HUDElement::Lives);
        stg->spawnPlayer(true);
        _player->respawned();
    }
    stage->spawnSprites(scroll);
    scroll.x += xspeed;
    updateYScroll();

    updateSprites(0, spriteLayer0);
    updateSprites(1, spriteLayer1);
    updateSprites(2, spriteLayer2);
    updateSprites(3, spriteLayer3);
    updateSprites(4, spriteLayer4);
    if (stageFade < 0)
        if (fade.fadeOut(1))
            unloadStage();
    
    if (gameInputEdge.drone) // DEBUG
    {
        weaponLevels[activeWeapon] = (weaponLevels[activeWeapon] + 1) % 3;
        updateHUD(HUDElement::WeaponLevels);
    }
}

void Shooter::gameOver()
{
    continueScreen = false;
    _isGameOver = true;
    popup->showGameOver();
    PlaySong(MusicTrack::GameOver);
}

PlayerSprite *Shooter::getPlayer()
{
    return _player;
}

void Shooter::collectOneUp()
{
    PlaySound(SoundEffect::OneUp);
    ++lives;
    updateHUD(HUDElement::Lives);
}

void Shooter::collectWeapon(int index)
{
    if (weaponLevels[index] < 3)
    {
        ++weaponLevels[index];
        PlaySound(weaponGetSounds[index]);
        updateHUD(HUDElement::WeaponIcons);
        updateHUD(HUDElement::WeaponLevels);
    }
}

void Shooter::explode(int centerX, int centerY, ExplosionSize size, bool quiet)
{
    stg->spriteLayer4.push_back(std::make_unique<ExplosionSprite>(
        stg->nextSpriteID(), centerX, centerY, size, true));
    if (!quiet)
    {
        switch (size)
        {
        case ExplosionSize::Small1:
            PlaySound(SoundEffect::ExplosionSmall1); break;
        case ExplosionSize::Small2:
            PlaySound(SoundEffect::ExplosionSmall2); break;
        case ExplosionSize::Medium1:
            PlaySound(SoundEffect::ExplosionMedium1); break;
        case ExplosionSize::Medium2:
            PlaySound(SoundEffect::ExplosionMedium2); break;
        case ExplosionSize::Large:
            PlaySound(SoundEffect::ExplosionLarge); break;
        }
    }
}

void Shooter::killPlayer()
{
    explode(_player->x() + 16, _player->y() + 8,
            ExplosionSize::Large, false);
    _respawnTicks = 50;
    if (activeWeapon > 0)
    {
        activeWeapon = 0;
        weaponLevels[activeWeapon] = -1;
    } else
        weaponLevels[activeWeapon] = 0;
    _player->kill();
    _player = nullptr;
}

void RenderGame(Image &fb)
{
    stg->blit(fb);
}

void DoGameTick()
{
    stg->tick();
}
