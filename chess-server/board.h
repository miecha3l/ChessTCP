#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include <vector>
#include "piece.h"

class Board{
private:
    int m_operationalBoard[8][8];
    
public:
    std::vector<Piece*> black_pieces;
    std::vector<Piece*> white_pieces;
	std::list<Move> moveStack;

public:
    Board();
    void addPiece(Piece *p);
    void showOperationalBoard();
	void setTheme(std::string theme);
	void createClassicSet(std::string);
    Piece *getWhiteKing();
    Piece *getBlackKing();
	Piece *getPieceWithCoords(int, int);


	static void undoLastMove(Board &b);


    friend class Piece;
    friend class Rook;
    friend class Knight;
    friend class Bishop;
    friend class King;
    friend class Queen;
    friend class Pawn;
    friend class ChessAnalisys;
};