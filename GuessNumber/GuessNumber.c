#include "GuessNumber.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int GenerateRandomNumber(int upper) {
	srand((unsigned int)time(NULL));
	return rand() % upper;
}

void GuessNumber_GenerateSpecifiedNumber(GuessNumber *game) {
	game->specifiedNumber = GenerateRandomNumber(GENERATE_UPPER);
}

void GuessNumber_PlayerInput(GuessNumber *game) {
	printf("Please input your guess : \t");
	scanf("%d", &game->inputNumber);
}

void GuessNumber_GameStateSet(GuessNumber *game, int state) {
	game->isGameRun = state;
}

int GuessNumber_GameStateGet(const GuessNumber *game) {
	return game->isGameRun;
}

void GuessNumber_GameStateUpdate(GuessNumber *game) {
	if (game->inputNumber == game->specifiedNumber) {
		printf("Prompt:\tYou Win!\n");
		GuessNumber_GameStateSet(game,GAME_STATE_EXIT);
	}
	else if (game->inputNumber > game->specifiedNumber) {
		printf("Prompt:\tLarge!\n");
	}
	else {
		printf("Prompt:\tSmall!\n");
	}
}

void GuessNumber_GameStart(GuessNumber *game) {
	GuessNumber_GenerateSpecifiedNumber(game);
	GuessNumber_GameStateSet(game,GAME_STATE_RUN);
	printf("Welcome to play the \"Guess Number Game\"\n");
}

void GuessNumber_GameMainloop(GuessNumber *game) {
	while (GuessNumber_GameStateGet(game)) {
		GuessNumber_PlayerInput(game);
		GuessNumber_GameStateUpdate(game);
	}
}

void GuessNumber_GameProgramLaunch(GuessNumber *game) {
	GuessNumber_GameStart(game);
	GuessNumber_GameMainloop(game);
}
