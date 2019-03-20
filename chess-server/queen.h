#include "piece.h"

class Queen : public Piece {

public:
    Queen(int x, int y, std::string col);
    void findLegalMoves(Board b);
};