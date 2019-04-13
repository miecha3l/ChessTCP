#include "SoloGame.h"
#include "Client.h"
SoloGame::SoloGame() { firstTurn = true; initialized = false; }
SoloGame::SoloGame(std::string playersColor)
{
	gameBoard = Board();
	bot = ChessBot();
	gameBoard.createClassicSet("white");
	gameBoard.createClassicSet("black");
	botsColor = playersColor == "white" ? "black" : "white";
	for (auto p : gameBoard.black_pieces) p->findLegalMoves(gameBoard);
	for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
	Client::instance()->setGameState(gameBoard);
	checks = false;
	initialized = true;
	turn = "white";
}

void SoloGame::getPlayersMove(int moveId) {
	if (firstTurn) firstTurn = false;
	if (gameBoard.getCurrentTurn() == "white") {
		for (auto p : gameBoard.white_pieces) {
			for (auto m : p->legalMoves) {
				if (m.id == moveId) {
					if (p->validateMove(m, gameBoard)) p->pushMove(m, gameBoard);
					turn = "black";
					gameBoard.setCurrentTurn(turn);
					Client::instance()->setGameState(gameBoard);
					checks = false;
				}
			}
		}
	}
	else {
		for (auto p : gameBoard.black_pieces) {
			for (auto m : p->legalMoves) {
				if (m.id == moveId) {
					if (p->validateMove(m, gameBoard)) p->pushMove(m, gameBoard);
					turn = "white";
					gameBoard.setCurrentTurn(turn);
					Client::instance()->setGameState(gameBoard);
					checks = false;
				}
			}
		}
	}
	
}

void SoloGame::botMove() {
	int moveId = bot.getMove(gameBoard, botsColor).id;
	getPlayersMove(moveId);
	gameBoard.setCurrentTurn(turn);
	Client::instance()->setGameState(gameBoard);
	checks = false;
}

void SoloGame::turnChecks()
{
	if (!checks) {
		if (turn == "white") {
			for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
			BoardAnalisys::revalidateWhiteMoves(gameBoard);
			if (BoardAnalisys::isWhiteInStalemate(gameBoard) || BoardAnalisys::isWhiteInCheckmate(gameBoard)) {
				gameOver = "white";
			}
		}
		else if (turn == "black") {
			for (auto p : gameBoard.black_pieces) p->findLegalMoves(gameBoard);
			BoardAnalisys::revalidateBlackMoves(gameBoard);
			if (BoardAnalisys::isBlackInStalemate(gameBoard) || BoardAnalisys::isBlackInCheckmate(gameBoard)) {
				gameOver = "black";
			}
		}
	}
	Client::instance()->setGameState(gameBoard);
	checks = true;
}

bool SoloGame::turnChecksDone() { return checks; }

bool SoloGame::isFirstTurn()
{
	return firstTurn;
}

bool SoloGame::isInitialized()
{
	return initialized;
}

std::string SoloGame::getCurrentTurn()
{
	return turn;
}
