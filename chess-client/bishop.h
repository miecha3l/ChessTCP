#include "piece.h"

class Bishop : public Piece {

public:
    Bishop(int x, int y, std::string col);
    void findLegalMoves(Board b);
};