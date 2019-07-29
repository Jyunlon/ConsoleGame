#include "TicTacToe.h"
#include <stdio.h>

const char TicTacToe_Chess_blank = ' ';
const char TicTacToe_Chess_circle = 'O';
const char TicTacToe_Chess_cross = 'X';

void TicTacToe_Start(TicTacToe_Game *game) {
	game->round = 1;
	game->currentPlayer = 'O';
	game->inputX = 0;
	game->intputY = 0;
	TicTacToe_InitChessboard(game);
	TicTacToe_PromptGameStart(game);
	TicTacToe_GameStateSet(game, TIC_TAC_TOE_RUN);
}

void TicTacToe_Update(TicTacToe_Game *game) {
	while (TicTacToe_GameStateGet(game)) {
		TicTacToe_WhoseTurnIsIt(game);
		TicTacToe_DisplayChessboard(game);

		TicTacToe_PromptPlayerInputBegin(game);
		TicTacToe_PlayerInputUntilCorrect(game);
		TicTacToe_PutChessOnBoard(game);
		TicTacToe_PromptPlayerInputAfter(game);
		if (TicTacToe_JudgeGameState(game) == 1) {
			printf("Player %c Win!\n",game->currentPlayer);
			TicTacToe_DisplayChessboard(game);
			TicTacToe_GameStateSet(game, TIC_TAC_TOE_EXIT);
		}
		else
			TicTacToe_EnterNextTurn(game);
	}
}

void TicTacToe_GameLaunch(TicTacToe_Game *game) {
	//Start
	TicTacToe_Start(game);
	//Update
	TicTacToe_Update(game);
}

void TicTacToe_GameStateSet(TicTacToe_Game *game, int state) {
	game->isRun = state;
}

int TicTacToe_GameStateGet(const TicTacToe_Game *game) {
	return game->isRun;
}

void TicTacToe_PromptGameStart(const TicTacToe_Game *game) {
	printf("Welcome to play \"Tic Tac Toe\" .\n");
}

void TicTacToe_PromptPlayerInputBegin(const TicTacToe_Game *game) {
	printf("Player %c input coordX,coordY:\t",game->currentPlayer);
}

void TicTacToe_PromptPlayerInputAfter(const TicTacToe_Game *game) {
	printf("Player %c put a chess on [%d,%d]\n", game->currentPlayer, game->inputX, game->intputY);
}

void TicTacToe_WhoseTurnIsIt(TicTacToe_Game *game) {
	if (game->round % 2 == 0)
		game->currentPlayer = 'O';
	else
		game->currentPlayer = 'X';
}

void TicTacToe_EnterNextTurn(TicTacToe_Game *game) {
	game->round++;
}

void TicTacToe_InitChessboard(TicTacToe_Game *game) {
	int i, j;
	for (i = 0; i < 3; ++i) {
		for (j = 0; j < 3; ++j) {
			game->board[i][j] = TicTacToe_Chess_blank;
		}
	}
}

void TicTacToe_DisplayChessboard(const TicTacToe_Game *game) {
	printf("Round %d=====================\n",game->round);
	printf("Current chessboard state:\n");
	int i, j;
	for (i = 0; i < 3; ++i) {
		printf("\t");
		for (j = 0; j < 3; ++j) {
			if (j == 2) {
				if (i != 2)
					printf("%c\n\t------\n", game->board[i][j]);
				else
					printf("%c\n", game->board[i][j]);
			}
			else
				printf("%c|", game->board[i][j]);
		}
	}
}

void TicTacToe_PlayerInput(TicTacToe_Game *game) {
	scanf("%d,%d", &game->inputX, &game->intputY);
}

int TicTacToe_CheckPlayerInput(const TicTacToe_Game *game) {
	if (game->inputX < 1 || game->inputX>3
		|| game->intputY < 1 || game->intputY>3) {
		printf("Over the border of chessboard,please input in [1,3].\n");
		return TIC_TAC_TOE_INPUT_ERROR;
	}
	if (game->board[game->inputX - 1][game->intputY - 1] != TicTacToe_Chess_blank) {
		printf("Position [%d,%d] is not blank.\n", game->inputX, game->intputY);
		return TIC_TAC_TOE_INPUT_ERROR;
	}
	return TIC_TAC_TOE_INPUT_OK;
}

void TicTacToe_PlayerInputUntilCorrect(TicTacToe_Game *game) {
	TicTacToe_PlayerInput(game);
	while (TicTacToe_CheckPlayerInput(game) !=TIC_TAC_TOE_INPUT_OK) {
		TicTacToe_PromptPlayerInputBegin(game);
		TicTacToe_PlayerInput(game);
	}
}

void TicTacToe_PutChessOnBoard(TicTacToe_Game *game) {
	game->board[game->inputX - 1][game->intputY - 1] = game->currentPlayer;
}

int TicTacToe_JudgeGameState(const TicTacToe_Game *game) {
	if (game->board[0][0] == game->board[1][0] && game->board[0][0] == game->board[2][0]
		&& game->board[0][0] != TicTacToe_Chess_blank) {
		return 1;
	}
	if (game->board[0][1] == game->board[1][1] && game->board[0][1] == game->board[2][1]
		&& game->board[0][1] != TicTacToe_Chess_blank) {
		return 1;
	}
	if (game->board[0][2] == game->board[1][2] && game->board[0][2] == game->board[2][2]
		&& game->board[0][2] != TicTacToe_Chess_blank) {
		return 1;
	}
	if (game->board[0][0] == game->board[0][1] && game->board[0][1] == game->board[0][2]
		&& game->board[0][0] != TicTacToe_Chess_blank) {
		return 1;
	}
	if (game->board[1][0] == game->board[1][1] && game->board[1][1] == game->board[1][2]
		&& game->board[1][0] != TicTacToe_Chess_blank) {
		return 1;
	}
	if (game->board[2][0] == game->board[2][1] && game->board[2][1] == game->board[2][2]
		&& game->board[2][0] != TicTacToe_Chess_blank) {
		return 1;
	}
	if (game->board[0][0] == game->board[1][1] && game->board[1][1] == game->board[2][2]
		&& game->board[0][0] != TicTacToe_Chess_blank) {
		return 1;
	}
	if (game->board[2][0] == game->board[1][1] && game->board[1][1] == game->board[0][2]
		&& game->board[2][0] != TicTacToe_Chess_blank) {
		return 1;
	}
	return 0;
}
