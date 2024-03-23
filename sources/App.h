// App.h : アプリケーション
//
#ifndef _APP_H_
#define _APP_H_


// 参照ファイル
//
#include "Const.h"


// 定数宣言
//

// ビデオ
#define APP_COLOR_TABLE                 0x2000
#define APP_COLOR_TABLE_DEFAULT         0x2000
#define APP_COLOR_TABLE_RED             0x2040
#define APP_COLOR_TABLE_GREEN           0x2080
#define APP_COLOR_TABLE_BLUE            0x20c0
#define APP_PATTERN_NAME_TABLE          0x2800
#define APP_PATTERN_GENERATOR_TABLE     0x0000
#define APP_SPRITE_ATTRIBUTE_TABLE      0x1b00
#define APP_SPRITE_GENERATOR_TABLE      0x3800

// スコア
#define APP_SCORE_10000000              0x00
#define APP_SCORE_01000000              0x01
#define APP_SCORE_00100000              0x02
#define APP_SCORE_00010000              0x03
#define APP_SCORE_00001000              0x04
#define APP_SCORE_00000100              0x05
#define APP_SCORE_00000010              0x06
#define APP_SCORE_00000001              0x07
#define APP_SCORE_SIZE                  0x08


// 型宣言
//

// アプリケーション
struct App {

    // 処理
    void (*process)(void);

    // スコア
    unsigned char scores[APP_SCORE_SIZE];

};


// 外部関数宣言
//
extern void AppTransit(void (*process)(void));
extern unsigned char *AppGetScore(void);
extern bool AppRecordScore(unsigned char *scores);


// 外部変数宣言
//


#endif /* _APP_H_ */
