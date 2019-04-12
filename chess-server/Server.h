#pragma once
#include <SFML/Network.hpp>
#include "Player.h"
#include <iostream>
#include <string>


class Server
{
private:
	Server();
	static Server *obj;
	bool exists;
	int port;
	void handleMessages();
	void acceptClients();
	void updatePlayersAndGamesList();
	void handleInput();
	void printASCII();
	void printPlayerList();
	void printPlayerListWithInfo();
	void printGameList();
	int messagesCount();

	sf::TcpSocket *client;
	sf::TcpListener listener;
	std::list<Player*> players;
	std::list<sf::Thread*> playerThreads;
	std::map<Player*, sf::Thread*> threadOf;
	std::map<Player*, Player*> matchOf;
	std::map<int, Player*> playerOf;
	

public:
	static Server *instance();
	void init();

	int playersCount();
	Player* getPlayer(int id);
	void matchPlayers(int, int);
	Player* getPlayerMatch(Player*);
	std::list<Player*> getPlayersList();
	~Server();
};

