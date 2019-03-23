#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Chess.h"

Board gameBoard;
sf::IpAddress addr("127.0.0.1");

sf::TcpListener infoListener, whiteListener, blackListener;
sf::TcpSocket clientWhite, clientBlack, tempSocket;
bool isWhiteConnected = false, isBlackConnected = false;
int whitePort = 0, blackPort = 0;

void connectWhite() {
	blackListener.setBlocking(true);
	blackListener.listen(blackPort);
	blackListener.accept(clientBlack);
	isBlackConnected = true;
}

void connectBlack() {
	whiteListener.setBlocking(true);
	whiteListener.listen(whitePort);
	whiteListener.accept(clientWhite);
	isWhiteConnected = true;
}

void createClassicSet(std::string color);

int main() {
	srand(time(NULL));

	createClassicSet("white");
	createClassicSet("black");

	for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
	for (auto p : gameBoard.black_pieces) p->findLegalMoves(gameBoard);

	sf::Thread doWhite(connectWhite);
	sf::Thread doBlack(connectBlack);

	while (whitePort == 0 || blackPort == 0) {
		infoListener.listen(1111);
		infoListener.setBlocking(true);
		infoListener.accept(tempSocket);
		sf::Packet portInfo, response;
		tempSocket.receive(portInfo);

		if (whitePort == 0) {
			portInfo >> whitePort;
			std::cout << whitePort << std::endl;
			doWhite.launch();
		}
		else if (blackPort == 0) {
			portInfo >> blackPort;
			std::cout << blackPort << std::endl;
			
		}

		response << "OK";
		tempSocket.send(response);
	}


	if (isWhiteConnected && isBlackConnected) {
		std::cout << "connection resolved\n";

		while (true) {
			system("cls");
			gameBoard.showOperationalBoard();
			GameState gs(gameBoard);

			sf::Packet message;
			std::string msgContainer = gs.parseGameStateToString();
			std::cout << msgContainer;
			message << msgContainer;
			if (gameBoard.getCurrentTurn() == "black") {
				clientBlack.send(message);
			}
			else {
				clientWhite.send(message);
			}

			sf::Packet response;
			std::string container;
			if (gameBoard.getCurrentTurn() == "black") {
				clientBlack.receive(response);
			}
			else {
				clientWhite.receive(response);
			}
			response >> container;

			int moveId = std::stoi(container);
			if (gameBoard.getCurrentTurn() == "white") {
				for (auto p : gameBoard.white_pieces) {
					for (auto m : p->legalMoves) {
						if (m.id == moveId) p->pushMove(m, gameBoard);
					}
				}
			}
			else {
				for (auto p : gameBoard.black_pieces) {
					for (auto m : p->legalMoves) {
						if (m.id == moveId) p->pushMove(m, gameBoard);
					}
				}
			}

			for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
			for (auto p : gameBoard.black_pieces) p->findLegalMoves(gameBoard);
			BoardAnalisys::revalidateBlackMoves(gameBoard);
			BoardAnalisys::revalidateWhiteMoves(gameBoard);

			if (gameBoard.getCurrentTurn() == "white") gameBoard.setCurrentTurn("black");
			else if (gameBoard.getCurrentTurn() == "black") gameBoard.setCurrentTurn("white");
		}
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