#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Chess.h"

Board gameBoard;

void createClassicSet(std::string color);

int main() {
	createClassicSet("white");
	createClassicSet("black");
	std::string state = "gameOn";
	std::string time1 = "4:23";
	std::string time2 = "4:20";

	for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
	for (auto p : gameBoard.black_pieces) p->findLegalMoves(gameBoard);

	GameState gs(gameBoard, state, time1, time2);
	gs.print();
	std::cout << gs.parseGameStateToString() << std::endl;

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