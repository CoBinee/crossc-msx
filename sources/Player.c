// Player.c : プレイヤ
//


// 参照ファイル
//
#include <string.h>
#include "C.h"
#include "App.h"
#include "Game.h"
#include "Map.h"
#include "Player.h"

// 外部参照関数
//

// 外部参照変数
//


// 内部関数の宣言
//
static void PlayerTransit(void (*process)(void));
static void PlayerPlay(void);
static void PlayerTurn(void);

// 変数の定義
//

// プレイヤ
static struct Player player;


// プレイヤを初期化する
//
void PlayerInitialize(void)
{
    // プレイヤの初期化
    memset(&player, 0x00, sizeof (struct Player));

    // 位置の設定
    MapGetRandomPositions(player.positions);

    // 向きの設定
    player.direction = MapGetRandomDirection(player.positions);

    // 速度の設定
    player.speed = PLAYER_SPEED_MINIMUM;

    // プレイヤの繊維
    PlayerTransit(PlayerPlay);
}

// プレイヤを更新する
//
void PlayerUpdate(void)
{
    player.process();
}

// プレイヤを描画する
//
void PlayerRender(void)
{
    static const unsigned char numbers[] = {
        0x20, 0x24, 0x28, 0x2c, 0x28, 0x24, 
    };
    unsigned char x = player.positions[_X_] + (MAP_ROUTE_VIEW_X * MAP_ROUTE_VIEW_PIXEL);
    unsigned char y = player.positions[_Y_] + (MAP_ROUTE_VIEW_Y * MAP_ROUTE_VIEW_PIXEL);
    unsigned char n = numbers[player.animation / PLAYER_ANIMATION_INTERVAL];
    {
        unsigned char *s = &sprite[GAME_SPRITE_PLAYER_0];
        *s++ = y - 0x08 - 0x01;
        *s++ = x - 0x08;
        *s++ = n;
        *s   = VDP_COLOR_LIGHT_YELLOW;
    }
    {
        unsigned char *s = &sprite[GAME_SPRITE_PLAYER_1];
        *s++ = y - 0x08 - 0x01;
        *s++ = x - 0x08;
        *s++ = n + 0x10;
        *s   = VDP_COLOR_BLACK;
    }
}

// プレイヤを遷移する
//
static void PlayerTransit(void (*process)(void))
{
    player.process = process;
    player.state = PLAYER_STATE_NULL;
}

// プレイヤを操作する
//
static void PlayerPlay(void)
{
    // 初期化
    if (player.state == 0x00) {

        // 初期化の完了
        ++player.state;
    }

    // 操作の確認
    if (GameIsPlay()) {

        // 速度の取得
        player.speed = GameGetCombo();
        if (player.speed < PLAYER_SPEED_MINIMUM) {
            player.speed = PLAYER_SPEED_MINIMUM;
        } else if (player.speed > PLAYER_SPEED_MAXIMUM) {
            player.speed = PLAYER_SPEED_MAXIMUM;
        }

        // 移動
        PlayerTurn();
        {
            unsigned char d = MapMoveToBranch(player.positions, player.direction, player.speed);
            if (d > 0) {
                PlayerTurn();
                MapMoveToBranch(player.positions, player.direction, d);
            }
        }

        // アニメーションの更新
        if (++player.animation >= PLAYER_ANIMATION_CYCLE * PLAYER_ANIMATION_INTERVAL) {
            player.animation = 0;
        }
    }
}

// プレイヤが分岐を曲がる
//
static void PlayerTurn(void)
{
    if (CIsInputPush(INPUT_KEY_UP) && MapIsTurn(player.positions, MAP_ROUTE_UP_BIT)) {
        player.direction = PLAYER_DIRECTION_UP;
    } else if (CIsInputPush(INPUT_KEY_DOWN) && MapIsTurn(player.positions, MAP_ROUTE_DOWN_BIT)) {
        player.direction = PLAYER_DIRECTION_DOWN;
    } else if (CIsInputPush(INPUT_KEY_LEFT) && MapIsTurn(player.positions, MAP_ROUTE_LEFT_BIT)) {
        player.direction = PLAYER_DIRECTION_LEFT;
    } else if (CIsInputPush(INPUT_KEY_RIGHT) && MapIsTurn(player.positions, MAP_ROUTE_RIGHT_BIT)) {
        player.direction = PLAYER_DIRECTION_RIGHT;
    } else if (MapIsBranch(player.positions)) {
        player.direction = MapTurn(player.positions, player.direction);
    }
}

// プレイヤの位置を取得する
//
unsigned char *PlayerGetPositions(void)
{
    return player.positions;
}

// プレイヤの速度を取得する
//
unsigned char PlayerGetSpeed(void)
{
    return player.speed;
}
