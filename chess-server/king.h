#include "piece.h"

class King : public Piece {

public:
    King(int x, int y, std::string col);
    void findLegalMoves(Board b);
};