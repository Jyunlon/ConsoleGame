#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
/*Chess*/
typedef struct GobangChess_t {
	char ch;
	unsigned short color;//WORD
}GobangChess;//CHAR_INFO

void GobangChessSet(GobangChess *chess,char ch,unsigned short color) {
	chess->ch = ch;	chess->color = color;
}

void GobangChessCopy(GobangChess *chess,const GobangChess *source) {
	chess->ch = source->ch; chess->color = source->color;
}

int GobangChessIsEqual(const GobangChess *ca, const GobangChess *cb) {
	if (ca->ch == cb->ch	&&	ca->color == cb->color)
		return 1;
	else
		return 0;
}
/*Chessboard*/
#define GOBANG_CHESSBOARD_DEFAULT_WIDTH 15
#define GOBANG_CHESSBOARD_DEFAULT_HEIGHT 15
typedef struct GobangChessboard_t {
	int width;
	int height;
	GobangChess **position;
}GobangChessboard;

void GobangChessboardCreate(GobangChessboard *chessboard,int width,int height) {
	chessboard->position = (GobangChess**)malloc(height * sizeof(GobangChess*));
	int i;
	for (i = 0; i < height;++i) {
		chessboard->position[i] = (GobangChess*)malloc(width * sizeof(GobangChess));
	}
	chessboard->width = width; chessboard->height = height;
}

void GobangChessboardDestroy(GobangChessboard *chessboard) {
	int i;
	for (i = 0; i < chessboard->height;++i) {
		free(chessboard->position[i]); chessboard->position[i] = NULL;
	}
	free(chessboard->position); chessboard->position = NULL;
}

void GobangPutChessOnBoard(GobangChessboard *chessboard, const GobangChess *chess, int x, int y) {
	GobangChessCopy(&chessboard->position[x][y], chess);
}

void GobangChessboardClear(GobangChessboard *chessboard, const GobangChess *chess) {
	int i, j;
	for (i = 0; i < chessboard->height; ++i) {
		for (j = 0; j < chessboard->width; ++j) {
			GobangChessCopy(&chessboard->position[i][j], chess);
		}
	}
}
/*Game*/
#define GOBANG_GAME_RUN 1
#define GOBANG_GAME_EXIT 0
#define GOBANG_INPUT_ERROR_OVERRANGE 1
#define GOBANG_INPUT_ERROR_HASEXIST 2
#define GOBANG_INPUT_OK 0
typedef struct GobangGame_t {
	int isGameRun;
	int round;
	GobangChess blankChess;//空白位置样式
	GobangChess preChess;//先手方棋子样式
	GobangChess postChess;//后手方棋子样式
	int inputX, inputY;
	GobangChessboard board;
}GobangGame;

void GobangGameSetState(GobangGame *game,int state) {
	game->isGameRun = state;
}

int GobangGameState(GobangGame *game) {
	return game->isGameRun;
}

void GobangGameRoundSet(GobangGame *game, int x) {
	game->round = x;
}

void GobangGameEnterNextRound(GobangGame *game) {
	game->round++;
}

void GobangGameSetChessboardBlankStyle(GobangGame *game,char ch,unsigned short color) {
	GobangChessSet(&game->blankChess, ch, color);
}

void GobangGameSetPreChessStyle(GobangGame *game,char ch,unsigned short color) {
	GobangChessSet(&game->preChess, ch, color);
}

void GobangGameSetPostChessStyle(GobangGame *game, char ch, unsigned short color) {
	GobangChessSet(&game->postChess, ch, color);
}

void GobangGameCreateChessboard(GobangGame *game,int width,int height) {
	GobangChessboardCreate(&game->board, width, height);
}
void GobangGameDestroyChessboard(GobangGame *game) {
	GobangChessboardDestroy(&game->board);
}
void GobangGameClearChessboard(GobangGame *game) {
	GobangChessboardClear(&game->board, &game->blankChess);
}

void GobangGameSetTextColor(GobangGame *game, unsigned short color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, color);
}

void GobangGameDrawChess(GobangGame *game,const GobangChess *chess) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, chess->color);
	printf("%c",chess->ch);
}

void GobangGameDrawChessboard(GobangGame *game,int width,int height) {
	int i, j;
	for (i = 0; i < game->board.height;++i) {
		for (j = 0; j < game->board.width;++j) {
			GobangGameDrawChess(&game, &game->board.position[i][j]);
		}
		printf("\n");
	}
}

void GobangGamePutInputChessOnBoard(GobangGame *game) {
	if (game->round % 2 == 1) {
		GobangChessCopy(&game->board.position[game->inputX - 1][game->inputY - 1], &game->preChess);
	}
	else {
		GobangChessCopy(&game->board.position[game->inputX - 1][game->inputY - 1], &game->postChess);
	}
}
//Some colored prompt text output
void GobangGamePromptGameStart(GobangGame *game) {
	GobangGameSetTextColor(game, FOREGROUND_GREEN);
	printf("Welcome to play the Gobang game!\n");
}
void GobangGamePromptRoundStart(GobangGame *game) {
	GobangGameSetTextColor(game, FOREGROUND_GREEN);
	printf("Round-%d======================\n", game->round);
	printf("Chessboard State:\n");
}
void GobangGamePromptPlayerInput(GobangGame *game) {
	GobangGameSetTextColor(game, FOREGROUND_GREEN);
	printf("PLAYER %d input :\t", (game->round % 2) + 1);
}
void GobangGamePromptPlayerInputOverRange(GobangGame *game) {
	GobangGameSetTextColor(game, FOREGROUND_RED);
	printf("Invalid input.Makesure your input in the chessboard range.\n");
}
void GobangGamePromptPlayerInputNotBlankPosition(GobangGame *game) {
	GobangGameSetTextColor(game, FOREGROUND_RED);
	printf("Invalid input.This position has allready a chess existed.\n");
}
void GobangGamePromptWin(GobangGame *game) {
	printf("You win!\n");
}

void GobangGameStart(GobangGame *game) {
	GobangGameSetChessboardBlankStyle(game, '#', FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	GobangGameSetPreChessStyle(game, '#', FOREGROUND_RED);
	GobangGameSetPostChessStyle(game, '#', FOREGROUND_GREEN);
	GobangGameCreateChessboard(game, GOBANG_CHESSBOARD_DEFAULT_WIDTH, GOBANG_CHESSBOARD_DEFAULT_HEIGHT);
	GobangGameClearChessboard(game);
	GobangGameSetState(game, GOBANG_GAME_RUN);
	GobangGamePromptGameStart(game);
	GobangGameRoundSet(game, 1);
}

int GobangGamePlayerInputWithJudge(GobangGame *game) {
	GobangGamePromptPlayerInput(game);
	scanf("%d,%d", &game->inputX,&game->inputY);

	if (game->inputX < 1 || game->inputX >= game->board.width
		|| game->inputY < 1 || game->inputY >= game->board.height) {
		return GOBANG_INPUT_ERROR_OVERRANGE;
	}
	else {
		if (GobangChessIsEqual(&game->board.position[game->inputX - 1][game->inputY - 1], &game->blankChess)) {
			return	GOBANG_INPUT_OK;
		}
		else {
			return GOBANG_INPUT_ERROR_HASEXIST;
		}
	}
}

void GobangGamePlayerInputUntilCorrect(GobangGame *game) {
	int curInputState = GobangGamePlayerInputWithJudge(game);
	while (curInputState != GOBANG_INPUT_OK) {
		if (curInputState==GOBANG_INPUT_ERROR_OVERRANGE) {
			GobangGamePromptPlayerInputOverRange(game);
		}
		if (curInputState==GOBANG_INPUT_ERROR_HASEXIST) {
			GobangGamePromptPlayerInputNotBlankPosition(game);
		}
		curInputState = GobangGamePlayerInputWithJudge(game);
	}
}

void GobangGameInput(GobangGame *game) {
	GobangGamePlayerInputUntilCorrect(game);
}
int GobangGameIsHorizontalLink(GobangGame *game,int ax,int ay,int bx,int by) {

}
int GobangGameIsVerticalLink(GobangGame *game,int ax,int ay,int bx,int by) {

}
int GobangGameIsMainDiagonalLink(GobangGame *game,int ax,int ay,int bx,int by) {

}
int GobangGameIsSubDiagonalLink(GobangGame *game, int ax, int ay, int bx, int by) {

}
int GobangGameIsWinJudge(GobangGame *game) {
	//从当前位置,判断以它为中心的,半径为5的矩形内部即可,不必判断整个棋盘
	
}

void GobangGameUpdate(GobangGame *game) {
	GobangGamePutInputChessOnBoard(game);
	GobangGameDrawChessboard(game, GOBANG_CHESSBOARD_DEFAULT_WIDTH, GOBANG_CHESSBOARD_DEFAULT_HEIGHT);
	if (GobangGameIsWinJudge(game)==1) {//有人获胜
		GobangGamePromptWin(game);
		GobangGameSetState(&game, GOBANG_GAME_EXIT);
	}
	else {
		GobangGameEnterNextRound(game);
	}
}

void GobangGameMainloop(GobangGame *game) {
	while (GobangGameState(game) == GOBANG_GAME_RUN) {
		GobangGameInput(game);
		GobangGameUpdate(game);
	}
}

void GobangGameLaunch(GobangGame *game) {
	GobangGameStart(game);
	GobangGameMainloop(game);
}

int main() {
	GobangGame game;
	GobangGameLaunch(&game);

	return 0;
}
