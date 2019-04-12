#pragma once
#include "piece.h"
#include "board.h"

/**
 *\class Pawn
 *\brief Class that represents Pawn piece.
 *
 *This class contains Pawns constructor and Pawns Piece::findLegalMoves() implementation.
 */
class Pawn : public Piece {

public:
	/**
	  *\brief Pawn constructor
	  *
	  *@param int x coordinate
	  *@param int y coordinate
	  *@param std::string color of created piece
	  */
    Pawn(int x, int y, std::string col);
    void findLegalMoves(Board b);
};