#include "GameInstance.h"
GameInstance::GameInstance() {
	ID = rand() % 94924895 + 3412;
	firstTurn = true;
	white = NULL;
	black = NULL;
	gameBoard.createClassicSet("white");
	gameBoard.createClassicSet("black");
	for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
	for (auto p : gameBoard.black_pieces) p->findLegalMoves(gameBoard);
}

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
		if (BoardAnalisys::isWhiteInStalemate(gameBoard) || BoardAnalisys::isWhiteInCheckmate(gameBoard)) {
			gameBoard.setCurrentFlag("winner_black");
			winner = "black";
			gameFinished = true;
		}
	}
	else if (gameBoard.getCurrentTurn() == "black") {
		gameBoard.setCurrentTurn("white");
		for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
		BoardAnalisys::revalidateWhiteMoves(gameBoard);
		if (BoardAnalisys::isWhiteInStalemate(gameBoard) || BoardAnalisys::isWhiteInCheckmate(gameBoard)) {
			gameBoard.setCurrentFlag("winner_white");
			winner = "white";
			gameFinished = true;
		}
	}

	return GameState(gameBoard);
}

GameState GameInstance::getCurrentGameState()
{
	return GameState(gameBoard);
}

bool GameInstance::isGameFinished()
{
	return gameFinished;
}

void GameInstance::setGameFinished(bool s)
{
	gameFinished = true;
}

std::string GameInstance::getWinner()
{
	return winner;
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

Board GameInstance::getBoard()
{
	return gameBoard;
}

void GameInstance::disconnectPlayer(Player*p) {
	if (p == white) white = NULL;
	else if (p == black) black = NULL;
}
