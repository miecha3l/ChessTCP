#pragma once
#include "SFML/Network.hpp"
#include "SFML/Graphics.hpp"
#include "Queue.h"
#include "GameState.h"
#include <iostream>
#include <list>
#include "SoloGame.h"
class Client
{
private:
	static Client *obj;
	Client();

	Queue<std::string> requests;
	Queue<sf::Packet> responses;

	sf::Thread *send;
	sf::Thread *receive;
	sf::Thread *handle;

	std::string ip;
	int port;
	bool console;
	bool sounds;
	bool highlightLegals;
	sf::TcpSocket serverConnection;
	GameState gs;
	SoloGame game;
	std::string playerName;
	std::string matchName;
	std::string color;
	std::string gameOverReason;
	std::list<std::string> onlinePlayers;
	bool inGame = false;
	bool onlinePlayersUpToDate = true;
	bool matchReq = false;
	std::string requester;

	void sendRequest();
	void receiveResponse();
	void handleResponse();

	void readConfigFile(std::string);

public:
	enum Screen {
		Menu,
		Lobby,
		OnlineGame,
		OfflineGame
	};

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
	void setMatchReq(bool);
	void setMatchName(std::string);
	void setRequester(std::string);
	Screen getCurrentScreen();
	void setCurrentScreen(Client::Screen);
	void setColor(std::string);
	GameState getGameState();
	void setGameState(Board b);
	void setGameState(GameState);
	void setInGame(bool);
	void setServerConnection(bool);
	void setGameOverReason(std::string);
	void setOnlinePlayers(std::list<std::string>);
	void addOnlinePlayer(std::string);
	std::string getGameOverReason();
	void setName(std::string s);
	void setOnlinePlayersU2D(bool);
	void resetSoloGame();
	SoloGame *getSoloGameInstance();
	bool playSounds();
	void setSounds(bool);
	bool doHighlightLegals();
	void setHighlights(bool);
	void initSoloGame();
	void killInstance();
	void connect();
	bool showConsole();

private:
	Screen currentScreen = Screen::Menu;
};

