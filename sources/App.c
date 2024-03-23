// App.c : アプリケーション
//


// 参照ファイル
//
#include <string.h>
#include "C.h"
#include "App.h"

// 外部参照関数
//
extern void TitleInitialize(void);

// 外部参照変数
//
extern unsigned char patternTable[];
extern unsigned char spriteTable[];


// 内部関数の宣言
//
static void AppDebug(void);

// 変数の定義
//

// アプリケーション
static struct App app;

// ビデオレジスタの初期値
static const unsigned char videoRegisterDefault[] = {
    0b00000000, 
    0b10100010, 
    APP_PATTERN_NAME_TABLE >> 10, 
    APP_COLOR_TABLE >> 6, 
    APP_PATTERN_GENERATOR_TABLE >> 11, 
    APP_SPRITE_ATTRIBUTE_TABLE >> 7, 
    APP_SPRITE_GENERATOR_TABLE >> 11, 
    0b00000111, 
};

// カラーテーブル
static const unsigned char colorTables[][0x20] = {
    {
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_YELLOW  << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_YELLOW   << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_YELLOW  << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_YELLOW   << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_YELLOW  << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_YELLOW   << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_YELLOW  << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_YELLOW   << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
    }, 
    {
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_RED     << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_RED      << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_RED     << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_RED      << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_RED     << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_RED      << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_RED     << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_RED      << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
    }, 
    {
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_GREEN   << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_GREEN    << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_GREEN   << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_GREEN    << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_GREEN   << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_GREEN    << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_GREEN   << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_GREEN    << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
    }, 
    {
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_BLUE    << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_BLUE     << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_BLUE    << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_BLUE     << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_BLUE    << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_BLUE     << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_DARK_BLUE    << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_DARK_BLUE     << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
        (unsigned char)((VDP_COLOR_WHITE        << 4) | VDP_COLOR_BLACK),        (unsigned char)((VDP_COLOR_WHITE         << 4) | VDP_COLOR_BLACK), 
    }, 
};


// アプリケーションを初期化する
//
void AppInitialize(void)
{
    // 画面表示の停止
    CDisableScreen();

    // ビデオの設定
    memcpy(videoRegister, videoRegisterDefault, 0x0008);

    // 割り込みの禁止
    CDisableInterrupt();

    // スプライトジェネレータの転送
    CLoadVram((void *)APP_SPRITE_GENERATOR_TABLE, spriteTable + 0x0000, 0x0800);

    // パターンジェネレータの転送
    CLoadVram((void *)APP_PATTERN_GENERATOR_TABLE, patternTable + 0x0000, 0x0800);

    // カラーテーブルの初期化
    CLoadVram((void *)APP_COLOR_TABLE_DEFAULT, colorTables[0], 0x0020);
    CLoadVram((void *)APP_COLOR_TABLE_RED,     colorTables[1], 0x0020);
    CLoadVram((void *)APP_COLOR_TABLE_GREEN,   colorTables[2], 0x0020);
    CLoadVram((void *)APP_COLOR_TABLE_BLUE,    colorTables[3], 0x0020);

    // パターンネームの初期化
    CFillVram((void *)APP_PATTERN_NAME_TABLE, 0x00, 0x0300);

    // 割り込み禁止の解除
    CEnableInterrupt();

    // アプリケーションの初期化
    {
        memset(&app, 0x00, sizeof (struct App));
    //  app.scores[APP_SCORE_10000000] = 0;
    //  app.scores[APP_SCORE_01000000] = 0;
    //  app.scores[APP_SCORE_00100000] = 0;
    //  app.scores[APP_SCORE_00010000] = 0;
        app.scores[APP_SCORE_00001000] = 5;
    //  app.scores[APP_SCORE_00000100] = 0;
    //  app.scores[APP_SCORE_00000010] = 0;
    //  app.scores[APP_SCORE_00000001] = 0;
    }

    // アプリケーションの設定
    AppTransit(TitleInitialize);
}

// アプリケーションを更新する
//
void AppUpdate(void)
{
    // 乱数を混ぜる
    CGetRandom();

    // 処理の実行
    app.process();

    // デバッグの更新
    // AppDebug();
}

// アプリケーションを遷移する
//
void AppTransit(void (*process)())
{
    app.process = process;
}

// スコアを取得する
//
unsigned char *AppGetScore(void)
{
    return app.scores;
}

// スコアを更新する
//
bool AppRecordScore(unsigned char *scores)
{
    bool result = false;
    unsigned char i = 0;
    while (i < APP_SCORE_SIZE) {
        if (app.scores[i] > scores[i]) {
            break;
        } else if (app.scores[i] < scores[i]) {
            result = true;
            break;
        }
        ++i;
    }
    if (result) {
        memcpy(app.scores, scores, APP_SCORE_SIZE * sizeof (unsigned char));
    }
    return result;
}

// デバッグを更新する
//
static void AppDebug(void)
{
    unsigned char *p = &patternName[0x17 * 0x20 + 0x00];
    *p++ = 'D' - ' ';
    *p++ = 'B' - ' ';
    *p++ = 'G' - ' ';
    *p++ = '=' - ' ';
    for (unsigned char i = 0; i < DEBUG_SIZE; i++) {
        static const unsigned char codes[] = {
            0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 
        };
        *p++ = codes[(debug[i] >> 4) & 0x0f];
        *p++ = codes[debug[i] & 0x0f];
    }
    ++debug[DEBUG_7];
}
