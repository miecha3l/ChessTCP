#pragma once
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "Queue.h"
#include "GameState.h"
#include <iostream>
#include <list>
class Client
{
private:
	static Client *obj;
	Client();

	Queue<std::string> requests;
	Queue<sf::Packet> responses;

	std::string ip;
	int port;
	sf::TcpSocket serverConnection;
	GameState gs;
	std::string playerName;
	std::string matchName;
	std::string color;
	std::string gameOverReason;
	std::string initialGs;
	std::list<std::string> onlinePlayers;
	bool inGame = false;
	bool gsUpToDate = false;
	bool onlinePlayersUpToDate = true;
	bool matchReq = false;
	std::string requester;
	bool matchAcc = false;

	void sendRequest();
	void receiveResponse();
	void handleResponse();

public:
	void init();
	static Client *instance();
	void addReqToQueue(std::string);
	std::string getColor();
	std::string getName();
	std::string getMatchName();
	bool isInGame();
	std::list<std::string> getOnlinePlayersList();
	void setOnlinePlayersListUpToDate(bool);
	bool isPlayersListUpToDate();
	bool isMatchReqPending();
	std::string getRequester();
	void setMatchAcc(bool);
	void setMatchReq(bool);
	void setMatchName(std::string);
	void setRequester(std::string);
	GameState getGameState();
};

