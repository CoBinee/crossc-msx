// Map.h : マップ
//
#ifndef _MAP_H_
#define _MAP_H_


// 参照ファイル
//
#include "Const.h"


// 定数宣言
//

// 迷路
#define MAP_MAZE_NULL                   0x00
#define MAP_MAZE_UP_BIT                 0x00
#define MAP_MAZE_UP                     (1 << MAP_MAZE_UP_BIT)
#define MAP_MAZE_DOWN_BIT               0x01
#define MAP_MAZE_DOWN                   (1 << MAP_MAZE_DOWN_BIT)
#define MAP_MAZE_LEFT_BIT               0x02
#define MAP_MAZE_LEFT                   (1 << MAP_MAZE_LEFT_BIT)
#define MAP_MAZE_RIGHT_BIT              0x03
#define MAP_MAZE_RIGHT                  (1 << MAP_MAZE_RIGHT_BIT)
#define MAP_MAZE_SIZE_X                 0x08
#define MAP_MAZE_SIZE_Y                 0x06

//　経路
#define MAP_ROUTE_NULL                  0x00
#define MAP_ROUTE_UP_BIT                0x00
#define MAP_ROUTE_UP                    (1 << MAP_ROUTE_UP_BIT)
#define MAP_ROUTE_DOWN_BIT              0x01
#define MAP_ROUTE_DOWN                  (1 << MAP_ROUTE_DOWN_BIT)
#define MAP_ROUTE_LEFT_BIT              0x02
#define MAP_ROUTE_LEFT                  (1 << MAP_ROUTE_LEFT_BIT)
#define MAP_ROUTE_RIGHT_BIT             0x03
#define MAP_ROUTE_RIGHT                 (1 << MAP_ROUTE_RIGHT_BIT)
#define MAP_ROUTE_SIZE_X                (MAP_MAZE_SIZE_X * 2 - 1)
#define MAP_ROUTE_SIZE_Y                (MAP_MAZE_SIZE_Y * 2 - 1)
#define MAP_ROUTE_SIZE_PIXEL            0x10
#define MAP_ROUTE_VIEW_X                0x01
#define MAP_ROUTE_VIEW_Y                0x01
#define MAP_ROUTE_VIEW_PIXEL            0x08


// 型宣言
//

// マップ
struct Map {

    // 迷路
    unsigned char mazes[MAP_MAZE_SIZE_Y][MAP_MAZE_SIZE_X];
    unsigned char mazeStacks[MAP_MAZE_SIZE_X * MAP_MAZE_SIZE_Y];
    unsigned char mazeStackCount;

    // 経路
    unsigned char routes[MAP_ROUTE_SIZE_Y][MAP_ROUTE_SIZE_X];

};


// 外部関数宣言
//
extern void MapInitialize(void);
extern void MapUpdate(void);
extern void MapRender(void);
extern void MapGetRandomPositions(unsigned char *positions);
extern unsigned char MapGetRandomDirection(unsigned char *positions);
extern unsigned char MapGetBranchCount(unsigned char *positions);
extern unsigned char MapMoveToBranch(unsigned char *positions, unsigned char direction, unsigned char distance);
extern unsigned char MapTurn(unsigned char *positions, unsigned char direction);
extern bool MapIsBranch(unsigned char *positions);
extern bool MapIsTurn(unsigned char *positions, unsigned char direction);


// 外部変数宣言
//


#endif /* _MAP_H_ */
