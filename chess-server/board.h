#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include <vector>
#include "piece.h"

class Board{
private:
    int m_operationalBoard[8][8];
	std::string currentTurn, currentFlag;
	std::string whiteTime, blackTime;

public:
    std::vector<Piece*> black_pieces;
    std::vector<Piece*> white_pieces;
	std::list<Move> moveStack;


public:
    Board();
    void addPiece(Piece *p);
    void showOperationalBoard();
    Piece *getWhiteKing();
    Piece *getBlackKing();
	Piece *getPieceWithCoords(int, int);
	void setWhiteTime(std::string);
	std::string getWhiteTime();
	void setBlackTime(std::string);
	std::string getBlackTime();
	void setCurrentTurn(std::string);
	std::string getCurrentTurn();
	std::string getCurrentFlag();
	void setCurrentFlag(std::string);

	//static void undoLastMove(Board &b);


    friend class Piece;
    friend class Rook;
    friend class Knight;
    friend class Bishop;
    friend class King;
    friend class Queen;
    friend class Pawn;
    friend class ChessAnalisys;
};