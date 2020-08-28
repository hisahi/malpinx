/****************************************************************************/
/*                                                                          */
/*   MALPINX SOURCE CODE (C) 2020      SAMPO HIPPELAINEN (HISAHI).          */
/*   SEE THE LICENSE FILE IN THE SOURCE ROOT DIRECTORY FOR LICENSE INFO.    */
/*                                                                          */
/****************************************************************************/
// player.cc: code for player ship

#include <iostream>
#include <memory>
#include "image.hh"
#include "m_game.hh"
#include "player.hh"
#include "input.hh"
#include "sfx.hh"
#include "bullet.hh"
#include "fix.hh"

static int droneNextTarget{0};

PlayerSprite::PlayerSprite(int id, std::shared_ptr<Image> img, Fix x, Fix y,
    int flags, std::shared_ptr<Spritesheet> playerSprites,
    std::shared_ptr<Shooter> stg)
    : Sprite(id, img, x, y, flags, SpriteType::Player), fireDelay(0),
        sheet(playerSprites), game(stg), deltaX(0), deltaY(0), invulTicks(0),
        activeSprite(-1),
        rightEdge(Fix(S_WIDTH - img->width())),
        bottomEdge(Fix(S_GHEIGHT - img->height()))
{
    updateImage(sheet->getImage(activeSprite = 0));
    updateHitbox(2, 2, _width - 4, _height - 4);
}

inline void PlayerSprite::moveTick()
{
    if ((-4 < deltaX && deltaX < 0) || (0 < deltaX && deltaX < 4))
        deltaX *= 2;
    if ((-4 < deltaY && deltaY < 0) || (0 < deltaY && deltaY < 4))
        deltaY *= 2;

    if (gameInputEdge.left)
        deltaX = -1_x;
    if (gameInputEdge.right)
        deltaX = 1_x;
    if (deltaX != 0 && !gameInput.left && !gameInput.right)
        deltaX = 0_x;

    if (gameInputEdge.up)
    {
        deltaY = -1_x;
        setActiveSprite(1);
    }
    if (gameInputEdge.down)
    {
        deltaY = 1_x;
        setActiveSprite(3);
    }
    if (deltaY != 0 && !gameInput.up && !gameInput.down)
    {
        setActiveSprite(deltaY > 0 ? 3 : 1);
        resetTicks = 4;
        deltaY = 0_x;
    }

    if (deltaY == -4)
        setActiveSprite(2);
    else if (deltaY == 4)
        setActiveSprite(4);
    
    _x += deltaX * (1 + game->playerSpeed) / 4;
    _y += deltaY * (1 + game->playerSpeed) / 4;
    if (_x < 0) {
        _x = 0_x;
        deltaX = 0_x;
    } else if (_x > rightEdge) {
        _x = rightEdge;
        deltaX = 0_x;
    }
    if (_y < topEdge) {
        _y = topEdge;
        deltaY = 0_x;
        setActiveSprite(1);
        resetTicks = 4;
    } else if (_y > bottomEdge) {
        _y = bottomEdge;
        deltaY = 0_x;
        setActiveSprite(3);
        resetTicks = 4;
    }
    
    if (activeSprite != 0 && deltaY == 0 && --resetTicks < 0)
        setActiveSprite(0);
}

void PlayerSprite::updateY(Fix y, Fix top, Fix bottom)
{
    _y = y;
    topEdge = top;
    bottomEdge = bottom - _img->height();
}

void PlayerSprite::onWeaponChange()
{
}

inline void PlayerSprite::fireTick()
{
    if (gameInput.fire && fireDelay == 0)
    {
        fireDelay = FireWeapon(*game, game->activeWeapon,
                game->weaponLevels[game->activeWeapon], _x + 31, _y + 10);
        for (auto &drone : game->drones)
            FireDroneWeapon(*game, game->activeWeapon,
                        drone->x() + 16, drone->y() + 8);
    }
    else if (fireDelay > 0)
        --fireDelay;
    if (gameInputEdge.sigma && sigmaDelay == 0 && game->useSigma())
    {
        sigmaDelay = FireSigma(*game, _x + 31, _y + 10);
    }
    else if (sigmaDelay > 0)
        --sigmaDelay;
}

inline void PlayerSprite::collisionTick()
{
    if (invulTicks)
    {
        _flags ^= SPRITE_NODRAW;
        if (!--invulTicks)
            _flags &= ~SPRITE_NODRAW;
        return;
    }

    for (auto &layer : stg->stage->terrainLayers)
    {
        if (layer->hitsSprite(*_img, stg->scroll, _hitbox, _x, _y))
        {
            stg->killPlayer();
            return;
        }
    }
}

bool PlayerSprite::hasShield()
{
    return shield;
}

void PlayerSprite::giveShield()
{
    PlaySound(SoundEffect::GotShield);
    shield = true;
}

void PlayerSprite::tick()
{
    moveTick();
    fireTick();
    collisionTick();
}

void PlayerSprite::respawned()
{
    invulTicks = S_TICKS * 3;
}

bool PlayerSprite::damage(int dmg)
{
    if (invulTicks) return false;
    PlaySound(SoundEffect::PlayerHit);
    if (dmg > 0)
    {
        if (shield)
        {
            PlaySound(SoundEffect::LostShield);
            shield = false;
            invulTicks = S_TICKS / 2;
            return false;
        }
        game->killPlayer();
        return true;
    }
    return false;
}

inline void PlayerSprite::setActiveSprite(int index)
{
    if (activeSprite != index)
    {
        updateImage(sheet->getImage(index), true, false);
        activeSprite = index;
    }
}

DroneSprite::DroneSprite(Shooter &stg, int id,
        PlayerSprite &player, int droneNum)
    : Sprite(id, nullptr, Fix(S_WIDTH), Fix(S_HEIGHT), SPRITE_NOSCROLL
            | SPRITE_COLLIDE_SPRITES | SPRITE_SURVIVE_OFF_SCREEN
            | SPRITE_ONLYBOXCHECK,
            SpriteType::Drone),
        game(stg), player(player), droneNum(droneNum), spawnTicks(60),
        centerX(player.x()), centerY(player.y())
{
    droneHitTargets.reserve(8);
    _hitbox = Hitbox(1, 2, 13, 12);
}

void DroneSprite::blit(Image &fb, int xoff, int yoff) const
{
    int ox = (_x + xoff).round(), oy = (_y + yoff).round();
    game.assets.drone0->blit(fb,
        (6 * game.activeWeapon) + ((_ticks / 3) % 6), ox + 2, oy + 2);
    game.assets.drone1->blit(fb, (_ticks / 6) % 8, ox, oy);
}

void DroneSprite::explode()
{
    game.explodeNoScroll(_x + 8, _y + 8, ExplosionSize::Medium2, true);
    kill();
}

void DroneSprite::tick()
{
    Fix distance = 30_x - (spawnTicks / 2_x);
    int sineIndex = ((game.totalTicks << 1) + droneNum * 64) % 128;
    int cosineIndex = (sineIndex + 32) % 128;
    _x = centerX - distance * sineTable[sineIndex] + 8;
    _y = centerY + distance * sineTable[cosineIndex];
    Fix2D discrepancy = Fix2D(player.x() - centerX, player.y() - centerY);
    if (discrepancy)
    {
        FixPolar2D dPolar = FixPolar2D(discrepancy);
        dPolar.length = std::min(2_x, dPolar.length);
        discrepancy = Fix2D(dPolar);
        centerX += discrepancy.x;
        centerY += discrepancy.y;
    }

    if (!damageTicks)
    {
        int index = 0;
        droneHitTargets.clear();
        for (auto &s : game.spriteLayer3)
            if (s && s->type() == SpriteType::BulletEnemy && hits(*s))
                s->kill();
        for (auto &s : game.spriteLayer2)
            if (s && s->type() == SpriteType::Enemy && hits(*s))
                droneHitTargets.push_back(s.get());
        if (droneHitTargets.size() > 1)
            index = droneNextTarget = (droneNextTarget + 1)
                                        % droneHitTargets.size();
        if (index < droneHitTargets.size())
        {
            droneHitTargets[index]->damage(5);
            damageTicks = 10;
        }
    }
    else
        --damageTicks;
    if (spawnTicks)
        --spawnTicks;
    ++_ticks;
}
