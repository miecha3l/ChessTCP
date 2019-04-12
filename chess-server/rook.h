#include "piece.h"

/**
 *\class Rook
 *\brief Class that represents Rook piece.
 *
 *This class contains Rooks constructor and Rooks Piece::findLegalMoves() implementation.
 */
class Rook : public Piece {

public:
	/**
	  *\brief Rook constructor
	  *
	  *@param int x coordinate 
	  *@param int y coordinate
	  *@param std::string color of created piece
	  */
    Rook(int x, int y, std::string);
    virtual void findLegalMoves(Board b);
};