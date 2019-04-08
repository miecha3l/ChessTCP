#include "GameInstance.h"
GameInstance::GameInstance(Player *w, Player *b) {
	ID = rand() % 94924895 + 3412;
	firstTurn = true;
	white = w;
	black = b;
	gameBoard.createClassicSet("white");
	gameBoard.createClassicSet("black");
	for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
	for (auto p : gameBoard.black_pieces) p->findLegalMoves(gameBoard);
}

GameState GameInstance::updateGameInstance(int moveId) {
	if (firstTurn) firstTurn = false;
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
	}
	else if (gameBoard.getCurrentTurn() == "black") {
		gameBoard.setCurrentTurn("white");
		for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
		BoardAnalisys::revalidateWhiteMoves(gameBoard);
	}

	return GameState(gameBoard);
}

GameState GameInstance::getCurrentGameState()
{
	return GameState(gameBoard);
}

GameInstance::~GameInstance() {
	std::cout << "destroying game instance" << std::endl;
	
}

bool GameInstance::hasPlayer(Player *p)
{
	return (p->getId() == white->getId() || p->getId() == black->getId());
}

bool GameInstance::isAnyPlayerDisconnected()
{
	return (white == NULL || black == NULL);
}

int GameInstance::getId()
{
	return ID;
}

Player * GameInstance::getWhite()
{
	return white;
}

Player * GameInstance::getBlack()
{
	return black;
}
