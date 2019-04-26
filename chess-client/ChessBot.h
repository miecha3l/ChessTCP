#pragma once
#include <iostream>
#include <string>
#include "Chess.h"

class ChessBot
{
private:

public:
	enum pieceValues {
		PAWN = 1,
		BISHOP = 3,
		KNIGHT = 3,
		ROOK = 5,
		QUEEN = 9,
		KING = 100
	};
	ChessBot();
	Move getMove(Board b, std::string color);
};

