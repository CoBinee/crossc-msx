// Map.c : マップ
//


// 参照ファイル
//
#include <string.h>
#include "C.h"
#include "App.h"
#include "Game.h"
#include "Map.h"

// 外部参照関数
//

// 外部参照変数
//


// 内部関数の宣言
//
static void MapBuild(void);

// 変数の定義
//

// マップ
static struct Map map;


// マップを初期化する
//
void MapInitialize(void)
{
    // マップの初期化
    memset(&map, 0x00, sizeof (struct Map));

    // マップの作成
    MapBuild();
}

// マップを更新する
//
void MapUpdate(void)
{
}

// マップを描画する
//
void MapRender(void)
{
}

// マップを作成する
//
static void MapBuild(void)
{
    // 迷路の初期化
    map.mazeStacks[0] = (CGetRandom() & 0x37) + 0x10;
    map.mazeStackCount = 1;

    // 迷路を掘る
    while (map.mazeStackCount > 0) {
        unsigned char m = map.mazeStacks[map.mazeStackCount - 1];
        unsigned char x = m & 0x0f;
        unsigned char y = (m >> 4) & 0x0f;
        unsigned char d = CGetRandom() % 0x03;
        unsigned char i = 0;
        while (i < 0x04 && !(
            (d == MAP_MAZE_UP_BIT && y > 0 && (map.mazes[y - 1][x] & 0x0f) == 0) ||
            (d == MAP_MAZE_DOWN_BIT && y < MAP_MAZE_SIZE_Y - 1 && (map.mazes[y + 1][x] & 0x0f) == 0) || 
            (d == MAP_MAZE_LEFT_BIT && x > 0 && (map.mazes[y][x - 1] & 0x0f) == 0) || 
            (d == MAP_MAZE_RIGHT_BIT && x < MAP_MAZE_SIZE_X - 1 && (map.mazes[y][x + 1] & 0x0f) == 0)
        )) {
            d = (d + 0x01) & 0x03;
            ++i;
        }
        if (i < 0x04) {
            map.mazes[y][x] |= (1 << d);
            if (d == MAP_MAZE_UP_BIT) {
                --y;
            } else if (d == MAP_MAZE_DOWN_BIT) {
                ++y;
            } else if (d == MAP_MAZE_LEFT_BIT) {
                --x;
            } else {
                ++x;
            }
            map.mazes[y][x] |= (1 << (d ^ 0x01));
            map.mazeStacks[map.mazeStackCount] = (y << 4) | x;
            ++map.mazeStackCount;
        } else {
            --map.mazeStackCount;
        }
    }

    // 行き止まりをなくす
    for (unsigned char y = 0; y < MAP_MAZE_SIZE_Y; y++) {
        for (unsigned char x = 0; x < MAP_MAZE_SIZE_X; x++) {
            unsigned char m = map.mazes[y][x];
            signed char d = -1;
            if (m == MAP_MAZE_UP) {
                d = MAP_MAZE_DOWN_BIT;
            } else if (m == MAP_MAZE_DOWN) {
                d = MAP_MAZE_UP_BIT;
            } else if (m == MAP_MAZE_LEFT) {
                d = MAP_MAZE_RIGHT_BIT;
            } else if (m == MAP_MAZE_RIGHT) {
                d = MAP_MAZE_LEFT_BIT;
            }
            {
                unsigned char b = 0x02;
                while (d >= 0) {
                    if (d == MAP_MAZE_UP_BIT && y > 0) {
                        map.mazes[y][x] |= MAP_MAZE_UP;
                        map.mazes[y - 1][x] |= MAP_MAZE_DOWN;
                        d = -1;
                    } else if (d == MAP_MAZE_DOWN_BIT && y < MAP_MAZE_SIZE_Y - 1) {
                        map.mazes[y][x] |= MAP_MAZE_DOWN;
                        map.mazes[y + 1][x] |= MAP_MAZE_UP;
                        d = -1;
                    } else if (d == MAP_MAZE_LEFT_BIT && x > 0) {
                        map.mazes[y][x] |= MAP_MAZE_LEFT;
                        map.mazes[y][x - 1] |= MAP_MAZE_RIGHT;
                        d = -1;
                    } else if (d == MAP_MAZE_RIGHT_BIT && x < MAP_MAZE_SIZE_X - 1) {
                        map.mazes[y][x] |= MAP_MAZE_RIGHT;
                        map.mazes[y][x + 1] |= MAP_MAZE_LEFT;
                        d = -1;
                    }
                    if (d >= 0) {
                        d = d ^ b;
                        if (b > 0x01) {
                            b = b >> 1;
                        }
                    }
                }
            }
        }
    }

    // 経路の作成
    for (unsigned char y = 0; y < MAP_MAZE_SIZE_Y; y++) {
        for (unsigned char x = 0; x < MAP_MAZE_SIZE_X; x++) {
            map.routes[y * 2][x * 2] = map.mazes[y][x];
            if (x < MAP_MAZE_SIZE_X - 1 && (map.mazes[y][x] & MAP_MAZE_RIGHT) != 0) {
                map.routes[y * 2][x * 2 + 1] = (MAP_ROUTE_LEFT | MAP_ROUTE_RIGHT);
            }
            if (y < MAP_MAZE_SIZE_Y - 1 && (map.mazes[y][x] & MAP_MAZE_DOWN) != 0) {
                map.routes[y * 2 + 1][x * 2] = (MAP_ROUTE_UP | MAP_ROUTE_DOWN);
            }
        }
    }

    // 経路の描画
    {
        unsigned char *pn = &patternName[MAP_ROUTE_VIEW_Y * 0x20 + MAP_ROUTE_VIEW_X];
        for (unsigned char y = 0; y < MAP_ROUTE_SIZE_Y; y++) {
            for (unsigned char x = 0; x < MAP_ROUTE_SIZE_X; x++) {
                static const unsigned char patterns[][4] = {
                    {0x40, 0x41, 0x50, 0x51, }, 
                    {0x42, 0x43, 0x52, 0x53, }, 
                    {0x44, 0x45, 0x54, 0x55, }, 
                    {0x46, 0x47, 0x56, 0x57, }, 
                    {0x48, 0x49, 0x58, 0x59, }, 
                    {0x4a, 0x4b, 0x5a, 0x5b, }, 
                    {0x4c, 0x4d, 0x5c, 0x5d, }, 
                    {0x4e, 0x4f, 0x5e, 0x5f, }, 
                    {0x60, 0x61, 0x70, 0x71, }, 
                    {0x62, 0x63, 0x72, 0x73, }, 
                    {0x64, 0x65, 0x74, 0x75, }, 
                    {0x66, 0x67, 0x76, 0x77, }, 
                    {0x68, 0x69, 0x78, 0x79, }, 
                    {0x6a, 0x6b, 0x7a, 0x7b, }, 
                    {0x6c, 0x6d, 0x7c, 0x7d, }, 
                    {0x6e, 0x6f, 0x7e, 0x7f, }, 
                };
                const unsigned char *ps = patterns[map.routes[y][x]];
                *(pn + 0x00) = *ps++;
                *(pn + 0x01) = *ps++;
                *(pn + 0x20) = *ps++;
                *(pn + 0x21) = *ps;
                pn = pn + 0x02;
            }
            pn = pn + 0x22;
        }
    }
}

// ランダムな位置を取得する
//
void MapGetRandomPositions(unsigned char *positions)
{
    positions[_X_] = (CGetRandom() % MAP_MAZE_SIZE_X) * 0x02 * MAP_ROUTE_SIZE_PIXEL + MAP_ROUTE_SIZE_PIXEL / 2;
    positions[_Y_] = (CGetRandom() % MAP_MAZE_SIZE_Y) * 0x02 * MAP_ROUTE_SIZE_PIXEL + MAP_ROUTE_SIZE_PIXEL / 2;
}

// ランダムな方向を取得する
//
unsigned char MapGetRandomDirection(unsigned char *positions)
{
    unsigned char d = CGetRandom() & 0x03;
    unsigned char r = map.routes[positions[_Y_] / MAP_ROUTE_SIZE_PIXEL][positions[_X_] / MAP_ROUTE_SIZE_PIXEL];
    while ((r & (1 << d)) == 0) {
        d = (d + 0x01) & 0x03;
    }
    return d;
}

// 分岐の数を取得する
//
unsigned char MapGetBranchCount(unsigned char *positions)
{
    static const unsigned char branches[] = {
        0x00, 0x01, 0x01, 0x02, 0x01, 0x02, 0x02, 0x03, 0x01, 0x02, 0x02, 0x03, 0x02, 0x03, 0x03, 0x04, 
    };
    return branches[map.routes[positions[_Y_] / MAP_ROUTE_SIZE_PIXEL][positions[_X_] / MAP_ROUTE_SIZE_PIXEL]];
}

// 分岐点まで移動する
//
unsigned char MapMoveToBranch(unsigned char *positions, unsigned char direction, unsigned char distance)
{
    if (direction == MAP_ROUTE_UP_BIT) {
        unsigned char d = 0x10 - ((0x08 - positions[_Y_]) & 0x0f);
        if (d < distance) {
            positions[_Y_] = positions[_Y_] - d;
            distance = distance - d;
        } else {
            positions[_Y_] = positions[_Y_] - distance;
            distance = 0;
        }
    } else if (direction == MAP_ROUTE_DOWN_BIT) {
        unsigned char d = 0x10 - ((positions[_Y_] - 0x08) & 0x0f);
        if (d < distance) {
            positions[_Y_] = positions[_Y_] + d;
            distance = distance - d;
        } else {
            positions[_Y_] = positions[_Y_] + distance;
            distance = 0;
        }
    } else if (direction == MAP_ROUTE_LEFT_BIT) {
        unsigned char d = 0x10 - ((0x08 - positions[_X_]) & 0x0f);
        if (d < distance) {
            positions[_X_] = positions[_X_] - d;
            distance = distance - d;
        } else {
            positions[_X_] = positions[_X_] - distance;
            distance = 0;
        }
    } else {
        unsigned char d = 0x10 - ((positions[_X_] - 0x08) & 0x0f);
        if (d < distance) {
            positions[_X_] = positions[_X_] + d;
            distance = distance - d;
        } else {
            positions[_X_] = positions[_X_] + distance;
            distance = 0;
        }
    }
    return distance;
}

// 分岐を曲がる
//
unsigned char MapTurn(unsigned char *positions, unsigned char direction)
{
    unsigned char r = map.routes[positions[_Y_] / MAP_ROUTE_SIZE_PIXEL][positions[_X_] / MAP_ROUTE_SIZE_PIXEL];
    if ((r & (1 << direction)) == 0) {
        direction = direction ^ 0x02;
        if ((r & (1 << direction)) == 0) {
            direction = direction ^ 0x01;
            if ((r & (1 << direction)) == 0) {
                direction = direction ^ 0x02;
            }
        }
    }
    return direction;
}

// 分岐点かどうかを判定する
//
bool MapIsBranch(unsigned char *positions)
{
    return (positions[_X_] & 0x0f) == 0x08 && (positions[_Y_] & 0x0f) == 0x08 ? true : false;
}

// その向きへ曲がれるかどうかを判定する
//
bool MapIsTurn(unsigned char *positions, unsigned char direction)
{
    bool result = false;
    if (MapIsBranch(positions)) {
        if ((map.routes[positions[_Y_] / MAP_ROUTE_SIZE_PIXEL][positions[_X_] / MAP_ROUTE_SIZE_PIXEL] & (1 << direction)) != 0) {
            result = true;
        }
    } else {
        if ((positions[_X_] & 0x0f) == 0x08) {
            if (direction == MAP_ROUTE_UP_BIT || direction == MAP_ROUTE_DOWN_BIT) {
                result =true;
            }
        } else {
            if (direction == MAP_ROUTE_LEFT_BIT || direction == MAP_ROUTE_RIGHT_BIT) {
                result =true;
            }
        }
    }
    return result;
}
