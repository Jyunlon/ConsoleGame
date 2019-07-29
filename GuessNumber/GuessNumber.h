#ifndef GUESS_NUMBER_H_
#define GUESS_NUMBER_H_

#define GENERATE_UPPER 100
#define GAME_STATE_RUN 1
#define GAME_STATE_EXIT 0

typedef struct GuessNumber_t {
	int isGameRun;
	int specifiedNumber;
	int inputNumber;
}GuessNumber;

int GenerateRandomNumber(int upper);

void GuessNumber_GenerateSpecifiedNumber(GuessNumber *game);

void GuessNumber_PlayerInput(GuessNumber *game);

void GuessNumber_GameStateSet(GuessNumber *game,int state);

int GuessNumber_GameStateGet(const GuessNumber *game);

void GuessNumber_GameStateUpdate(GuessNumber *game);

void GuessNumber_GameStart(GuessNumber *game);

void GuessNumber_GameMainloop(GuessNumber *game);

void GuessNumber_GameProgramLaunch(GuessNumber *game);

#endif
