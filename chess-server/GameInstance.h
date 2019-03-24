#pragma once
#include "SFML/Network.hpp"
#include "Chess.h"
#include "GameState.h"
#include <thread>

class GameInstance {
private:
	int ID;
	sf::TcpSocket clientWhite;
	sf::TcpSocket clientBlack;
	Board gameBoard;

public:
	GameInstance(int portWhite, int portBlack, Board b);
	void play();
};