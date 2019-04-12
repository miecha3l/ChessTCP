#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <list>
#include "Queue.h"

class Player
{
private:
	sf::TcpSocket *client;
	int id;
	std::string login;

public:
	static enum Status {
		Idle,
		InLobby,
		InGame,
		Disconnected
	};

	Status getPlayersStatus();
	void setPlayersStatus(Status s);
	Player(sf::TcpSocket *socket, std::string login);
	std::string getLogin();
	int getId();
	sf::TcpSocket *getClient();
	void communicate();
	~Player();

private:
	Status status;
};

