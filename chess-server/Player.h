#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <list>
struct Queue {
	std::list<sf::Packet> message;

	sf::Packet pop() {
		if (message.size() > 0) {
			sf::Packet toReturn = message.back();
			message.pop_back();
			return toReturn;
		}
		else return sf::Packet();
	}

	void push(sf::Packet packet) {
		message.push_back(packet);
	}

	int size() {
		return message.size();
	}
};

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

