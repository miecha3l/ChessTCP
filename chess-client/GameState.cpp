#include "GameState.h"

GameState::GameState()
{
}

//GameState::GameState(Board gameBoard) {
//	whiteTimeLeft = gameBoard.getWhiteTime();
//	blackTimeLeft = gameBoard.getBlackTime();
//	currentGameTurn = gameBoard.getCurrentTurn();
//	currentFlag = gameBoard.getCurrentFlag();
//
//	for (auto p : gameBoard.white_pieces) {
//		whitePieces.push_back(CompressedPiece(p->getName(), p->getBoardPosition(), p->legalMoves));
//	}
//	for (auto p : gameBoard.black_pieces) {
//		blackPieces.push_back(CompressedPiece(p->getName(), p->getBoardPosition(), p->legalMoves));
//	}
//}

GameState::GameState(std::string gameStateAsString) {
	std::string currentPart = "";
	std::string currentSubpart = "";
	currentGameTurn = "";
	currentFlag = "";
	whiteTimeLeft = "";
	blackTimeLeft = "";

	bool inSubpart = false;
	for (auto c : gameStateAsString) {
		if (c != '{' && !inSubpart) {
			currentPart.push_back(c);
		}
		else if (c == '{' && !inSubpart) {
			inSubpart = true;
			continue;
		}
		if (inSubpart && c == '[') {
			currentSubpart.clear();
			continue;
		}
		if (inSubpart && c == '}') {
			currentPart.clear();
			inSubpart = false;
			continue;
		}
		if (inSubpart && c != ']') {
			currentSubpart.push_back(c);
		}
		else if (inSubpart && c == ']' && currentPart == "white") {
			whitePieces.push_back(CompressedPiece(currentSubpart));
			currentSubpart.clear();
			continue;
		} 
		else if (inSubpart && c == ']' && currentPart == "black") {
			blackPieces.push_back(CompressedPiece(currentSubpart));
			currentSubpart.clear();
			continue;
		}
		else if (inSubpart && c == ']' && currentPart == "state") {
			int semicolonCount = 0;
			std::string buff = "";
			for (auto cc : currentSubpart) {
				if (cc == ';') {
					semicolonCount++;
					if (semicolonCount == 1) currentGameTurn = buff;
					else if (semicolonCount == 2) currentFlag = buff;
					else if (semicolonCount == 3) whiteTimeLeft = buff; 
					else if (semicolonCount == 4) blackTimeLeft = buff;
					buff.clear();
					continue;
				}
				buff.push_back(cc);
			}
		}
	}
}

void GameState::print() {
	for (auto p : whitePieces) {
		p.print();
		std::cout << "\n\n";
	}
	std::cout << "===============================================\n";
	for (auto p : blackPieces) {
		p.print();
		std::cout << "\n\n";
	}
	std::cout << "===============================================\n";

	std::cout << "Current game turn: " << currentGameTurn << std::endl;
	std::cout << "Current game flag:" << currentFlag << std::endl;
	std::cout << "White time left: " << whiteTimeLeft << std::endl;
	std::cout << "Black time left: " << blackTimeLeft << std::endl;
}

std::string GameState::parseGameStateToString() {
	std::string gameStateAsString = "white{";
	for (auto p : whitePieces) {
		gameStateAsString.append("[");
		gameStateAsString.append(p.parseToString());
		gameStateAsString.append("]");
	}
	gameStateAsString.append("}black{");

	for (auto p : blackPieces) {
		gameStateAsString.append("[");
		gameStateAsString.append(p.parseToString());
		gameStateAsString.append("]");
	}
	gameStateAsString.append("}state{[");
	gameStateAsString.append(currentGameTurn);
	gameStateAsString.push_back(';');
	gameStateAsString.append(currentFlag);
	gameStateAsString.push_back(';');
	gameStateAsString.append(whiteTimeLeft);
	gameStateAsString.push_back(';');
	gameStateAsString.append(blackTimeLeft);
	gameStateAsString.push_back(';');
	gameStateAsString.append("]}");

	return gameStateAsString;
}

std::string GameState::getBlackTimeLeft() { return blackTimeLeft; }
void GameState::setBlackTimeLeft(std::string t) { blackTimeLeft = t; }
std::string GameState::getWhiteTimeLeft() { return whiteTimeLeft; }
void GameState::setWhiteTimeLeft(std::string t) { whiteTimeLeft = t; }
std::string GameState::getCurrentGameTurn() { return currentGameTurn; }
void GameState::setCurrentGameTurn(std::string s) { currentGameTurn = s; }
std::vector<CompressedPiece> GameState::getBlackPieces() { return blackPieces; }
std::vector<CompressedPiece> GameState::getWhitePieces() { return whitePieces; }
GameState::~GameState()
{
}
