#pragma once
#include "SFML/Network.hpp"
#include "Chess.h"
#include "GameState.h"
#include <thread>

class GameInstance {
private:
	int ID;
	sf::TcpSocket *clientWhite;
	sf::TcpSocket *clientBlack;
	Board *gameBoard;

public:
	GameInstance(sf::TcpSocket*, sf::TcpSocket*, Board*);
	void play();
};