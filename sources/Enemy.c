// Enemy.c : エネミー
//


// 参照ファイル
//
#include <stdlib.h>
#include <string.h>
#include "C.h"
#include "App.h"
#include "Game.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"

// 外部参照関数
//

// 外部参照変数
//


// 内部関数の宣言
//
static void EnemyTransit(struct Enemy *enemy, void (*process)(struct Enemy *));
static void EnemyIdle(struct Enemy *enemy);
static void EnemyBorn(struct Enemy *enemy);
static void EnemyPlay(struct Enemy *enemy);
static void EnemyDead(struct Enemy *enemy);
static void EnemyTurn(struct Enemy *enemy);

// 変数の定義
//

// エネミー
static struct Enemy enemies[ENEMY_ENTRY];

// スプライト
static unsigned char enemySpriteRotate;


// エネミーを初期化する
//
void EnemyInitialize(void)
{
    // エネミーの初期化
    memset(&enemies, 0x00, sizeof (struct Enemy) * ENEMY_ENTRY);
    for (char i = 0; i < ENEMY_ENTRY; i++) {
        static const unsigned char colors[] = {
            VDP_COLOR_LIGHT_RED, 
            VDP_COLOR_LIGHT_GREEN, 
            VDP_COLOR_LIGHT_BLUE, 
            VDP_COLOR_LIGHT_RED, 
            VDP_COLOR_LIGHT_GREEN, 
            VDP_COLOR_LIGHT_BLUE, 
        };
        enemies[i].color = colors[i];
        enemies[i].animation = i * 7;
        EnemyTransit(&enemies[i], EnemyIdle);
    }

    // スプライトの初期化
    enemySpriteRotate = 0x00;
}

// エネミーを更新する
//
void EnemyUpdate(void)
{
    for (char i = 0; i < ENEMY_ENTRY; i++) {
        enemies[i].process(&enemies[i]);
    }
}

// エネミーを描画する
//
void EnemyRender(void)
{
    // エネミーの描画
    {
        unsigned char r = enemySpriteRotate;
        for (char i = 0; i < ENEMY_ENTRY; i++) {
            struct Enemy *enemy = &enemies[i];
            if ((enemy->flag & ENEMY_FLAG_RENDER) != 0) {
                unsigned char *s = &sprite[GAME_SPRITE_ENEMY + r];
                *s++ = enemy->positions[_Y_] + (MAP_ROUTE_VIEW_Y * MAP_ROUTE_VIEW_PIXEL) - 0x08 - 0x01;
                *s++ = enemy->positions[_X_] + (MAP_ROUTE_VIEW_X * MAP_ROUTE_VIEW_PIXEL) - 0x08;
                *s++ = enemy->sprite;
                *s   = enemy->color;
                r = (r + 0x04) & 0x1f;
            }
        }
    }

    // スプライトの更新
    enemySpriteRotate = (enemySpriteRotate + 0x04) & 0x1f;
}

// エネミーを遷移する
//
static void EnemyTransit(struct Enemy *enemy, void (*process)(struct Enemy *))
{
    enemy->process = process;
    enemy->state = ENEMY_STATE_NULL;
}

// エネミーのヒット判定を行う
//
void EnemyHit(void)
{
    unsigned char *p = PlayerGetPositions();
    bool hit = false;
    for (char i = 0; i < ENEMY_ENTRY; i++) {
        struct Enemy *enemy = &enemies[i];
        if ((enemy->flag & ENEMY_FLAG_PLAY) != 0) {
            if (abs(p[_X_] - enemy->positions[_X_]) + abs(p[_Y_] - enemy->positions[_Y_]) <= ENEMY_HIT_DISTANCE) {
                EnemyTransit(enemy, EnemyDead);
                hit = true;
            }
        }
    }
    if (hit) {
        GameSetUnplay(ENEMY_ANIMATION_DEAD_CYCLE * ENEMY_ANIMATION_DEAD_INTERVAL);
    }
}

// エネミーが待機する
//
static void EnemyIdle(struct Enemy *enemy)
{
    // 初期化
    if (enemy->state == 0x00) {

        // フラグの設定
        enemy->flag = ENEMY_FLAG_NULL;

        // 初期化の完了
        ++enemy->state;
    }

    // アニメーションの更新
    if (enemy->animation > 0) {
        --enemy->animation;
    }

    // エネミーの遷移
    if (enemy->animation == 0) {
        EnemyTransit(enemy, EnemyBorn);
    }
}

// エネミーを生成する
//
static void EnemyBorn(struct Enemy *enemy)
{
    // 初期化
    if (enemy->state == 0x00) {

        // フラグの設定
        enemy->flag = ENEMY_FLAG_NULL;

        // 位置の設定
        {
            unsigned char *p = PlayerGetPositions();
            if ((CGetRandom() & 0x10) != 0) {
                enemy->positions[_X_] = CGetRandom() % (MAP_MAZE_SIZE_X / 2);
                enemy->positions[_Y_] = CGetRandom() % MAP_MAZE_SIZE_Y;
                if (p[_X_] < MAP_ROUTE_SIZE_X * MAP_ROUTE_SIZE_PIXEL / 2) {
                    enemy->positions[_X_] = enemy->positions[_X_] + (MAP_MAZE_SIZE_X / 2);
                }
            } else {
                enemy->positions[_X_] = CGetRandom() % MAP_MAZE_SIZE_X;
                enemy->positions[_Y_] = CGetRandom() % (MAP_MAZE_SIZE_Y / 2);
                if (p[_Y_] < MAP_ROUTE_SIZE_Y * MAP_ROUTE_SIZE_PIXEL / 2) {
                    enemy->positions[_Y_] = enemy->positions[_Y_] + (MAP_MAZE_SIZE_Y / 2);
                }
            }
            enemy->positions[_X_] = enemy->positions[_X_] * 0x02 * MAP_ROUTE_SIZE_PIXEL + MAP_ROUTE_SIZE_PIXEL / 2;
            enemy->positions[_Y_] = enemy->positions[_Y_] * 0x02 * MAP_ROUTE_SIZE_PIXEL + MAP_ROUTE_SIZE_PIXEL / 2;
        }

        // 向きの設定
        enemy->direction = MapGetRandomDirection(enemy->positions);

        // 速度の設定
        enemy->speed = 0x01;

        // アニメーションの設定
        enemy->animation = ENEMY_ANIMATION_BORN;

        // スプライトの設定
        enemy->sprite = ENEMY_SPRITE_PLAY;

        // 初期化の完了
        ++enemy->state;
    }

    // アニメーションの更新
    if (--enemy->animation == 0) {
        
        // エネミーの遷移
        EnemyTransit(enemy, EnemyPlay);
    }

    // フラグの更新
    if ((enemy->animation & ENEMY_ANIMATION_BORN_INTERVAL) != 0) {
        enemy->flag |= ENEMY_FLAG_RENDER;
    } else {
        enemy->flag &= ~ENEMY_FLAG_RENDER;
    }
}

// エネミーを操作する
//
static void EnemyPlay(struct Enemy *enemy)
{
    // 初期化
    if (enemy->state == 0x00) {

        // フラグの設定
        enemy->flag |= ENEMY_FLAG_PLAY | ENEMY_FLAG_RENDER;

        // 初期化の完了
        ++enemy->state;
    }

    // 操作の確認
    if (GameIsPlay()) {

        // 移動
        if (MapIsBranch(enemy->positions)) {
            EnemyTurn(enemy);
        }
        {
            unsigned char d = MapMoveToBranch(enemy->positions, enemy->direction, enemy->speed);
            if (d > 0) {
                EnemyTurn(enemy);
                MapMoveToBranch(enemy->positions, enemy->direction, d);
            }
        }

        // アニメーションの更新
        if (++enemy->animation >= ENEMY_ANIMATION_PLAY_CYCLE * ENEMY_ANIMATION_PLAY_INTERVAL) {
            enemy->animation = 0;
        }

        // スプライトの更新
        enemy->sprite = ENEMY_SPRITE_PLAY + 0x04 * enemy->animation / ENEMY_ANIMATION_PLAY_INTERVAL;
    }
}

// エネミーが死亡する
//
static void EnemyDead(struct Enemy *enemy)
{
    // 初期化
    if (enemy->state == 0x00) {

        // フラグの設定
        enemy->flag &= ~ENEMY_FLAG_PLAY;
        enemy->flag |= ENEMY_FLAG_RENDER;

        // アニメーションの設定
        enemy->animation = ENEMY_ANIMATION_DEAD_CYCLE * ENEMY_ANIMATION_DEAD_INTERVAL;

        // SE の再生
        CPlaySe("T1@0V15,3L0O3AO4C+FGO3ABO4C+D+EFGAA+BO5CC+DD+EFF+GG+ABO6C+D+FGAO7CEG-B-");

        // 初期化の完了
        ++enemy->state;
    }

    // アニメーションの更新
    if (enemy->animation > 0) {
        --enemy->animation;
    }

    // スプライトの更新
    enemy->sprite = ENEMY_SPRITE_DEAD + 0x04 * enemy->animation / ENEMY_ANIMATION_DEAD_INTERVAL;

    // 死亡の完了
    if (enemy->animation == 0) {

        // スコアの加算
        GameAddScore(enemy->color);

        // エネミーの遷移
        enemy->animation = ENEMY_ANIMATION_IDLE;
        EnemyTransit(enemy, EnemyIdle);
    }
}

// エネミーが分岐を曲がる
//
static void EnemyTurn(struct Enemy *enemy)
{
    // 分岐がある場合はモンスター別に曲がる
    if (MapGetBranchCount(enemy->positions) >= 3) {

        // プレイヤとの位置関係の取得
        unsigned char *p = PlayerGetPositions();
        signed char dx = (signed char)(p[_X_] - enemy->positions[_X_]);
        signed char dy = (signed char)(p[_Y_] - enemy->positions[_Y_]);
        unsigned char ax = abs(dx);
        unsigned char ay = abs(dy);

        // 赤は近づく
        if (enemy->color == VDP_COLOR_LIGHT_RED) {
            if (ay >= ax) {
                unsigned char d = dy <= 0 ? ENEMY_DIRECTION_UP : ENEMY_DIRECTION_DOWN;
                if (enemy->direction != (d ^ 0x01) && MapIsTurn(enemy->positions, d)) {
                    enemy->direction = d;
                } else {
                    d = dx <= 0 ? ENEMY_DIRECTION_LEFT : ENEMY_DIRECTION_RIGHT;
                    if (enemy->direction != (d ^ 0x01) && MapIsTurn(enemy->positions, d)) {
                        enemy->direction = d;
                    }
                }
            } else {
                unsigned char d = dx <= 0 ? ENEMY_DIRECTION_LEFT : ENEMY_DIRECTION_RIGHT;
                if (enemy->direction != (d ^ 0x01) && MapIsTurn(enemy->positions, d)) {
                    enemy->direction = d;
                } else {
                    d = dy <= 0 ? ENEMY_DIRECTION_UP : ENEMY_DIRECTION_DOWN;
                    if (enemy->direction != (d ^ 0x01) && MapIsTurn(enemy->positions, d)) {
                        enemy->direction = d;
                    }
                }
            }

        // 緑は遠ざかる
        } else if (enemy->color == VDP_COLOR_LIGHT_GREEN) {
            if (ay <= ax) {
                unsigned char d = dy >= 0 ? ENEMY_DIRECTION_UP : ENEMY_DIRECTION_DOWN;
                if (enemy->direction != (d ^ 0x01) && MapIsTurn(enemy->positions, d)) {
                    enemy->direction = d;
                } else {
                    d = dx >= 0 ? ENEMY_DIRECTION_LEFT : ENEMY_DIRECTION_RIGHT;
                    if (enemy->direction != (d ^ 0x01) && MapIsTurn(enemy->positions, d)) {
                        enemy->direction = d;
                    }
                }
            } else {
                unsigned char d = dx >= 0 ? ENEMY_DIRECTION_LEFT : ENEMY_DIRECTION_RIGHT;
                if (enemy->direction != (d ^ 0x01) && MapIsTurn(enemy->positions, d)) {
                    enemy->direction = d;
                } else {
                    d = dy >= 0 ? ENEMY_DIRECTION_UP : ENEMY_DIRECTION_DOWN;
                    if (enemy->direction != (d ^ 0x01) && MapIsTurn(enemy->positions, d)) {
                        enemy->direction = d;
                    }
                }
            }

        // 青はランダム
        } else {
            unsigned char d = CGetRandom() & 0x03;
            while (enemy->direction == (d ^ 0x01) || !MapIsTurn(enemy->positions, d)) {
                d = (d + 1) & 0x03;
            }
            enemy->direction = d;
        }
    }

    // 進めないときは道なりに曲がる
    if (!MapIsTurn(enemy->positions, enemy->direction)) {
        enemy->direction = MapTurn(enemy->positions, enemy->direction);
    }
}

