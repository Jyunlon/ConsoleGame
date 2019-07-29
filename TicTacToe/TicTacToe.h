#pragma once

#define TIC_TAC_TOE_RUN 1
#define TIC_TAC_TOE_EXIT 0
#define TIC_TAC_TOE_INPUT_ERROR 1
#define TIC_TAC_TOE_INPUT_OK 0

extern const char TicTacToe_Chess_blank;
extern const char TicTacToe_Chess_circle;
extern const char TicTacToe_Chess_cross;

typedef struct TicTacToe_Game_t {
	int isRun;
	int round;
	//当前回合执子的玩家 O or X
	char currentPlayer;
	int inputX; int intputY;
	//棋盘
	char board[3][3];
}TicTacToe_Game;

void TicTacToe_PromptGameStart(const TicTacToe_Game *game);

void TicTacToe_PromptPlayerInputBegin(const TicTacToe_Game *game);

void TicTacToe_PromptPlayerInputAfter(const TicTacToe_Game *game);

void TicTacToe_InitChessboard(TicTacToe_Game *game);

void TicTacToe_DisplayChessboard(const TicTacToe_Game *game);

void TicTacToe_PutChessOnBoard(TicTacToe_Game *game);

void TicTacToe_WhoseTurnIsIt(TicTacToe_Game *game);

void TicTacToe_EnterNextTurn(TicTacToe_Game *game);

void TicTacToe_PlayerInput(TicTacToe_Game *game);

int TicTacToe_CheckPlayerInput(const TicTacToe_Game *game);

void TicTacToe_PlayerInputUntilCorrect(TicTacToe_Game *game);

int TicTacToe_JudgeGameState(const TicTacToe_Game *game);

void TicTacToe_GameStateSet(TicTacToe_Game *game, int state);

int TicTacToe_GameStateGet(const TicTacToe_Game *game);

void TicTacToe_GameLaunch(TicTacToe_Game *game);

void TicTacToe_Start(TicTacToe_Game *game);

void TicTacToe_Update(TicTacToe_Game *game);
