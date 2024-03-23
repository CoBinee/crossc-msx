// Title.c : タイトル
//


// 参照ファイル
//
#include <string.h>
#include "C.h"
#include "App.h"
#include "Title.h"

// 外部参照関数
//
extern void GameInitialize(void);

// 外部参照変数
//


// 内部関数の宣言
//
static void TitleTransit(void (*process)(void));
static void TitleIdle(void);
static void TitleStart(void);
static void TitlePrintScreen(void);
static void TitlePrintHitSpaceBar(bool print);
static void TitlePrintString(unsigned char x, unsigned char y, char *string);

// 変数の定義
//

// タイトル
static struct Title title;

// BGM
static const char titleBgm_A[] = 
    "T2@0V15,4"
    "L3O6C5O5FO6C6C4C1CO5B-O6CD"
    "L3O6C5O5FO6C6C4C1CO5B-O6CD"
    "L3O6C4O5F1FB-O6CDC4O5F1FB-O6CD"
    "L3O3O6C4O5F1FB-O6CDC4O5A1O6CD4O5B1O6D"
    "L9O6ER7"
    "L9O6E";
static const char titleBgm_B[] = 
    "T2@0V13,4"
    "L1O4A6A3AAAAA6B-6"
    "L1O4A6A3AAAAA6B-6"
    "L1O4A6B-3B-B-B-B-A6B-3B-B-B-B-"
    "L1O4A6B-3B-B-B-B-A3AAAAB3BBBB"
    "L1O5E4O4AO5E3E4O4AO5E3E4O4AO5E3E4O4AO5E3"
    "L9O5A";
static const char titleBgm_C[] = 
    "T2@0V13,4"
    "L3O4F9RDCO3B-"
    "L3O4F9RDCO3B-"
    "L3O4F6DCO3B-O4F6DCO3B-"
    "L3O4F6DCO3B-O4F8"
    "L1O5C+4O4C+O5C+5RO4C+O5C+5RO4C+O5C+5RO4C+O5"
    "L7O5C+R3R";


// タイトルを初期化する
//
void TitleInitialize(void)
{
    // スプライトのクリア
    CClearSprite();

    // タイトルの初期化
    memset(&title, 0x00, sizeof (struct Title));

    // タイトル画面の描画
    TitlePrintScreen();

    // カラーテーブルの設定
    videoRegister[VDP_R3] = APP_COLOR_TABLE >> 6;

    // 描画の開始
    videoRegister[VDP_R1] |= (1 << VDP_R1_BL);

    // タイトルの遷移
    TitleTransit(TitleIdle);

    // アプリケーションの遷移
    AppTransit(TitleUpdate);
}

// タイトルを更新する
//
void TitleUpdate(void)
{
    // スプライトのクリア
    CClearSprite();

    // 処理の実行
    title.process();
}

// タイトルを遷移する
//
static void TitleTransit(void (*process)(void))
{
    title.process = process;
    title.state = TITLE_STATE_NULL;
}

// タイトルを待機する
//
static void TitleIdle(void)
{
    // 初期化
    if (title.state == 0x00) {

        // フレームの設定
        title.frame = 0;

        // BGM の再生
        CPlayBgm(titleBgm_A, titleBgm_B, titleBgm_C);

        // 初期化の完了
        ++title.state;
    }

    // ロゴの色の更新
    videoRegister[VDP_R3] = (APP_COLOR_TABLE >> 6) + ((title.frame >> 5) & 0x03);

    // HIT SPACE BAR の描画
    TitlePrintHitSpaceBar((title.frame & 0x10) == 0 ? true : false);

    // フレームの更新
    ++title.frame;

    // タイトルの遷移
    if (CIsInputEdge(INPUT_BUTTON_SPACE)) {
        TitleTransit(TitleStart);
    }
}

// タイトルを開始する
//
static void TitleStart(void)
{
    // 初期化
    if (title.state == 0x00) {

        // フレームの設定
        title.frame = 0x30;

        // SE の再生
        CStopSound();
        CPlaySe("T2@0V15L3O6BO5BR9");

        // 初期化の完了
        ++title.state;
    }

    // HIT SPACE BAR の描画
    TitlePrintHitSpaceBar((title.frame & 0x02) == 0 ? true : false);

    // アプリケーションの遷移
    if (--title.frame == 0) {
        AppTransit(GameInitialize);
    }
}

// タイトル画面を描画する
//
static void TitlePrintScreen(void)
{
    // パターンネームのクリア
    CClearPatternName(0x00);

    // ロゴの描画
    {
        static const unsigned char logos[] = {
            0x40, 0x41, 0x64, 0x65, 0x4c, 0x4d, 0x40, 0x41, 
            0x50, 0x51, 0x74, 0x75, 0x5c, 0x5d, 0x50, 0x51, 
            0x64, 0x65, 0x4a, 0x4b, 0x62, 0x63, 0x4c, 0x4d, 
            0x74, 0x75, 0x5a, 0x5b, 0x72, 0x73, 0x5c, 0x5d, 
            0x62, 0x63, 0x4c, 0x4d, 0x64, 0x65, 0x4a, 0x4b, 
            0x72, 0x73, 0x5c, 0x5d, 0x74, 0x75, 0x5a, 0x5b, 
            0x40, 0x41, 0x62, 0x63, 0x4a, 0x4b, 0x40, 0x41, 
            0x50, 0x51, 0x72, 0x73, 0x5a, 0x5b, 0x50, 0x51, 
        };
        unsigned char *dst = &patternName[0x04 * 0x20 + 0x0c];
        const unsigned char *src = logos;
        for (unsigned char i = 0; i < 8; i++) {
            for (unsigned char j = 0; j < 8; j++) {
                *dst++ = *src++;
            }
            dst += 0x20 - 0x08;
        }
    }
    
    // スコアの描画
    {
        unsigned char *p = &patternName[0x10 * 0x20 + 0x09];
        *p++ = 'T' - ' ';
        *p++ = 'O' - ' ';
        *p++ = 'P' - ' ';
        *p++ = ' ' - ' ';
        *p++ = ' ' - ' ';
        {
            unsigned char *s = AppGetScore();
            unsigned char i = 0;
            while (i < APP_SCORE_SIZE - 1 && s[i] == 0) {
                *p++ = ' ' - ' ';
                ++i;
            }
            while (i < APP_SCORE_SIZE) {
                *p++ = s[i++] + '0' - ' ';
            }

        }
    }
}

// HIT SPACE BAR を描画する
//
static void TitlePrintHitSpaceBar(bool print)
{
    TitlePrintString(0x09, 0x14, print ? "HIT SPACE BAR" : "             ");
}

// 文字列を描画する
//
static void TitlePrintString(unsigned char x, unsigned char y, char *string)
{
    unsigned char *p = &patternName[y * 0x20 + x];
    while (*string != '\0') {
        *p++ = *string++ - ' ';
    }
}

