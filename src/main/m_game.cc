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
#include "object.hh"
#include "bullet.hh"
#include "powerup.hh"

std::shared_ptr<Shooter> stg;
static constexpr int STAGE_COUNT = 6;

static inline std::shared_ptr<Spritesheet> loadSprites(const std::string &s)
{
    return std::make_shared<Spritesheet>(LoadSpritesheet(s));
}

static void LoadAssets()
{
    stg->menuSprites = LoadSpritesheet("menuspr");
    stg->assets.playerShip = loadSprites("ship");
    stg->assets.powerupSprites = loadSprites("powerups");
    stg->assets.bulletSprites = loadSprites("bullets");
    stg->assets.sigma = loadSprites("sigma");
    stg->assets.enemy01 = loadSprites("enemy01");
    stg->assets.enemy02 = loadSprites("enemy02");
    stg->assets.enemy03 = loadSprites("enemy03");
    stg->assets.enemy04 = loadSprites("enemy04");
    stg->assets.enemy05 = loadSprites("enemy05");
    stg->assets.enemy06 = loadSprites("enemy06");
    stg->assets.boss1a = loadSprites("boss1a");
}

void StartNewGame()
{
    stg = std::make_shared<Shooter>();
    LoadAssets();
    stg->fade.fadeOut(S_MAXCLR);
    stg->spawnPlayer(false);
    stg->continues = startContinues;
    stg->difficulty = difficulty;
    stg->pmode = pmode;
    stg->popup = std::make_unique<ScreenPopup>();
    stg->spriteLayer0.reserve(256);
    stg->spriteLayer1.reserve(256);
    stg->spriteLayer2.reserve(256);
    stg->spriteLayer3.reserve(256);
    stg->spriteLayer4.reserve(256);
    switch (difficulty)
    {
    case DifficultyLevel::EASY:
        ++stg->sigmas;
    case DifficultyLevel::NORMAL:
        ++stg->sigmas;
    }
    stg->drawHUD();
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
    {
        visibility = 0;
        ticks += permanent;
    }
}

void ScreenPopup::clear()
{
    ticks = visibility = 0;
    back.clear();
    front.clear();
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

inline void Shooter::blitPlayer(Image &fb, int oy)
{
    int px = static_cast<int>(player->x()), py = static_cast<int>(player->y());
    if (!player->hasFlag(SPRITE_NODRAW))
        player->blit(gameArea, 0, oy);
    if (player->hasShield() && !(totalFrames & 1))
        assets.playerShip->blit(gameArea, 5, px, py + oy - 4);
}

void Shooter::blit(Image &fb)
{
    ++totalFrames;
    if (_isGameOver)
    {
        pauseBuffer.blit(fb);
        popup->blit(fb);
        return;
    } 
    else if (paused || continueScreen)
    {
        pauseBuffer.blit(fb);
        menu.blit(fb);
        menuSprites.blit(fb, 0, 124, 124 + 16 * pauseCursor);
        return;
    }
    fb.clear();
    if (stage)
    {
        int oy = static_cast<int>(-stg->scroll.y);
        for (auto &bl : stage->backgroundLayers)
            bl->blit(gameArea, stg->scroll);
        hud.blit(fb);
        for (auto &sprite : spriteLayer0)
            if (!sprite->hasFlag(SPRITE_NODRAW))
                sprite->blit(gameArea, 0, oy);
        for (auto &fl : stage->terrainLayers)
            fl->blit(gameArea, stg->scroll);
        for (auto &sprite : spriteLayer1)
            if (!sprite->hasFlag(SPRITE_NODRAW))
                sprite->blit(gameArea, 0, oy);
        for (auto &sprite : spriteLayer2)
            if (!sprite->hasFlag(SPRITE_NODRAW))
                sprite->blit(gameArea, 0, oy);
        if (player)
            blitPlayer(gameArea, oy);
        for (auto &sprite : spriteLayer3)
            if (!sprite->hasFlag(SPRITE_NODRAW))
                sprite->blit(gameArea, 0, oy);
        for (auto &sprite : spriteLayer4)
            if (!sprite->hasFlag(SPRITE_NODRAW))
                sprite->blit(gameArea, 0, oy);
        for (auto &bl : stage->foregroundLayers)
            bl->blit(gameArea, stg->scroll);
        fade.blit(gameArea);
        gameArea.blitFast(fb, 0, S_HUDHEIGHT, 0, 0, S_WIDTH, S_GHEIGHT);
    } else
        hud.blit(fb);
    text.blit(fb);
    popup->blit(fb);
}

static inline bool tickSprite(Sprite &sprite)
{
    sprite.tick();
    if (sprite.hasFlag(SPRITE_COLLIDE_SPRITES))
        sprite.computeCollisionGrid();
    if (!sprite.hasFlag(SPRITE_NOSCROLL))
        sprite.move(-stg->xSpeed, 0_x);
    if (!sprite.hasFlag(SPRITE_SURVIVE_OFF_SCREEN)
            && sprite.offScreenDistance() > 4)
        sprite.kill();
    return sprite.isDead();
}

static inline bool tickSpritePtr(std::unique_ptr<Sprite> &sprite)
{
    return tickSprite(*sprite);
}

void Shooter::spawnPlayer(bool respawn)
{
    int sly = stage ? stage->spawnLevelY : 100;
    lastPlayerY = sly;
    player = assets.playerShip->makeUniqueSprite<PlayerSprite>(
            nextSpriteID(), 0, 48_x, Fix(sly),
            SPRITE_COLLIDE_SPRITES | SPRITE_COLLIDE_FG | SPRITE_NOSCROLL,
            assets.playerShip, stg);
    if (respawn)
        player->respawned();
}

inline void Shooter::updateSprites(const int layer,
        std::vector<std::unique_ptr<Sprite>> &sprites)
{
    sprites.erase(
            std::remove_if(sprites.begin(), sprites.end(), tickSpritePtr),
            sprites.end());
}

void Shooter::addScore(int points)
{
    if (difficulty == DifficultyLevel::EASY && highScore > 0 &&
            score < highScore && score + points >= highScore)
        collectOneUp();
    if ((score += points) > highScore)
    {
        highScore = score;
        updateHUD(HUDElement::HighScore);
    }
    updateHUD(HUDElement::Score);
}

static const std::array<int, 5> bonusScores = {
    500, 1000, 2000, 3000, 5000
};

void Shooter::nextBonus(Fix x, Fix y)
{
    bool maxScore = _nextBonus == (bonusScores.size() - 1);
    if (maxScore)
        PlaySound(SoundEffect::ScorePowerup);
    else
        PlaySound(SoundEffect::ScoreNotMax);
    spawnScore(x + 8, y, bonusScores[_nextBonus++]);
    if (_nextBonus == bonusScores.size())
        --_nextBonus;
};

static const std::array<std::string, 4> difficultyLevels = {
    "EASY",
    "NORMAL",
    "HARD",
    "BIZARRE"
};

void Shooter::drawHUD()
{
    std::string clear = std::string(TextLayer<8,8>::Columns, ' ');
    for (int y = 0; y < 4; ++y)
        hud.writeString(menuFont, 0, y, clear);
    hud.writeString(menuFont, 1, 0, "STAGE");
    hud.writeString(menuFont, 1, 2, "HI");
    hud.writeString(menuFont, 1, 3, "SC");
    hud.writeStringRightAlign(menuFont, 38, 0,
                    difficultyLevels[static_cast<int>(difficulty)]);
    hud.writeString(menuFont, 28, 2, "SPEED");
    hud.writeString(hudFont, 18, 2, "\x44\x42");
    hud.writeString(hudFont, 32, 3, "\x0e\x0f\x42");
    updateHUD(HUDElement::Stage);
    updateHUD(HUDElement::Score);
    updateHUD(HUDElement::HighScore);
    updateHUD(HUDElement::WeaponName);
    updateHUD(HUDElement::SigmaCount);
    updateHUD(HUDElement::Speed);
    updateHUD(HUDElement::Lives);
}

static const std::array<std::string, 4> weaponNames = {
    "\x48\x49\x4a\x4b\x4c\x4d",
    "\x58\x59\x5a\x5b\x5c\x5d",
    "\x68\x69\x6a\x6b\x6c\x6d",
    "\x78\x79\x7a\x7b\x7c\x7d"
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
    case HUDElement::WeaponName:
        hud.writeString(hudFont, 17, 1, weaponNames[activeWeapon]);
        break;
    case HUDElement::SigmaCount:
        hud.writeString(menuFont, 20, 2,
                        rightAlignPad(std::to_string(sigmas), 2));
        break;
    case HUDElement::Speed:
        for (int i = 1; i <= 4; ++i)
            hud.writeChar(hudFont, 33 + i, 2, playerSpeed >= i ? '\x41' : ' ');
        break;
    case HUDElement::Lives:
        hud.writeString(menuFont, 36, 3,
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
    player->updateY(Fix(py), Fix(stage->levelHeight));
    scroll.x = 0_x;
    scroll.y = clamp(Fix(py - (S_GHEIGHT - 16) / 2), 0_x, maximumScrollY);
    popup->showStage(stageNum);
    xSpeed = Fix(1);
    stageFade = 1;
    ResetBullets(stageNum);
    colGridHeight = stage->levelHeight;
    maximumScrollY = Fix(stage->levelHeight - S_GHEIGHT);
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
    stage = nullptr;
    gameArea.clear();
    spriteLayer0.clear();
    spriteLayer1.clear();
    spriteLayer2.erase(
            std::remove_if(spriteLayer2.begin(), spriteLayer2.end(),
                [](const std::unique_ptr<Sprite> &s) { 
                    return s->type() != SpriteType::Player
                        && s->type() != SpriteType::Drone;
                }),
            spriteLayer2.end());
    spriteLayer3.clear();
    spriteLayer4.clear();
    ++stageNum;
    updateHUD(HUDElement::Stage);
}

void Shooter::pauseGame()
{
    fb_back.blit(pauseBuffer);
    pauseBuffer.subtract(Color(8, 8, 8));
    menu.writeString(menuFont, 18, 10, "PAUSE");
    menu.writeString(menuFont, 18, 16, "CONTINUE");
    menu.writeString(menuFont, 18, 18, "EXIT");
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
    menu.writeString(menuFont, 16, 10, "GAME OVER");
    menu.writeString(menuFont, 18, 16, "CONTINUE");
    menu.writeString(menuFont, 18, 18, "EXIT");
    menu.writeString(menuFont, 15, 22, "CREDITS");
    menu.writeStringRightAlign(menuFont, 25, 22, std::to_string(continues));
    continueScreen = true;
    PauseSound();
}

void Shooter::unpauseGame()
{
    menu.clear();
    ResumeSound();
}

void Shooter::useContinue()
{
    unpauseGame();
    StopSounds();
    StopSong();
    popup->clear();
    lives = DEFAULT_LIVES + 1;
    continueScreen = false;
    --continues;
    _respawnTicks = 1;
    ResumeSound();
    --stageNum;
    xSpeed = Fix(0);
    usedContinue = true;
    endStage();
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
                    menu.clear();
                    gameOver();
                    break;
                }
            }
        }
    }
    else if (_isGameOver)
    {
        if (!IsSongPlaying() || menuInput.exit)
            StartFadeOut([this]() { endGame(); });
    }
    return paused || continueScreen || _isGameOver;
}

inline void Shooter::controlTick()
{
    if (gameInputEdge.speedUp && playerSpeed < 4)
    {
        ++playerSpeed;
        updateHUD(HUDElement::Speed);
        PlayMenuSound(SoundEffect::SpeedChange);
    }
    if (gameInputEdge.speedDown && playerSpeed > 1)
    {
        --playerSpeed;
        updateHUD(HUDElement::Speed);
        PlayMenuSound(SoundEffect::SpeedChange);
    }
}

inline void Shooter::updateYScroll()
{
    if (player)
        lastPlayerY = static_cast<int>(player->y());
    int oldScrollY = static_cast<int>(scroll.y);
    int py = lastPlayerY - oldScrollY;
    if (py < 96 && scroll.y > 0)
    {
        int dist = py;
        int speed = std::min((103 - dist) / 8, 6);
        scroll.y = std::max(0_x, scroll.y - speed);
    }
    else if (py > S_GHEIGHT - 96 && scroll.y < maximumScrollY)
    {
        int dist = S_GHEIGHT - py;
        int speed = std::min((103 - dist) / 8, 6);
        scroll.y = std::min(maximumScrollY, scroll.y + speed);
    }
}

void Shooter::respawnPlayer()
{
    switch (difficulty)
    {
    case DifficultyLevel::EASY:
        if (--weaponLevels[activeWeapon] < 0)
        {
            if (activeWeapon == 0)
                weaponLevels[activeWeapon] = 0;
            else
                activeWeapon = 0;
        }
        break;
    case DifficultyLevel::NORMAL:
        if (activeWeapon > 0)
        {
            activeWeapon = 0;
            weaponLevels[activeWeapon] = -1;
        } else
            weaponLevels[activeWeapon] = 0;
        break;
    case DifficultyLevel::HARD:
    case DifficultyLevel::BIZARRE:
        weaponLevels.fill(-1);
        weaponLevels[0] = 0;
        activeWeapon = 0;
        break;
    }
    _nextBonus = 0;
    updateHUD(HUDElement::WeaponName);
    stg->spawnPlayer(true);
    player->respawned();
}

void Shooter::tick()
{
    if (!stage)
        loadStage(stageNum);
    if (!usedContinue)
    {
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
            respawnPlayer();
        }
        stage->spawnSprites(scroll);
        scroll.x += xSpeed;
        updateYScroll();

        updateSprites(0, spriteLayer0);
        updateSprites(1, spriteLayer1);
        updateSprites(2, spriteLayer2);
        updateSprites(3, spriteLayer3);
        updateSprites(4, spriteLayer4);
        if (player)
            tickSprite(*player);
    }
    if (stageFade < 0)
        if (fade.fadeOut(1))
        {
            unloadStage();
            if (usedContinue)
            {
                drawHUD();
                usedContinue = false;
            }
        }
}

void Shooter::gameOver()
{
    continueScreen = false;
    _isGameOver = true;
    popup->showGameOver();
    PlaySong(MusicTrack::GameOver);
}

void Shooter::spawnScore(Fix x, Fix y, int score)
{
    addScore(score);
    spriteLayer3.push_back(std::make_unique<ScoreSprite>(
        nextSpriteID(), x, y, score));
}

bool Shooter::collectOneUp()
{
    if (lives >= 10) return false;
    PlaySound(SoundEffect::OneUp);
    ++lives;
    updateHUD(HUDElement::Lives);
    return true;
}

bool Shooter::collectWeapon(int index)
{
    if (weaponLevels[index] >= 2) return false;
    ++weaponLevels[index];
    if (activeWeapon == index)
        PlaySound(SoundEffect::WeaponUpLevel);
    else
    {
        PlaySound(SoundEffect::WeaponChange);
        activeWeapon = index;
        updateHUD(HUDElement::WeaponName);
    }
    return true;
}

bool Shooter::collectSigma()
{
    if (sigmas >= 5) return false;
    PlaySound(SoundEffect::WeaponUpLevel);
    ++sigmas;
    updateHUD(HUDElement::SigmaCount);
    return true;
}

bool Shooter::useSigma()
{
    if (!sigmas) return false;
    --sigmas;
    updateHUD(HUDElement::SigmaCount);
    return true;
}

bool Shooter::collectDrone()
{
    if (true) return false;
    PlaySound(SoundEffect::WeaponUpLevel);
    return true;
}

Fix2D Shooter::vecToPlayer(Fix x, Fix y)
{
    if (!isPlayerAlive()) return Fix2D(0_x, 0_x);
    return Fix2D(player->x() - x, player->y() - y);
}

void Shooter::spawnPowerup(Fix x, Fix y, PowerupType type)
{
    spriteLayer2.push_back(std::make_unique<PowerupSprite>(
        *this, nextSpriteID(), x - 8, y - 8, type));
}

void Shooter::explode(Fix centerX, Fix centerY, ExplosionSize size, bool quiet)
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
    explode(player->x() + 16, player->y() + 8, ExplosionSize::Large, false);
    _respawnTicks = 50;
    player->kill();
    player = nullptr;
}

void RenderGame(Image &fb)
{
    stg->blit(fb);
}

void DoGameTick()
{
    stg->tick();
}
