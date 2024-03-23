// Title.h : タイトル
//
#ifndef _TITLE_H_
#define _TITLE_H_


// 参照ファイル
//
#include "Const.h"


// 定数宣言
//

// スプライト

// 状態
#define TITLE_STATE_NULL                 0x00

// フラグ
#define TITLE_FLAG_NULL                  0x00

// フレーム


// 型宣言
//

// タイトル
struct Title {

    // 処理
    void (*process)(void);

    // 状態
    unsigned char state;

    // フラグ
    unsigned char flag;

    // フレーム
    unsigned char frame;

};


// 外部関数宣言
//
extern void TitleInitialize(void);
extern void TitleUpdate(void);


// 外部変数宣言
//


#endif /* _TITLE_H_ */
