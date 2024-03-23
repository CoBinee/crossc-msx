// Enemy.h : エネミー
//
#ifndef _ENEMY_H_
#define _ENEMY_H_


// 参照ファイル
//
#include "Const.h"


// 定数宣言
//

// 状態
#define ENEMY_STATE_NULL                0x00

// フラグ
#define ENEMY_FLAG_NULL                 0x00
#define ENEMY_FLAG_PLAY_BIT             0x00
#define ENEMY_FLAG_PLAY                 (1 << ENEMY_FLAG_PLAY_BIT)
#define ENEMY_FLAG_RENDER_BIT           0x07
#define ENEMY_FLAG_RENDER               (1 << ENEMY_FLAG_RENDER_BIT)

// 位置

// 向き
#define ENEMY_DIRECTION_UP              0x00
#define ENEMY_DIRECTION_DOWN            0x01
#define ENEMY_DIRECTION_LEFT            0x02
#define ENEMY_DIRECTION_RIGHT           0x03

// 速度
#define ENEMY_SPEED_MINIMUM             0x01
#define ENEMY_SPEED_MAXIMUM             0x08

// 色

// アニメーション
#define ENEMY_ANIMATION_IDLE            0x18
#define ENEMY_ANIMATION_BORN            0x18
#define ENEMY_ANIMATION_BORN_INTERVAL   0x02
#define ENEMY_ANIMATION_PLAY_CYCLE      0x03
#define ENEMY_ANIMATION_PLAY_INTERVAL   0x02
#define ENEMY_ANIMATION_DEAD_CYCLE      0x06
#define ENEMY_ANIMATION_DEAD_INTERVAL   0x02

// スプライト
#define ENEMY_SPRITE_PLAY               0x40
#define ENEMY_SPRITE_DEAD               0x60

// エネミー数
#define ENEMY_ENTRY                     0x06

// ヒット処理
#define ENEMY_HIT_DISTANCE              0x08


// 型宣言
//

// エネミー
struct Enemy {

    // 処理
    void (*process)(struct Enemy *);

    // 状態
    unsigned char state;

    // フラグ
    unsigned char flag;

    // 位置
    unsigned char positions[_XY_];

    // 向き
    unsigned char direction;

    // 速度
    unsigned char speed;

    // 色
    unsigned char color;

    // アニメーション
    unsigned char animation;

    // スプライト
    unsigned char sprite;

};


// 外部関数宣言
//
extern void EnemyInitialize(void);
extern void EnemyUpdate(void);
extern void EnemyRender(void);
extern void EnemyHit(void);


// 外部変数宣言
//


#endif /* _ENEMY_H_ */
