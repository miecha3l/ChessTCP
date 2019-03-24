#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Chess.h"
#include "GameInstance.h"
void createClassicSet(std::string color);
void resolveClientConnection();

Board gameBoard;
bool isWhiteConnected = false;
bool isBlackConnected = false;
int serverPort = 1111;
int portA, portB;
sf::Thread resolveConnections(&resolveClientConnection);

int main() {
	srand(time(NULL));

	while (true) {
		system("cls");
		isWhiteConnected = false;

		gameBoard = Board();
		createClassicSet("white");
		createClassicSet("black");
		for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
		
		portA = rand() % 9999 + 1000;
		portB = rand() % 9999 + 1000;
		resolveConnections.launch();
		std::cout << "Waiting for players..." << std::endl;

		GameInstance game(portA, portB, gameBoard);
		std::cout << "connection resolved\n";

		sf::Thread gameThread(&GameInstance::play, &game);
		gameThread.launch();
		std::cout << "Game thread launched" << std::endl;
	}

	system("pause");
	return 0;
}

void createClassicSet(std::string color) {
	if (color == "white") {
		for (int i = 0; i < 8; ++i) {
			gameBoard.addPiece((new Pawn(i, 6, "white")));
		}
		gameBoard.addPiece(new Rook(0, 7, "white"));
		gameBoard.addPiece(new Rook(7, 7, "white"));
		gameBoard.addPiece(new Knight(1, 7, "white"));
		gameBoard.addPiece(new Knight(6, 7, "white"));
		gameBoard.addPiece(new Bishop(2, 7, "white"));
		gameBoard.addPiece(new Bishop(5, 7, "white"));
		gameBoard.addPiece(new Queen(3, 7, "white"));
		gameBoard.addPiece(new King(4, 7, "white"));
	}
	else if (color == "black") {
		for (int i = 0; i < 8; ++i) {
			gameBoard.addPiece((new Pawn(i, 1, "black")));
		}
		gameBoard.addPiece(new Rook(0, 0, "black"));
		gameBoard.addPiece(new Rook(7, 0, "black"));
		gameBoard.addPiece(new Knight(1, 0, "black"));
		gameBoard.addPiece(new Knight(6, 0, "black"));
		gameBoard.addPiece(new Bishop(2, 0, "black"));
		gameBoard.addPiece(new Bishop(5, 0, "black"));
		gameBoard.addPiece(new Queen(3, 0, "black"));
		gameBoard.addPiece(new King(4, 0, "black"));
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