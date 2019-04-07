#pragma once
#include "SFML/Network.hpp"
#include "Chess.h"
#include "GameState.h"
#include "Server.h"
#include <thread>

class GameInstance {
private:
	int ID;
	Player *white;
	Player *black;
	Board *gameBoard;

public:
	GameInstance(Player*, Player*, Board*);
	~GameInstance();
	void play();
};