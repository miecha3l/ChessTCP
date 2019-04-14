#pragma once
#include "SFML/Network.hpp"
#include "Chess.h"
#include "GameState.h"
#include "Server.h"
#include <thread>

class GameInstance {
private:
	int ID;
	bool firstTurn;
	Player *white;
	Player *black;
	Board gameBoard;

public:
	GameInstance();
	GameInstance(Player*, Player*);
	~GameInstance();
	bool hasPlayer(Player*);
	bool isAnyPlayerDisconnected();
	int getId();
	Player *getWhite();
	Player *getBlack();
	void disconnectPlayer(Player*);
	Board getBoard();
	GameState updateGameInstance(int);
	GameState getCurrentGameState();
};