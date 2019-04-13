#include "piece.h"

/**
 *\class Bishop
 *\brief Class that represents bishop piece.
 *
 *This class contains bishop constructor and bishops Piece::findLegalMoves() implementation.
 */
class Bishop : public Piece {

public:
	/**
	  *\brief Bishop constructor
	  *
	  *@param int x coordinate
	  *@param int y coordinate
	  *@param std::string color of created piece
	  */
    Bishop(int x, int y, std::string col);
    void findLegalMoves(Board b);
};