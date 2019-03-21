#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "Chess.h"
#include "GameState.h"
#include <iostream>

GameState gs;

struct Connection {
	std::string ipAddr;
	int port;
	Connection(std::string ip, int port) : ipAddr(ip), port(port){}
};

int main() {
	sf::TcpSocket serverConnection;
	std::string temp;
	int port = 0;
	std::cout << "Select color: "; std::cin >> temp;
	if (temp == "white") port = 1234;
	else port = 4321;

	Connection gameServer("127.0.0.1", port);
	serverConnection.connect(gameServer.ipAddr, gameServer.port);

	while (true) {
		system("cls");
		sf::Packet message;
		std::string container;
		serverConnection.receive(message);
		message >> container;


		std::cout << container << std::endl;
		system("pause");
		system("cls");

		GameState gs(container);
		gs.print();

		system("pause");
		system("cls");


		if (port == 1234) {
			for (auto p : gs.getWhitePieces()) {
				p.print();
			}

			std::string response;
			std::cin >> response;
			sf::Packet message;
			message << response;
			serverConnection.send(message);
		}
		else if (port == 4321) {
			for (auto p : gs.getBlackPieces()) {
				p.print();
			}

			std::string response;
			std::cin >> response;
			sf::Packet message;
			message << response;
			serverConnection.send(message);
		}	
	}
	
	system("pause");
	return 0;
}