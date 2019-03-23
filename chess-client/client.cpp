#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "Chess.h"
#include "GameState.h"
#include <iostream>

GameState gs;
std::string myColor = "";

struct Connection {
	std::string ipAddr;
	int port;
	Connection(std::string ip, int port) : ipAddr(ip), port(port){}
};

int main() {
	srand(time(NULL));


	sf::TcpSocket serverConnection;
	int port = rand() % 9999 + 1000;
	std::cout << port << std::endl;


	Connection gameServer("127.0.0.1", 1111);
	serverConnection.connect(gameServer.ipAddr, gameServer.port);

	sf::Packet communicationPort, response;
	communicationPort << port;
	serverConnection.send(communicationPort);

	std::string respContainer;
	serverConnection.receive(response);
	response >> respContainer;

	if (respContainer == "OK") {
		serverConnection.disconnect();
		gameServer.port = port;
		serverConnection.setBlocking(true);
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

			myColor = gs.getCurrentGameTurn();

			system("pause");
			system("cls");


			if (myColor == "white") {
				for (auto p : gs.getWhitePieces()) {
					p.print();
				}

				std::string response;
				std::cin >> response;
				sf::Packet message;
				message << response;
				serverConnection.send(message);
			}
			else if (myColor == "black") {
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
	}
	else {
		std::cout << "sth went wrong" << std::endl;
	}

	
	
	system("pause");
	return 0;
}