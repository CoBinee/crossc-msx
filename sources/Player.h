// Player.h : プレイヤ
//
#ifndef _PLAYER_H_
#define _PLAYER_H_


// 参照ファイル
//
#include "Const.h"


// 定数宣言
//

// 状態
#define PLAYER_STATE_NULL               0x00

// 位置

// 向き
#define PLAYER_DIRECTION_UP             0x00
#define PLAYER_DIRECTION_DOWN           0x01
#define PLAYER_DIRECTION_LEFT           0x02
#define PLAYER_DIRECTION_RIGHT          0x03

// 速度
#define PLAYER_SPEED_MINIMUM            0x02
#define PLAYER_SPEED_MAXIMUM            0x08

// アニメーション
#define PLAYER_ANIMATION_CYCLE          0x06
#define PLAYER_ANIMATION_INTERVAL       0x02


// 型宣言
//

// プレイヤ
struct Player {

    // 処理
    void (*process)(void);

    // 状態
    unsigned char state;

    // 位置
    unsigned char positions[_XY_];

    // 向き
    unsigned char direction;

    // 速度
    unsigned char speed;

    // アニメーション
    unsigned char animation;

};


// 外部関数宣言
//
extern void PlayerInitialize(void);
extern void PlayerUpdate(void);
extern void PlayerRender(void);
extern unsigned char *PlayerGetPositions(void);
extern unsigned char PlayerGetSpeed(void);


// 外部変数宣言
//


#endif /* _PLAYER_H_ */
