#include "Player.h"
#include "Server.h"

Queue communicationQueue;

Player::Player(sf::TcpSocket *socket, std::string login)
	: client(socket), login(login), id(rand() % 9999 + 1), status(Status::Idle)
{
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
			communicationQueue.push(packet);
		}
		else if (client->receive(packet) == sf::Socket::Status::Disconnected) {
			status = Status::Disconnected;
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
	std::cout << "Destroying player" << std::endl;
	delete client;
}
