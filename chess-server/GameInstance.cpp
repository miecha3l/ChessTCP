#include "GameInstance.h"
extern std::map<GameInstance*, Board*> boardOf;
GameInstance::GameInstance(Player *w, Player *b, Board *board) {
	ID = rand() % 94924895 + 3412;
	firstTurn = true;
	gameBoard = board;
	white = w;
	black = b;
	gameBoard->createClassicSet("white");
	gameBoard->createClassicSet("black");
	for (auto p : gameBoard->white_pieces) p->findLegalMoves(*gameBoard);
	for (auto p : gameBoard->black_pieces) p->findLegalMoves(*gameBoard);
}

GameState GameInstance::updateGameInstance(int moveId) {
	if (firstTurn) firstTurn = false;
	if (gameBoard->getCurrentTurn() == "white") {
		for (auto p : gameBoard->white_pieces) {
			for (auto m : p->legalMoves) {
				if (m.id == moveId) {
					if (p->validateMove(m, *gameBoard)) p->pushMove(m, *gameBoard);
				}
			}
		}
	}
	else {
		for (auto p : gameBoard->black_pieces) {
			for (auto m : p->legalMoves) {
				if (m.id == moveId) {
					if (p->validateMove(m, *gameBoard)) p->pushMove(m, *gameBoard);
				}
			}
		}
	}

	if (gameBoard->getCurrentTurn() == "white") {
		gameBoard->setCurrentTurn("black");
		for (auto p : gameBoard->black_pieces) p->findLegalMoves(*gameBoard);
		BoardAnalisys::revalidateBlackMoves(*gameBoard);
	}
	else if (gameBoard->getCurrentTurn() == "black") {
		gameBoard->setCurrentTurn("white");
		for (auto p : gameBoard->white_pieces) p->findLegalMoves(*gameBoard);
		BoardAnalisys::revalidateWhiteMoves(*gameBoard);
	}

	return GameState(*gameBoard);
}

GameState GameInstance::getCurrentGameState()
{
	return GameState(*gameBoard);
}

GameInstance::~GameInstance() {
	delete boardOf[this];
	delete gameBoard;
}

bool GameInstance::hasPlayer(Player *p)
{
	return (p->getId() == white->getId() || p->getId() == black->getId());
}
