#pragma once
#include "piece.h"
#include "board.h"

class Pawn : public Piece {

public:
    Pawn();
    Pawn(int x, int y, std::string col);
    void findLegalMoves(Board b);
};