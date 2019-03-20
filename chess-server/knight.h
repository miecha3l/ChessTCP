#include "piece.h"

class Knight : public Piece {

public:
    Knight(int x, int y, std::string col);
    void findLegalMoves(Board b);
};