#include "piece.h"

/**
 *\class Queen
 *\brief Class that represents Queen piece.
 *
 *This class contains Queens constructor and Queens Piece::findLegalMoves() implementation.
 */
class Queen : public Piece {

public:
	/**
	  *\brief Queen constructor
	  *
	  *@param int x coordinate
	  *@param int y coordinate
	  *@param std::string color of created piece
	  */
    Queen(int x, int y, std::string col);
    void findLegalMoves(Board b);
};