#include "GameState.h"

GameState::GameState()
{
}

GameState::GameState(Board gameBoard, std::string state, std::string whiteTime, std::string blackTime) {
	whiteTimeLeft = whiteTime;
	blackTimeLeft = blackTime;
	currentGameState = state;

	for (auto p : gameBoard.white_pieces) {
		whitePieces.push_back(CompressedPiece(p->getName(), p->getBoardPosition(), p->legalMoves));
	}
	for (auto p : gameBoard.black_pieces) {
		blackPieces.push_back(CompressedPiece(p->getName(), p->getBoardPosition(), p->legalMoves));
	}
}

GameState::GameState(std::string gameStateAsString) {
	std::string currentPart = "";
	std::string currentSubpart = "";
	currentGameState = "";
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
			for (auto cc : currentSubpart) {
				if (cc == ';') { semicolonCount++; continue; }
				switch (semicolonCount)
				{
				case 0:
					currentGameState.push_back(cc);
					break;
				case 1:
					whiteTimeLeft.push_back(cc);
					break;
				case 2:
					blackTimeLeft.push_back(cc);
					break;
				default:
					break;
				}
			}
			continue;
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

	std::cout << "Current game state: " << currentGameState << std::endl;
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
	gameStateAsString.append(currentGameState.append(";"));
	gameStateAsString.append(whiteTimeLeft.append(";"));
	gameStateAsString.append(blackTimeLeft.append(";"));
	gameStateAsString.append("]}");

	return gameStateAsString;
}

GameState::~GameState()
{
}
