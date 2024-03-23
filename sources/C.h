// C.h : C ライブラリ
//
#ifndef _C_H_
#define _C_H_


// 参照ファイル
//
#include <stdbool.h>


// 割り込み
//

// 定数

// 関数
extern void CDisableInterrupt(void);
extern void CEnableInterrupt(void);

// 変数


// ビデオレジスタ
//

// 定数
#define VDP_R0                          0       // mode register #0
#define VDP_R0_M5                       3       // screen mode
#define VDP_R0_M4                       2       // screen mode
#define VDP_R0_M3                       1       // screen mode
#define VDP_R0_EXTVID                   0       // 1:enable external video input, 0:disable external video input
#define VDP_R1                          1       // mode register #1
#define VDP_R1_4_16K                    7       // 1:16KB RAM, 0: 4KB RAM
#define VDP_R1_BL                       6       // 1:screen on, 0:screen off
#define VDP_R1_GINT                     5       // generate interrupt
#define VDP_R1_M1                       4       // screen mode
#define VDP_R1_M2                       3       // screen mode
#define VDP_R1_SI                       1       // 1:16x16 sprite, 0:8x8 sprite
#define VDP_R1_MAG                      0       // 1:2x2 sprite pixels, 0:1x1 sprite pixels
#define VDP_R2                          2       // pattern name table
#define VDP_R2_PN13                     3       // pattern name table bit 13
#define VDP_R2_PN12                     2       // pattern name table bit 12
#define VDP_R2_PN11                     1       // pattern name table bit 11
#define VDP_R2_PN10                     0       // pattern name table bit 10
#define VDP_R3                          3       // color table
#define VDP_R3_CT13                     7       // color table bit 13
#define VDP_R3_CT12                     6       // color table bit 12
#define VDP_R3_CT11                     5       // color table bit 11
#define VDP_R3_CT10                     4       // color table bit 10
#define VDP_R3_CT9                      3       // color table bit 9
#define VDP_R3_CT8                      2       // color table bit 8
#define VDP_R3_CT7                      1       // color table bit 7
#define VDP_R3_CT6                      0       // color table bit 6
#define VDP_R4                          4       // pattern generator table
#define VDP_R4_PG13                     2       // pattern generator table bit 13
#define VDP_R4_PG12                     1       // pattern generator table bit 12
#define VDP_R4_PG11                     0       // pattern generator table bit 11
#define VDP_R5                          5       // sprite attribute table
#define VDP_R5_SA13                     6       // sprite attribute table bit 13
#define VDP_R5_SA12                     5       // sprite attribute table bit 12
#define VDP_R5_SA11                     4       // sprite attribute table bit 11
#define VDP_R5_SA10                     3       // sprite attribute table bit 10
#define VDP_R5_SA9                      2       // sprite attribute table bit 9
#define VDP_R5_SA8                      1       // sprite attribute table bit 8
#define VDP_R5_SA7                      0       // sprite attribute table bit 7
#define VDP_R6                          6       // sprite generator table
#define VDP_R6_SG13                     2       // sprite generator table bit 13
#define VDP_R6_SG12                     1       // sprite generator table bit 12
#define VDP_R6_SG11                     0       // sprite generator table bit 11
#define VDP_R7                          7       // text color / back drop color
#define VDP_R7_TC3                      7       // text color bit 3
#define VDP_R7_TC2                      6       // text color bit 2
#define VDP_R7_TC1                      5       // text color bit 1
#define VDP_R7_TC0                      4       // text color bit 0
#define VDP_R7_BD3                      3       // text color bit 3
#define VDP_R7_BD2                      2       // text color bit 2
#define VDP_R7_BD1                      1       // text color bit 1
#define VDP_R7_BD0                      0       // text color bit 0

// 関数

// 変数
extern unsigned char videoRegister[];


// スプライト
//

// 定数
#define VDP_SPRITE_Y                    0x00
#define VDP_SPRITE_X                    0x01
#define VDP_SPRITE_PATTERN              0x02
#define VDP_SPRITE_COLOR                0x03
#define VDP_SPRITE_SIZE                 0x04
#define VDP_SPRITE_ENTRY                0x20

// 関数
extern void CClearSprite(void);

// 変数
extern unsigned char sprite[];


// パターンネーム
//

// 定数

// 関数
extern void CClearPatternName(unsigned char name);

// 変数
extern unsigned char patternName[];


// 色
//

// 定数
#define VDP_COLOR_TRANSPARENT           0x00
#define VDP_COLOR_BLACK                 0x01
#define VDP_COLOR_MEDIUM_GREEN          0x02
#define VDP_COLOR_LIGHT_GREEN           0x03
#define VDP_COLOR_DARK_BLUE             0x04
#define VDP_COLOR_LIGHT_BLUE            0x05
#define VDP_COLOR_DARK_RED              0x06
#define VDP_COLOR_CYAN                  0x07
#define VDP_COLOR_MEDIUM_RED            0x08
#define VDP_COLOR_LIGHT_RED             0x09
#define VDP_COLOR_DARK_YELLOW           0x0a
#define VDP_COLOR_LIGHT_YELLOW          0x0b
#define VDP_COLOR_DARK_GREEN            0x0c
#define VDP_COLOR_MAGENTA               0x0d
#define VDP_COLOR_GRAY                  0x0e
#define VDP_COLOR_WHITE                 0x0f

// 関数

// 変数


// VDP アクセス
//

// 定数

// 関数
extern void CDisableScreen(void);
extern void CEnableScreen(void);
extern void CFillVram(void *dst, unsigned char data, unsigned int size);
extern void CLoadVram(void *dst, void *src, unsigned int size);

// 変数


// キー入力
//

// 定数
#define INPUT_KEY_UP                    0x00
#define INPUT_KEY_DOWN                  0x01
#define INPUT_KEY_LEFT                  0x02
#define INPUT_KEY_RIGHT                 0x03
#define INPUT_BUTTON_SPACE              0x04
#define INPUT_BUTTON_SHIFT              0x05
#define INPUT_BUTTON_ESC                0x06
#define INPUT_BUTTON_STOP               0x07

// 変数

// 関数
extern bool CIsInputPush(unsigned char input);
extern bool CIsInputEdge(unsigned char input);
extern bool CIsInputRepeat(unsigned char input);


// サウンド　
//

// 定数

// 変数

// 関数
extern void CPlayBgm(const char *ca, const char *cb, const char *cc);
extern void CPlaySe(const char *se);
extern void CStopSound(void);
extern bool CIsPlayBgm(void);
extern bool CIsPlaySe(void);


// 乱数
//

// 定数

// 関数
extern unsigned char CGetRandom(void);

// 変数


// デバッグ
//

// 定数
enum DEBUG {
    DEBUG_0 = 0, 
    DEBUG_1, 
    DEBUG_2, 
    DEBUG_3, 
    DEBUG_4, 
    DEBUG_5, 
    DEBUG_6, 
    DEBUG_7, 
    DEBUG_SIZE, 
};

// 関数

// 変数
extern unsigned char debug[];


#endif /* _C_H_ */
