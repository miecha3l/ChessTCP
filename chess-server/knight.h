#include "piece.h"

/**
 *\class Knight
 *\brief Class that represents Knight piece.
 *
 *This class contains Knights constructor and Knights Piece::findLegalMoves() implementation.
 */
class Knight : public Piece {

public:
	/**
	  *\brief Knight constructor
	  *
	  *@param int x coordinate
	  *@param int y coordinate
	  *@param std::string color of created piece
	  */
    Knight(int x, int y, std::string col);
    void findLegalMoves(Board b);
};