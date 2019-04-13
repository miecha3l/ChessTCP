#include "piece.h"

/**
 *\class King
 *\brief Class that represents king piece.
 *
 *This class contains kings constructor and kings Piece::findLegalMoves() implementation.
 */
class King : public Piece {

public:
	/**
	  *\brief King constructor
	  *
	  *@param int x coordinate
	  *@param int y coordinate
	  *@param std::string color of created piece
	  */
    King(int x, int y, std::string col);
    void findLegalMoves(Board b);
};