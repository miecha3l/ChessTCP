#include "ChessBot.h"
#include <chrono>
#include <thread>
ChessBot::ChessBot()
{
}

Move ChessBot::getMove(Board b, std::string color)
{
	std::vector<Piece*>* botsPieces = color == "white" ? &b.white_pieces : &b.black_pieces;
	std::vector<Move> defaults;
	std::vector<Move> captures;
	for (auto p : *botsPieces) {
		for (auto m : p->legalMoves) {
			int value = 0;
			if (m.isKnockingOut) {
				captures.push_back(m);
			}
			else defaults.push_back(m);
		}
	}
	if (captures.size() > 0) return captures[rand() % captures.size()];
	else return defaults[rand() % defaults.size()];
}
