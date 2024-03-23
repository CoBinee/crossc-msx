// Game.c : ゲーム
//


// 参照ファイル
//
#include <string.h>
#include "C.h"
#include "App.h"
#include "Game.h"
#include "Map.h"
#include "Player.h"
#include "Enemy.h"

// 外部参照関数
//
extern void TitleInitialize(void);

// 外部参照変数
//


// 内部関数の宣言
//
static void GameTransit(void (*process)(void));
static void GameIdle(void);
static void GameStart(void);
static void GamePlay(void);
static void GameTimeup(void);
static void GameResult(void);
static void GamePrintScreen(void);
static void GamePrintStatus(void);
static void GamePrintString(unsigned char x, unsigned char y, char *string);
static void GamePrintValue(unsigned char x, unsigned char y, unsigned char value);

// 変数の定義
//

// ゲーム
static struct Game game;

// BGM
static const char gameBgmPlay_A[] = 
    "T2@0V15,4"
    "L3O4D5D5FDDFF+GG+AG+F5D"
    "L3O4A5ABO5CDEFECDO4BO5CO4A5A"
    "L3O4D5D5FD5FF+GG+AG+F5D"
    "L3O4E5B5BO5CO4BAGEF+D+EEE5"
    "L3O5D5F+DF5DF+DFDF+DDD5"
    "L3O5D5F+DF5DF+DFDF+DDD5"
    "L3O5F+5F+5DF5F+DD-CO4BO5CD-D-5"
    "L3O5F+5F+5DF5F+DD-CO4BO5CD-D5"
    "\xff";
static const char gameBgmPlay_B[] = 
    "T2@0V13,4"
    "L3O3DDDDDDDDDDDDDDDD"
    "L3O3EEEEEEEEEEG+G+AAAA"
    "L3O3DDDDDDDDDDDDDDDD"
    "L3O3E5EF+GABO4CO3BGAF+EEEE"
    "L3O3DDD-D-CCO2BBB-B-AAA-GF+5"
    "L3O3DDD-D-CCO2BBB-B-AAA-GF+5"
    "L3O4DDD-D-CCO3BBB-B-AAA-GF+5"
    "L3O3DDD-D-CCO2BBB-B-AAA-GF+5"
    "\xff";
static const char gameBgmTimeup_A[] = 
    "T2@0V15,4"
    "O5C4O4G1O5C3D4O4A1O5D3E4E1E3E8R7";
static const char gameBgmTimeup_B[] = 
    "T2@0V15,4"
    "O4G4E1G3A4F+1A3B4B1B3B8R7";
static const char gameBgmTimeup_C[] = 
    "T2@0V15,4"
    "O3E4C1E3F+4D1F+3G+8R5R7";
static const char gameBgmResult_A[] = 
    "T2@0V15,4"
    "L3O5F+5F+5E5F+RF+G5G5F+GR"
    "L3O5GA5A5AARAG5G5GGR"
    "L3O5GF+5F+5EF+RF+G5G5F+GR"
    "L3O5GA5A5GGRF+RR5R7";
static const char gameBgmResult_B[] = 
    "T2@0V13,4"
    "L3O4R7A5R5R7RAR5"
    "L9RR"
    "L3O4R7RAR5R7RAR5"
    "L9RR";


// ゲームを初期化する
//
void GameInitialize(void)
{
    // スプライトのクリア
    CClearSprite();

    // ゲームの初期化
    {
        memset(&game, 0x00, sizeof (struct Game));
        game.times[GAME_TIME_SECOND_10] = 9;
        game.times[GAME_TIME_SECOND_01] = 9;
    //  game.times[GAME_TIME_FRAME] = 0;
    }

    // ゲーム画面の描画
    GamePrintScreen();

    // マップの初期化
    MapInitialize();

    // プレイヤの初期化
    PlayerInitialize();

    // エネミーの初期化
    EnemyInitialize();

    // アナウンス領域のコピー
    memcpy(game.announceBacks, &patternName[0x0b * 0x20 + 0x00], 0x40);

    // カラーテーブルの設定
    videoRegister[VDP_R3] = APP_COLOR_TABLE_DEFAULT >> 6;

    // 描画の開始
    videoRegister[VDP_R1] |= (1 << VDP_R1_BL);

    // ゲームの遷移
    GameTransit(GameStart);

    // アプリケーションの遷移
    AppTransit(GameUpdate);
}

// ゲームを更新する
//
void GameUpdate(void)
{
    // スプライトのクリア
    CClearSprite();

    // 処理の実行
    game.process();
}

// ゲームを遷移する
//
static void GameTransit(void (*process)(void))
{
    game.process = process;
    game.state = GAME_STATE_NULL;
}

// ゲームを待機する
//
static void GameIdle(void)
{
    // 初期化
    if (game.state == 0x00) {

        // 初期化の完了
        ++game.state;
    }
}

// ゲームを開始する
//
static void GameStart(void)
{
    // 初期化
    if (game.state == 0x00) {

        // 操作させない
        game.unplay = 1;

        // アナウンスの設定
        memset(&patternName[0x0b * 0x20 + 0x00], 0x00, 0x40);
        game.announce = GAME_ANNOUNCE_START;

        // フレームの設定
        game.frame = 0x30;

        // BGM の再生
        CPlayBgm(gameBgmPlay_A, gameBgmPlay_B, NULL);

        // 初期化の完了
        ++game.state;
    }

    // マップの更新
    MapUpdate();

    // プレイヤの更新
    PlayerUpdate();

    // エネミーの更新
    EnemyUpdate();

    // マップの描画
    MapRender();

    // プレイヤの描画
    PlayerRender();

    // エネミーの描画
    EnemyRender();

    // ステータスの描画
    GamePrintStatus();

    // フレームの更新
    if (--game.frame == 0) {

        // ゲームの遷移
        GameTransit(GamePlay);
    }
}

// ゲームをプレイする
//
static void GamePlay(void)
{
    // 初期化
    if (game.state == 0x00) {

        // フラグの設定
        game.flag |= GAME_FLAG_TIME;

        // アナウンスの設定
        memcpy(&patternName[0x0b * 0x20 + 0x00], game.announceBacks, 0x40);
        game.announce = GAME_ANNOUNCE_NULL;

        // 初期化の完了
        ++game.state;
    }

    // 操作の更新
    if (game.unplay > 0) {
        --game.unplay;
    }

    // ヒット処理
    if (game.unplay == 0) {
        EnemyHit();
    }

    // マップの更新
    MapUpdate();

    // プレイヤの更新
    PlayerUpdate();

    // エネミーの更新
    EnemyUpdate();

    // 時間の更新
    {
        if ((game.flag & GAME_FLAG_TIME) != 0) {
            unsigned char *t = &game.times[GAME_TIME_FRAME];
            if (*t > 0) {
                --*t;
            } else {
                *t-- = 29;
                if (*t > 0) {
                    --*t;
                } else {
                    *t-- = 9;
                    if (*t > 0) {
                        --*t;
                    } else {
                        *t++ = 0;
                        *t++ = 0;
                        *t   = 0;
                        game.flag &= ~GAME_FLAG_TIME;
                    }
                }
            }
        }
    }

    // マップの描画
    MapRender();

    // プレイヤの描画
    PlayerRender();

    // エネミーの描画
    EnemyRender();

    // ステータスの描画
    GamePrintStatus();

    // タイムアップの判定
    if ((game.flag & GAME_FLAG_TIME) == 0) {

        // ゲームの遷移
        GameTransit(GameTimeup);
    }
}

// タイムアップする
//
static void GameTimeup(void)
{
    // 初期化
    if (game.state == 0x00) {

        // 操作させない
        game.unplay = 1;

        // アナウンスの設定
        memset(&patternName[0x0b * 0x20 + 0x00], 0x00, 0x40);
        game.announce = GAME_ANNOUNCE_TIMEUP;

        // フレームの設定
        game.frame = 0x60;

        // BGM の再生
        CPlayBgm(gameBgmTimeup_A, gameBgmTimeup_B, gameBgmTimeup_C);

        // 初期化の完了
        ++game.state;
    }

    // マップの更新
    MapUpdate();

    // プレイヤの更新
    PlayerUpdate();

    // エネミーの更新
    EnemyUpdate();

    // マップの描画
    MapRender();

    // プレイヤの描画
    PlayerRender();

    // エネミーの描画
    EnemyRender();

    // ステータスの描画
    GamePrintStatus();

    // フレームの更新
    if (--game.frame == 0) {

        // ゲームの遷移
        GameTransit(GameResult);
    }
}

// 結果を表示する
//
static void GameResult(void)
{
    // 初期化
    if (game.state == 0x00) {

        // パターンネームのクリア
        CClearPatternName(0x00);

        // 操作させない
        game.unplay = 1;

        // フレームの設定
        game.frame = 0;

        // BGM の再生
        CPlayBgm(gameBgmResult_A, gameBgmResult_B, NULL);

        // 初期化の完了
        ++game.state;
    }

    // フレームの更新
    if (game.frame > 0) {
        --game.frame;
    }
    if (game.frame == 0) {

        // 画面の更新
        switch (game.state) {
        case 0x01:
            {
                GamePrintString(0x0d, 0x04, "RESULT");
                game.frame = 0x18;
                ++game.state;
            }
            break;
        case 0x02:
            {
                GamePrintString(0x03, 0x07, "  RED MONSTER");
                GamePrintValue(0x15, 0x07, game.monsters[GAME_MONSTER_RED]);
                game.frame = 0x18;
                ++game.state;
            }
            break;
        case 0x03:
            {
                GamePrintString(0x03, 0x09, "GREEN MONSTER");
                GamePrintValue(0x15, 0x09, game.monsters[GAME_MONSTER_GREEN]);
                game.frame = 0x18;
                ++game.state;
            }
            break;
        case 0x04:
            {
                GamePrintString(0x03, 0x0b, " BLUE MONSTER");
                GamePrintValue(0x15, 0x0b, game.monsters[GAME_MONSTER_BLUE]);
                game.frame = 0x18;
                ++game.state;
            }
            break;
        case 0x05:
            {
                GamePrintString(0x03, 0x0d, "    MAX COMBO");
                GamePrintValue(0x15, 0x0d, game.comboMaximum);
                game.frame = 0x18;
                ++game.state;
            }
            break;
        case 0x06:
            {
                GamePrintString(0x03, 0x10, "        SCORE");
                {
                    unsigned char *p = &patternName[0x10 * 0x20 + 0x10];
                    unsigned char i = 0;
                    while (i < GAME_SCORE_SIZE - 1 && game.scores[i] == 0) {
                        *p++ = ' ' - ' ';
                        ++i;
                    }
                    while (i < GAME_SCORE_SIZE) {
                        *p++ = game.scores[i++] + '0' - ' ';
                    }
                }
                game.frame = 0x18;
                ++game.state;
            }
            break;
        case 0x07:
            {
                if (AppRecordScore(game.scores)) {
                    GamePrintString(0x0b, 0x13, "TOP SCORE!");
                    game.frame = 0x18;
                }
                ++game.state;
            }
            break;
        case 0x08:
            {
                if (CIsInputEdge(INPUT_BUTTON_SPACE)) {
                    game.frame = 0x18;
                    CStopSound();
                    CPlaySe("T2@0V15O4B0");
                    ++game.state;
                }
            }
            break;
        default:
            AppTransit(TitleInitialize);
            break;
        }
    }
}

// ゲームの操作不能時間を設定する
//
void GameSetUnplay(unsigned char unplay)
{
    game.unplay = unplay;
}

// ゲームが操作可能かどうかを判定する
//
bool GameIsPlay(void)
{
    return game.unplay == 0 ? true : false;
}

// スコアを加算する
//
void GameAddScore(unsigned char color)
{
    // コンボの更新
    if (game.color == color) {
        unsigned char *p = &game.combos[GAME_COMBO_01];
        if (++*p >= 10) {
            *p = 0;
            --p;
            if (++*p >= 10) {
                *p++ = 9;
                *p   = 9;
            }
        }
    } else {
        unsigned char m = game.combos[GAME_COMBO_10] * 10 + game.combos[GAME_COMBO_01];
        if (game.comboMaximum < m) {
            game.comboMaximum = m;
        }
        game.combos[GAME_COMBO_10] = 0;
        game.combos[GAME_COMBO_01] = 0;
    }

    // スコアの加算
    {
        unsigned char *p = &game.scores[GAME_SCORE_00000100];
        unsigned char n = game.combos[GAME_COMBO_10];
        *p += game.combos[GAME_COMBO_01] + 1;
        if (*p >= 10) {
            *p -= 10;
            ++n;
        }
        --p;
        *p += n;
        if (*p >= 10) {
            *p -= 10;
            --p;
            {
                signed char i = GAME_SCORE_00010000;
                while (i >= 0 && ++*p >= 10) {
                    *p = 0;
                    --p;
                    --i;
                }
                if (i < 0) {
                    for (i = 0; i < GAME_SCORE_00000010; i++) {
                        *++p = 9;
                    }
                }
            }
        }
    }

    // 色とモンスターの更新
    game.color = color;
    if (color == VDP_COLOR_LIGHT_RED) {
        ++game.monsters[GAME_MONSTER_RED];
        videoRegister[VDP_R3] = APP_COLOR_TABLE_RED >> 6;
    } else if (color == VDP_COLOR_LIGHT_GREEN) {
        ++game.monsters[GAME_MONSTER_GREEN];
        videoRegister[VDP_R3] = APP_COLOR_TABLE_GREEN >> 6;
    } else if (color == VDP_COLOR_LIGHT_BLUE) {
        ++game.monsters[GAME_MONSTER_BLUE];
        videoRegister[VDP_R3] = APP_COLOR_TABLE_BLUE >> 6;
    } else {
        videoRegister[VDP_R3] = APP_COLOR_TABLE_DEFAULT >> 6;
    }
}

// コンボを取得する
//
unsigned char GameGetCombo(void)
{
    return game.combos[GAME_COMBO_10] * 10 + game.combos[GAME_COMBO_01];
}

// ゲーム画面を描画する
//
static void GamePrintScreen(void)
{
    // パターンネームのクリア
    CClearPatternName(0x00);

    // スコアの描画
    GamePrintString(0x00, 0x00, "SCORE        0");

    // コンボの描画

    // タイムの描画
    GamePrintString(0x16, 0x17, "TIME 90\"00");

    // 速度の描画
    GamePrintString(0x00, 0x17, ">");
}

// ステータスを描画する
//
static void GamePrintStatus(void)
{
    // スコアの描画
    {
        unsigned char *p = &patternName[0x00 * 0x20 + 0x06];
        unsigned char i = 0;
        while (i < GAME_SCORE_SIZE - 1 && game.scores[i] == 0) {
            *p++ = ' ' - ' ';
            ++i;
        }
        while (i < GAME_SCORE_SIZE) {
            *p++ = game.scores[i++] + '0' - ' ';
        }
    }

    // コンボの描画
    {
        if (game.combos[GAME_COMBO_10] == 0 && game.combos[GAME_COMBO_01] == 0) {
            GamePrintString(0x17, 0x00, "         ");
        } else if (game.combos[GAME_COMBO_10] == 0 && game.combos[GAME_COMBO_01] == 1) {
            GamePrintString(0x17, 0x00, "   COMBO ");
        } else {
            unsigned char *p = &patternName[0x00 * 0x20 + 0x17];
            if (game.combos[GAME_COMBO_10] > 0) {
                *p++   = game.combos[GAME_COMBO_10] + '0' - ' ';
            } else {
                *p++ = ' ' - ' ';
            }
            *p   = game.combos[GAME_COMBO_01] + '0' - ' ';
            GamePrintString(0x19, 0x00, " COMBOS");
        }
    }

    // タイムの描画
    {
        unsigned char *p = &patternName[0x17 * 0x20 + 0x1b];
        if (game.times[GAME_TIME_SECOND_10] > 0) {
            *p++ = game.times[GAME_TIME_SECOND_10] + '0' - ' ';
        } else {
            *p++ = ' ' - ' ';
        }
        *p++ = game.times[GAME_TIME_SECOND_01] + '0' - ' ';
        ++p;
        {
            static const unsigned char frames[] = {
                0x00, 0x03, 0x06, 0x10, 0x13, 0x16, 0x20, 0x23, 0x26, 0x30, 
                0x33, 0x36, 0x40, 0x43, 0x46, 0x50, 0x53, 0x56, 0x60, 0x63, 
                0x66, 0x70, 0x73, 0x76, 0x80, 0x83, 0x86, 0x90, 0x93, 0x96, 
            };
            unsigned char f = frames[game.times[GAME_TIME_FRAME]];
            *p++ = ((f >> 4) & 0x0f) + '0' - ' ';
            *p   = (f & 0x0f) + '0' - ' ';
        }
    }

    // アナウンスの描画
    if (game.announce != 0) {
        static const unsigned char sprites[][0x10] = {
            {
                0x58 - 0x01, 0x60, 0x80, VDP_COLOR_WHITE, 
                0x58 - 0x01, 0x70, 0x84, VDP_COLOR_WHITE, 
                0x58 - 0x01, 0x80, 0x88, VDP_COLOR_WHITE, 
                0x58 - 0x01, 0x90, 0x8c, VDP_COLOR_WHITE, 
            }, 
            {
                0x58 - 0x01, 0x60, 0x90, VDP_COLOR_WHITE, 
                0x58 - 0x01, 0x70, 0x94, VDP_COLOR_WHITE, 
                0x58 - 0x01, 0x80, 0x98, VDP_COLOR_WHITE, 
                0x58 - 0x01, 0x90, 0x9c, VDP_COLOR_WHITE, 
            }, 
        };
        memcpy(&sprite[GAME_SPRITE_ANNOUNCE], sprites[game.announce - 1], 0x10);
    }

    // 速度の描画
    {
        unsigned char *p = &patternName[0x17 * 0x20 + 0x00];
        unsigned char s = PlayerGetSpeed();
        unsigned char i = PLAYER_SPEED_MINIMUM;
        while (i <= s) {
            *p++ = '>' - ' ';
            ++i;
        }
        while (i <= PLAYER_SPEED_MAXIMUM) {
            *p++ = ' ' - ' ';
            ++i;
        }
    }
}

// 文字列を描画する
//
static void GamePrintString(unsigned char x, unsigned char y, char *string)
{
    unsigned char *p = &patternName[y * 0x20 + x];
    while (*string != '\0') {
        *p++ = *string++ - ' ';
    }
}

// 数値を描画する
//
static void GamePrintValue(unsigned char x, unsigned char y, unsigned char value)
{
    unsigned char *p = &patternName[y * 0x20 + x];
    unsigned char n_10 = value / 10;
    unsigned char n_01 = value % 10;
    if (n_10 > 0) {
        if (n_10 >= 20) {
            *p++ = '2' - ' ';
            n_10 -= 20;
        } else if (n_10 >= 10) {
            *p++ = '1' - ' ';
            n_10 -= 10;
        } else {
            *p++ = ' ' - ' ';
        }
        *p++ = n_10 + '0' - ' ';
    } else {
        *p++ = ' ' - ' ';
        *p++ = ' ' - ' ';
    }
    *p = n_01 + '0' - ' ';
}
