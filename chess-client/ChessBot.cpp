#include "ChessBot.h"
ChessBot::ChessBot()
{
}

Move ChessBot::getMove(Board b, std::string color)
{
	std::vector<Move> allMoves;
	if (color == "black") {
		for (auto p : b.black_pieces) for (auto m : p->legalMoves) allMoves.push_back(m);
	}
	else if (color == "white") {
		for (auto p : b.black_pieces) for (auto m : p->legalMoves) allMoves.push_back(m);
	}

	return allMoves[rand() % allMoves.size()];
}
