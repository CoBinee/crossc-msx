// Game.h : ゲーム
//
#ifndef _GAME_H_
#define _GAME_H_


// 参照ファイル
//
#include "Const.h"


// 定数宣言
//

// スプライト
#define GAME_SPRITE_PLAYER_0            0x10
#define GAME_SPRITE_PLAYER_1            0x40
#define GAME_SPRITE_ENEMY               0x20
#define GAME_SPRITE_ANNOUNCE            0x00

// 状態
#define GAME_STATE_NULL                 0x00

// フラグ
#define GAME_FLAG_NULL                  0x00
#define GAME_FLAG_TIME_BIT              0x00
#define GAME_FLAG_TIME                  (1 << GAME_FLAG_TIME_BIT)

// フレーム

// 操作不能時間

// スコア
#define GAME_SCORE_10000000             0x00
#define GAME_SCORE_01000000             0x01
#define GAME_SCORE_00100000             0x02
#define GAME_SCORE_00010000             0x03
#define GAME_SCORE_00001000             0x04
#define GAME_SCORE_00000100             0x05
#define GAME_SCORE_00000010             0x06
#define GAME_SCORE_00000001             0x07
#define GAME_SCORE_SIZE                 0x08

// コンボ
#define GAME_COMBO_10                   0x00
#define GAME_COMBO_01                   0x01
#define GAME_COMBO_SIZE                 0x02

// 色

// モンスター
#define GAME_MONSTER_RED                0x00
#define GAME_MONSTER_GREEN              0x01
#define GAME_MONSTER_BLUE               0x02
#define GAME_MONSTER_SIZE               0x03

// タイム
#define GAME_TIME_SECOND_10             0x00
#define GAME_TIME_SECOND_01             0x01
#define GAME_TIME_FRAME                 0x02
#define GAME_TIME_SIZE                  0x03

// アナウンス
#define GAME_ANNOUNCE_NULL              0x00
#define GAME_ANNOUNCE_START             0x01
#define GAME_ANNOUNCE_TIMEUP            0x02


// 型宣言
//

// ゲーム
struct Game {

    // 処理
    void (*process)(void);

    // 状態
    unsigned char state;

    // フラグ
    unsigned char flag;

    // フレーム
    unsigned char frame;

    // 操作不能時間
    unsigned char unplay;

    // スコア
    unsigned char scores[GAME_SCORE_SIZE];

    // コンボ
    unsigned char combos[GAME_COMBO_SIZE];
    unsigned char comboMaximum;

    // 色
    unsigned char color;

    // モンスター
    unsigned char monsters[GAME_MONSTER_SIZE];

    // タイム
    unsigned char times[GAME_TIME_SIZE];

    // アナウンス
    unsigned char announce;
    unsigned char announceBacks[0x40];

};


// 外部関数宣言
//
extern void GameInitialize(void);
extern void GameUpdate(void);
extern void GameSetUnplay(unsigned char unplay);
extern bool GameIsPlay(void);
extern void GameAddScore(unsigned char color);
extern unsigned char GameGetCombo(void);


// 外部変数宣言
//


#endif /* _GAME_H_ */
