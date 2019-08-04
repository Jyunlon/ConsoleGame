#pragma once

#define GOBANG_BOOL int
#define GOBANG_TRUE 1
#define GOBANG_FALSE 0

#define GOBANG_CHESS int
#define GOBANG_CHESS_BLANK 0
#define GOBANG_CHESS_BLACK 1
#define GOBANG_CHESS_WHITE 2

#define GOBANG_CHESSBOARD_WIDTH 15
#define GOBANG_CHESSBOARD_HEIGHT 15

typedef struct GobangChessboard_t {
	int width;
	int height;
	GOBANG_CHESS **location;
}GobangChessboard;

void GobangChessboard_Create(GobangChessboard *board,int width,int height);
void GobangChessboard_Destroy(GobangChessboard *board);
void GobangChessboard_SetEmpty(GobangChessboard *board);

typedef struct GobangGame_t {
	GOBANG_BOOL isGameRun;
	int round;
	int inputX; int inputY;//实际上是行,列,而并非屏幕上的(x,y),对应屏幕应该是(y,x)
	GobangChessboard chessboard;
}GobangGame;

void GobangGame_Launch(GobangGame *game);

void GobangGame_Start(GobangGame *game);

void GobangGame_Mainloop(GobangGame *game);

void GobangGame_End(GobangGame *game);

void GobangGame_Input(GobangGame *game);

void GobangGame_Update(GobangGame *game);

void GobangGame_PutChessOnBoard(GobangGame *game);

void GobangGame_SetBrushColor(unsigned short color);
void GobangGame_RenderChess(GOBANG_CHESS type);
void GobangGame_RenderChessboard(GobangGame *game);

GOBANG_BOOL GobangGame_IsSomeoneWon(GobangGame *game);
GOBANG_BOOL GobangGame_IsHorizontalLink(GobangGame *game);
GOBANG_BOOL GobangGame_IsVerticalLink(GobangGame *game);
GOBANG_BOOL GobangGame_IsMainDiagonalLink(GobangGame *game);
GOBANG_BOOL GobangGame_IsSubDiagonalLink(GobangGame *game);
