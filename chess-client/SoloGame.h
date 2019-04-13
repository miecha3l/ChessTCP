#pragma once
#include "ChessBot.h"
class SoloGame
{
private:
	Board gameBoard;
	ChessBot bot;
	std::string botsColor;
	bool checks;
	bool firstTurn;
	bool initialized;
	std::string turn;
	std::string gameOver;
	
public:
	SoloGame();
	SoloGame(std::string);
	void getPlayersMove(int moveId);
	void botMove();
	bool turnChecks();
	bool turnChecksDone();
	bool isFirstTurn();
	bool isInitialized();
	std::string getCurrentTurn();
};

