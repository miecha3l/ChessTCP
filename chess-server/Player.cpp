#include "Player.h"
#include "Server.h"

Queue<sf::Packet> inQueue;
Queue<Player*> playersToRemove;

Player::Player(sf::TcpSocket *socket, std::string login)
	: client(socket), login(login), id(rand() % 8999 + 1000), status(Status::Idle)
{
	for (auto p : Server::instance()->getPlayersList()) {
		while (p->getId() == id) id = rand() % 8999 + 1000;
	}
	client->setBlocking(true);
}

sf::TcpSocket *Player::getClient() {
	return client;
}

void Player::communicate() {
	while (status != Status::Disconnected) {
		sf::Packet packet;
		std::string container;
		if (client->receive(packet) == sf::Socket::Status::Done) {
			inQueue.push(packet);
		}
		else if (client->receive(packet) == sf::Socket::Status::Disconnected) {
			status = Status::Disconnected;
			playersToRemove.push(this);
		}
	}
}

std::string Player::getLogin() {
	return login;
}

int Player::getId()
{
	return id;
}

Player::Status Player::getPlayersStatus() {
	return status;
}

void Player::setPlayersStatus(Status s) {
	status = s;
}

Player::~Player()
{
	delete client;
}
