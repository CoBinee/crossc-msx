; C.s : C ライブラリ
;


; モジュール宣言
;
    .module C

; 参照ファイル
;
    .include    "bios.inc"
    .include    "vdp.inc"
    .include    "System.inc"

; 外部変数宣言
;

; マクロの定義
;


; CODE 領域
;
    .area   _CODE

; 割り込みを禁止する
;
_CDisableInterrupt::

    ; レジスタの保存

    ; 割り込みの禁止
    di

    ; レジスタの復帰

    ; 終了
    ret

; 割り込み禁止を解除する
;
_CEnableInterrupt::

    ; レジスタの保存

    ; 割り込みの禁止
    ei

    ; レジスタの復帰

    ; 終了
    ret

; スプライトをクリアする
;
_CClearSprite::

    ; レジスタの保存

    ; スプライトのクリア
    call    _SystemClearSprite

    ; レジスタの復帰

    ; 終了
    ret

; パターンネームをクリアする
;
_CClearPatternName::

    ; レジスタの保存

    ; unsigned char name < パターンネーム

    ; スプライトのクリア
    ld      hl, #0x0002
    add     hl, sp
    ld      a, (hl)
    call    _SystemClearPatternName

    ; レジスタの復帰

    ; 終了
    ret

; 画面表示を禁止する
;
_CDisableScreen::

    ; レジスタの保存

    ; 画面表示の禁止
    call    DISSCR

    ; レジスタの復帰

    ; 終了
    ret

; 画面を表示する
;
_CEnableScreen::

    ; レジスタの保存

    ; 画面表示の禁止
    call    ENASCR

    ; レジスタの復帰

    ; 終了
    ret

; VRAM をデータで埋める
;
_CFillVram::

    ; レジスタの保存
    
    ; void *dst          < VRAM アドレス
    ; unsigned char data < データ
    ; unsigned int size  < サイズ

    ; VRAM を埋める
    ld      iy, #0x0002
    add     iy, sp
    ld      l, 0x00(iy)
    ld      h, 0x01(iy)
    ld      a, 0x02(iy)
    ld      c, 0x03(iy)
    ld      b, 0x04(iy)
    call    FILVRM

    ; レジスタの復帰

    ; 終了
    ret

; VRAM へデータを転送する
;
_CLoadVram::

    ; レジスタの保存
    
    ; void *dst         < VRAM アドレス
    ; void *src         < データアドレス
    ; unsigned int size < サイズ

    ; VRAM への転送
    ld      iy, #0x0002
    add     iy, sp
    ld      e, 0x00(iy)
    ld      d, 0x01(iy)
    ld      l, 0x02(iy)
    ld      h, 0x03(iy)
    ld      c, 0x04(iy)
    ld      b, 0x05(iy)
    call    LDIRVM

    ; レジスタの復帰

    ; 終了
    ret

; キー入力を取得する
;
_CIsInputPush::

    ; レジスタの保存

    ; unsigned char input < キー
    ; char value          > 1 = キーが押された

    ; キーの取得
    ld      hl, #0x0002
    add     hl, sp
    ld      e, (hl)
    ld      d, #0x00
    ld      hl, #_input
    add     hl, de
    ld      a, (hl)
    or      a
    jr      z, 10$
    ld      a, #0x01
10$:
    ld      l, a

    ; レジスタの復帰

    ; 終了
    ret

_CIsInputEdge::

    ; レジスタの保存

    ; unsigned char input < キー
    ; char value          > 1 = キーが押された

    ; キーの取得
    ld      hl, #0x0002
    add     hl, sp
    ld      e, (hl)
    ld      d, #0x00
    ld      hl, #_input
    add     hl, de
    ld      a, (hl)
    cp      #0x01
    jr      z, 10$
    xor     a
10$:
    ld      l, a

    ; レジスタの復帰

    ; 終了
    ret

_CIsInputRepeat::

    ; レジスタの保存

    ; unsigned char input < キー
    ; char value          > 1 = キーが押された

    ; キーの取得
    ld      hl, #0x0002
    add     hl, sp
    ld      e, (hl)
    ld      d, #0x00
    ld      hl, #_input
    add     hl, de
    ld      a, (hl)
    cp      #0x01
    jr      z, 10$
    cp      #0x10
    jr      c, 11$
10$:
    ld      l, #0x01
    jr      19$
11$:
    ld      l, #0x00
;   jr      19$
19$:

    ; レジスタの復帰

    ; 終了
    ret

; BGM を再生する
;
_CPlayBgm::

    ; レジスタの保存

    ; unsigned char *ca < チャンネル A
    ; unsigned char *cb < チャンネル B
    ; unsigned char *cc < チャンネル C

    ; サウンドの再生
    ld      iy, #0x0002
    add     iy, sp
    ld      e, 0x00(iy)
    ld      d, 0x01(iy)
    ld      hl, (_soundChannel + SOUND_CHANNEL_A + SOUND_CHANNEL_HEAD)
    or      a
    sbc     hl, de
    jr      z, 19$
    ld      (_soundChannel + SOUND_CHANNEL_A + SOUND_CHANNEL_REQUEST), de
    ld      e, 0x02(iy)
    ld      d, 0x03(iy)
    ld      (_soundChannel + SOUND_CHANNEL_B + SOUND_CHANNEL_REQUEST), de
    ld      e, 0x04(iy)
    ld      d, 0x05(iy)
    ld      (_soundChannel + SOUND_CHANNEL_C + SOUND_CHANNEL_REQUEST), de
19$:

    ; レジスタの復帰

    ; 終了
    ret

; SE を再生する
;
_CPlaySe::

    ; レジスタの保存

    ; unsigned char *se < SE

    ; サウンドの再生
    ld      hl, #0x0002
    add     hl, sp
    ld      e, (hl)
    inc     hl
    ld      d, (hl)
    ld      (_soundChannel + SOUND_CHANNEL_D + SOUND_CHANNEL_REQUEST), de

    ; レジスタの復帰

    ; 終了
    ret

; サウンドを停止する
;
_CStopSound::

    ; レジスタの保存

    ; サウンドの停止
    call    _SystemStopSound

    ; レジスタの復帰

    ; 終了
    ret

; BGM が再生中かどうかを判定する
;
_CIsPlayBgm::

    ; レジスタの保存

    ; bool value > 0/1 = 停止/再生中

    ; サウンドの監視
    ld      hl, (_soundChannel + SOUND_CHANNEL_A + SOUND_CHANNEL_REQUEST)
    ld      a, h
    or      l
    jr      nz, 10$
    ld      hl, (_soundChannel + SOUND_CHANNEL_A + SOUND_CHANNEL_PLAY)
    ld      a, h
    or      l
    jr      nz, 10$
;   ld      l, #0x00
    jr      19$
10$:
    ld      l, #0x01
19$:

    ; レジスタの復帰

    ; 終了
    ret

; SE が再生中かどうかを判定する
;
_CIsPlaySe::

    ; レジスタの保存

    ; bool value > 0/1 = 停止/再生中

    ; サウンドの監視
    ld      hl, (_soundChannel + SOUND_CHANNEL_D + SOUND_CHANNEL_REQUEST)
    ld      a, h
    or      l
    jr      nz, 10$
    ld      hl, (_soundChannel + SOUND_CHANNEL_D + SOUND_CHANNEL_PLAY)
    ld      a, h
    or      l
    jr      nz, 10$
;   ld      l, #0x00
    jr      19$
10$:
    ld      l, #0x01
19$:

    ; レジスタの復帰

    ; 終了
    ret

; 乱数を取得する
;
_CGetRandom::

    ; レジスタの保存

    ; unsigned char value > 乱数

    ; 乱数の取得
    call    _SystemGetRandom
    ld      l, a

    ; レジスタの復帰

    ; 終了
    ret

; 定数の定義
;


; DATA 領域
;
    .area   _DATA

; 変数の定義
;

