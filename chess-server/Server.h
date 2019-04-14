#pragma once
#include <SFML/Network.hpp>
#include "Player.h"
#include "GameInstance.h"
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
	void deletePlayer(Player*p);
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
	void removeMatch(int);
	Player* getPlayerMatch(Player*);
	sf::Thread* getThreadOf(Player*);
	std::list<Player*> getPlayersList();
	~Server();

};

