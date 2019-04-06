#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Chess.h"
#include "GameInstance.h"
void createClassicSet(std::string color, Board &b);
void resolveClientConnection();
std::vector<Board*> boards;
std::vector<GameInstance*> games;
std::vector<sf::Thread*> threads;
bool isWhiteConnected = false;
bool isBlackConnected = false;
int serverPort = 1111;
int portA, portB;


int main() {
	srand(time(NULL));
	
	while (true) {
		sf::Thread resolveConnections(&resolveClientConnection);
		system("cls");
		std::cout << "----------------------------NEXT GAME----------------------------------" << std::endl;
		std::cout << "Games: " << games.size() << std::endl;
		std::cout << "-----------------------------------------------------------------------" << std::endl;
		isWhiteConnected = false;

		boards.push_back(new Board());
		createClassicSet("white", *boards[boards.size() - 1]);
		createClassicSet("black", *boards[boards.size() - 1]);
		for (auto p : boards[boards.size() - 1]->white_pieces) p->findLegalMoves(*boards[boards.size() - 1]);
		
		portA = rand() % 9999 + 1000;
		portB = rand() % 9999 + 1000;
		resolveConnections.launch();
		std::cout << "Waiting for players..." << std::endl;

		GameInstance * gameInstance = new GameInstance(portA, portB, *boards[boards.size() - 1]);
		games.push_back(gameInstance);
		std::cout << "connection resolved\n";
		
		sf::Thread *gameThread = new sf::Thread(&GameInstance::play, gameInstance);
		threads.push_back(gameThread);
		gameThread->launch();
		std::cout << "Game thread launched" << std::endl;
		resolveConnections.terminate();
	}

	system("pause");
	return 0;
}

void createClassicSet(std::string color, Board &b) {
	if (color == "white") {
		for (int i = 0; i < 8; ++i) {
			b.addPiece((new Pawn(i, 6, "white")));
		}
		b.addPiece(new Rook(0, 7, "white"));
		b.addPiece(new Rook(7, 7, "white"));
		b.addPiece(new Knight(1, 7, "white"));
		b.addPiece(new Knight(6, 7, "white"));
		b.addPiece(new Bishop(2, 7, "white"));
		b.addPiece(new Bishop(5, 7, "white"));
		b.addPiece(new Queen(3, 7, "white"));
		b.addPiece(new King(4, 7, "white"));
	}
	else if (color == "black") {
		for (int i = 0; i < 8; ++i) {
			b.addPiece((new Pawn(i, 1, "black")));
		}
		b.addPiece(new Rook(0, 0, "black"));
		b.addPiece(new Rook(7, 0, "black"));
		b.addPiece(new Knight(1, 0, "black"));
		b.addPiece(new Knight(6, 0, "black"));
		b.addPiece(new Bishop(2, 0, "black"));
		b.addPiece(new Bishop(5, 0, "black"));
		b.addPiece(new Queen(3, 0, "black"));
		b.addPiece(new King(4, 0, "black"));
	}
	else {
		std::cout << "Invalid color";
	}
}
void resolveClientConnection() {
	while (!isWhiteConnected || !isBlackConnected) {
		sf::TcpListener listener;
		sf::TcpSocket temp;

		listener.listen(serverPort);
		listener.accept(temp);
		std::string freePort = isWhiteConnected ? std::to_string(portB) : std::to_string(portA);
		std::string color = isWhiteConnected ? "black" : "white";
		if (!isWhiteConnected) isWhiteConnected = true;
		else isBlackConnected = true;
		sf::Packet msg;
		msg << freePort << color;

		temp.send(msg);
	}
}