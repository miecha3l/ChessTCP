#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Chess.h"

Board gameBoard;
bool clientDisconnected = false;
void createClassicSet(std::string color);

int main() {
	

	while (true) {
		system("cls");
		clientDisconnected = false;

		gameBoard = Board();
		createClassicSet("white");
		createClassicSet("black");

		for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
		for (auto p : gameBoard.black_pieces) p->findLegalMoves(gameBoard);

		sf::TcpListener clientListener;
		sf::TcpSocket clientWhite, clientBlack;
		std::cout << "Waiting for players..." << std::endl;

		clientListener.listen(1234);
		clientListener.accept(clientWhite);
		std::cout << "White connected" << std::endl;

		clientListener.listen(4321);
		clientListener.accept(clientBlack);
		std::cout << "Black connected" << std::endl;


		std::cout << "connection resolved\n";

		while (!clientDisconnected) {
			GameState gs(gameBoard);

			sf::Packet message;
			message << gs.parseGameStateToString();
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

			try {
				int moveId = std::stoi(container);
				if (gameBoard.getCurrentTurn() == "white") {
					for (auto p : gameBoard.white_pieces) {
						for (auto m : p->legalMoves) {
							if (m.id == moveId) {
								if (p->validateMove(m, gameBoard)) p->pushMove(m, gameBoard);
							}
						}
					}
				}
				else {
					for (auto p : gameBoard.black_pieces) {
						for (auto m : p->legalMoves) {
							if (m.id == moveId) {
								if (p->validateMove(m, gameBoard)) p->pushMove(m, gameBoard);
							}
						}
					}
				}

				

				if (gameBoard.getCurrentTurn() == "white") {
					gameBoard.setCurrentTurn("black");
					for (auto p : gameBoard.black_pieces) p->findLegalMoves(gameBoard);
					BoardAnalisys::revalidateBlackMoves(gameBoard);

					gs = GameState(gameBoard);
					sf::Packet message;
					message << gs.parseGameStateToString();
					clientWhite.send(message);
				}
				else if (gameBoard.getCurrentTurn() == "black") {
					gameBoard.setCurrentTurn("white");
					for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
					BoardAnalisys::revalidateWhiteMoves(gameBoard);

					gs = GameState(gameBoard);
					sf::Packet message;
					message << gs.parseGameStateToString();
					clientBlack.send(message);
				}


			}
			catch (std::invalid_argument &e) {
				std::cout << e.what() << std::endl;
				clientDisconnected = true;
			}
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