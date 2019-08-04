#include "Gobang.h"
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

void GobangChessboard_Create(GobangChessboard *board, int width, int height) {
	board->width = width; board->height = height;
	board->location = (GOBANG_CHESS**)malloc(board->height * sizeof(GOBANG_CHESS*));
	int i;
	for (i = 0; i < board->height; ++i) {
		board->location[i] = (GOBANG_CHESS*)malloc(board->width * sizeof(GOBANG_CHESS*));
	}
}

void GobangChessboard_Destroy(GobangChessboard *board) {
	int i;
	for (i = 0; i < board->height; ++i) {
		free(board->location[i]); board->location[i] = NULL;
	}
	free(board->location); board->location = NULL;
	board->width = 0; board->height = 0;
}

void GobangChessboard_SetEmpty(GobangChessboard *board) {
	int i, j;
	for (i = 0; i < board->height;++i) {
		for (j = 0; j < board->width;++j) {
			board->location[i][j] = GOBANG_CHESS_BLANK;
		}
	}
}

void GobangGame_Launch(GobangGame *game) {
	GobangGame_Start(game);
	GobangGame_Mainloop(game);
	GobangGame_End(game);
}

void GobangGame_Start(GobangGame *game) {
	GobangChessboard_Create(&game->chessboard, GOBANG_CHESSBOARD_WIDTH, GOBANG_CHESSBOARD_HEIGHT);
	GobangChessboard_SetEmpty(&game->chessboard);
	game->round = 1;
	GobangGame_SetBrushColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	printf("欢迎来玩五子棋\n");
	GobangGame_RenderChessboard(game);
	game->isGameRun = GOBANG_TRUE;
}

void GobangGame_Mainloop(GobangGame *game) {
	while (game->isGameRun) {
		GobangGame_Input(game);
		GobangGame_Update(game);
	}
}

void GobangGame_End(GobangGame *game) {
	GobangChessboard_SetEmpty(&game->chessboard);
}

void GobangGame_Input(GobangGame *game) {
	int turn = game->round % 2;
	if (turn == 0) { turn = 2; }
	if (turn==1) {
		GobangGame_SetBrushColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
	else {
		GobangGame_SetBrushColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	printf("第%d轮,玩家%d回合:\t", game->round, turn);
	scanf("%d,%d",&game->inputX,&game->inputY);
}

void GobangGame_Update(GobangGame *game) {
	GobangGame_PutChessOnBoard(game);
	if (GobangGame_IsSomeoneWon(game)) {
		int turn = game->round % 2;
		if (turn == 0) { turn = 2; }
		GobangGame_SetBrushColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		printf("第%d轮,玩家%d获得胜利!\n", game->round, turn);
		GobangGame_RenderChessboard(game);
		game->isGameRun = FALSE;
	}
	else {
		GobangGame_RenderChessboard(game);
		game->round++;
	}
}

void GobangGame_PutChessOnBoard(GobangGame *game) {
	int turn = game->round % 2;
	if (turn == 0) { turn = 2; }
	if (turn == 1) {
		game->chessboard.location[game->inputX - 1][game->inputY - 1] = GOBANG_CHESS_BLACK;
	}
	else {
		game->chessboard.location[game->inputX - 1][game->inputY - 1] = GOBANG_CHESS_WHITE;
	}
}

void GobangGame_SetBrushColor(unsigned short color) {
	HANDLE handle;
	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, color);
}
void GobangGame_RenderChess(GOBANG_CHESS type) {
	if (type == GOBANG_CHESS_BLACK) {
		GobangGame_SetBrushColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	}
	else if (type == GOBANG_CHESS_WHITE) {
		GobangGame_SetBrushColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}
	else {
		GobangGame_SetBrushColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
	}
	printf("#");
}

void GobangGame_RenderChessboard(GobangGame *game) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int i, j;
	for (i = 0; i < game->chessboard.height;++i) {
		for (j = 0; j < game->chessboard.width;++j) {
			GobangGame_RenderChess(game->chessboard.location[i][j]);
		}
		printf("\n");
	}
}

GOBANG_BOOL GobangGame_IsSomeoneWon(GobangGame *game) {
	if (GobangGame_IsHorizontalLink(game)) {
		return GOBANG_TRUE;
	}
	if (GobangGame_IsVerticalLink(game)) {
		return GOBANG_TRUE;
	}
	if (GobangGame_IsMainDiagonalLink(game)) {
		return GOBANG_TRUE;
	}
	if (GobangGame_IsSubDiagonalLink(game)) {
		return GOBANG_TRUE;
	}
	return GOBANG_FALSE;
}

GOBANG_BOOL GobangGame_IsHorizontalLink(GobangGame *game) {
	int startY, endY, X;//实际是行列,X对应的是行,故不变,Y对应的是列,故变
	startY = (game->inputY - 1) - 5;
	if (startY < 0) { startY = 0; }
	endY = (game->inputY - 1) + 5;
	if (endY > GOBANG_CHESSBOARD_WIDTH - 1) { endY = GOBANG_CHESSBOARD_WIDTH - 1; }
	X = game->inputX - 1;
	int scantimes = endY - startY - 5 + 1;
	int i;
	for (i = 0; i < scantimes; ++i) {
		if (game->chessboard.location[X][startY + i] != GOBANG_CHESS_BLANK
			&& game->chessboard.location[X][startY + i] == game->chessboard.location[X][startY + i + 1]
			&& game->chessboard.location[X][startY + i] == game->chessboard.location[X][startY + i + 2]
			&& game->chessboard.location[X][startY + i] == game->chessboard.location[X][startY + i + 3]
			&& game->chessboard.location[X][startY + i] == game->chessboard.location[X][startY + i + 4]) {
			return GOBANG_TRUE;
		}
	}
	return GOBANG_FALSE;
}
GOBANG_BOOL GobangGame_IsVerticalLink(GobangGame *game) {
	int startX, endX, Y;//实际是行列,X对应的是行,故变,Y对应的是列,故不变
	startX = (game->inputX - 1) - 5;
	if (startX < 0) { startX = 0; }
	endX = (game->inputX - 1) + 5;
	if (endX > GOBANG_CHESSBOARD_HEIGHT - 1) { endX = GOBANG_CHESSBOARD_HEIGHT - 1; }
	Y = game->inputY - 1;
	int scantimes = endX - startX - 5 + 1;
	int i;
	for (i = 0; i < scantimes; ++i) {
		if (game->chessboard.location[startX + i][Y] != GOBANG_CHESS_BLANK
			&& game->chessboard.location[startX + i][Y] == game->chessboard.location[startX + i + 1][Y]
			&& game->chessboard.location[startX + i][Y] == game->chessboard.location[startX + i + 2][Y]
			&& game->chessboard.location[startX + i][Y] == game->chessboard.location[startX + i + 3][Y]
			&& game->chessboard.location[startX + i][Y] == game->chessboard.location[startX + i + 4][Y]) {
			return GOBANG_TRUE;
		}
	}
	return GOBANG_FALSE;
}
GOBANG_BOOL GobangGame_IsMainDiagonalLink(GobangGame *game) {
	//列数增加,行数增加
	int startX, startY;
	int endX, endY;
	startX = (game->inputX - 1) - 5;
	if (startX < 0) { startX = 0; }
	startY = (game->inputY - 1) - 5;
	if (startY < 0) { startY = 0; }
	endX = (game->inputX - 1) + 5;
	if (endX > GOBANG_CHESSBOARD_HEIGHT - 1) { endX = GOBANG_CHESSBOARD_HEIGHT - 1; }
	endY = (game->inputY - 1) + 5;
	if (endY > GOBANG_CHESSBOARD_WIDTH - 1) { endY = GOBANG_CHESSBOARD_WIDTH - 1; }
	int scantimes = endX - startX - 5 + 1;
	int i;
	for (i = 0; i < scantimes; ++i) {
		if (game->chessboard.location[startX + i][startY + i] != GOBANG_CHESS_BLANK
			&& game->chessboard.location[startX + i][startY + i] == game->chessboard.location[startX + i + 1][startY + i + 1]
			&& game->chessboard.location[startX + i][startY + i] == game->chessboard.location[startX + i + 2][startY + i + 2]
			&& game->chessboard.location[startX + i][startY + i] == game->chessboard.location[startX + i + 3][startY + i + 3]
			&& game->chessboard.location[startX + i][startY + i] == game->chessboard.location[startX + i + 4][startY + i + 4]) {
			return GOBANG_TRUE;
		}
	}
	return GOBANG_FALSE;
}
GOBANG_BOOL GobangGame_IsSubDiagonalLink(GobangGame *game) {
	//列数增加,行数减少
	int startX, startY;
	int endX, endY;
	startX = (game->inputX - 1) - 5;
	if (startX < 0) { startX = 0; }
	startY = (game->inputY - 1) - 5;
	if (startY < 0) { startY = 0; }
	endX = (game->inputX - 1) + 5;
	if (endX > GOBANG_CHESSBOARD_HEIGHT - 1) { endX = GOBANG_CHESSBOARD_HEIGHT - 1; }
	endY = (game->inputY - 1) + 5;
	if (endY > GOBANG_CHESSBOARD_WIDTH - 1) { endY = GOBANG_CHESSBOARD_WIDTH - 1; }
	int scantimes = endY - startY - 5 + 1;
	int i;
	for (i = 0; i < scantimes; ++i) {
		if (game->chessboard.location[endX - i][startY + i] != GOBANG_CHESS_BLANK
			&& game->chessboard.location[endX - i][startY + i] == game->chessboard.location[endX - i - 1][startY + i + 1]
			&& game->chessboard.location[endX - i][startY + i] == game->chessboard.location[endX - i - 2][startY + i + 2]
			&& game->chessboard.location[endX - i][startY + i] == game->chessboard.location[endX - i - 3][startY + i + 3]
			&& game->chessboard.location[endX - i][startY + i] == game->chessboard.location[endX - i - 4][startY + i + 4]) {
			return GOBANG_TRUE;
		}
	}
	return GOBANG_FALSE;
}
