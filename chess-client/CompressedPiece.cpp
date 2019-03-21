#include "CompressedPiece.h"

CompressedPiece::CompressedPiece(std::string pieceAsString) {
	int semicolonCount = 0;
	std::string posAsString = "";
	std::string posBuff = "";
	std::string idBuff = "";
	std::string nameBuff = "";
	for (auto c : pieceAsString) {
		if (c == ';') {
			semicolonCount++;
			if (posBuff.size() == 2) {
				legalMoves.push_back(CompressedMove(sf::Vector2i(posBuff[0] - '0', posBuff[1] - '0'), std::stoi(idBuff)));
				posBuff.clear();
				idBuff.clear();
			}
			continue;
		}
		if (semicolonCount == 0) {
			nameBuff.push_back(c);
		}
		if (semicolonCount == 1) {
			posAsString.push_back(c);
		}
		if (semicolonCount >= 2) {
			if (c == '#') continue;
			if(posBuff.size() < 2) posBuff.push_back(c);
			else if (posBuff.size() == 2) idBuff.push_back(c);
		}
	}

	name = nameBuff;
	if (posAsString.size() == 2) {
		boardPos.x = posAsString[0] - '0';
		boardPos.y = posAsString[1] - '0';
	}
	else {
		boardPos.x = 0;
		boardPos.y = 0;
	}
}

std::string CompressedPiece::parseToString() {
	std::string cPieceAsString = name;
	cPieceAsString.push_back(';');

	cPieceAsString.push_back(std::to_string(boardPos.x)[0]);
	cPieceAsString.push_back(std::to_string(boardPos.y)[0]);
	cPieceAsString.push_back(';');

	for (auto m : legalMoves) {
		cPieceAsString.push_back(std::to_string(m.moveCoords.x)[0]);
		cPieceAsString.push_back(std::to_string(m.moveCoords.y)[0]);
		cPieceAsString.push_back('#');
		cPieceAsString.append(std::to_string(m.id));
		cPieceAsString.push_back(';');
	}

	return cPieceAsString;
}

void CompressedPiece::print() {
	std::cout << name << std::endl;
	std::cout << "x=" << boardPos.x << "  y=" << boardPos.y << std::endl;
	std::cout << "LEGAL MOVES:" << std::endl;
	for (auto m : legalMoves) {
		std::cout << "x=" << m.moveCoords.x << "  y=" << m.moveCoords.y << "   id=" << m.id << std::endl;
	}
}