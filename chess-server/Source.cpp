#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Chess.h"

Board gameBoard;

void createClassicSet(std::string color);

int main() {
	createClassicSet("white");
	createClassicSet("black");

	for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
	for (auto p : gameBoard.black_pieces) p->findLegalMoves(gameBoard);

	sf::TcpListener clientListener;
	sf::TcpSocket clientWhite, clientBlack;
	clientListener.listen(1234);
	clientListener.accept(clientWhite);

	clientListener.listen(4321);
	clientListener.accept(clientBlack);
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