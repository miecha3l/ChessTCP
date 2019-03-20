#include "piece.h"

class Rook : public Piece {

public:
    Rook(int x, int y, std::string);
    virtual void findLegalMoves(Board b);
};