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
	/**
	  *Default constructor
	  */
    Board();

	/**
	  *Adds new piece to the board
	  */
    void addPiece(Piece *p);

	/**
	  *Debug method. Prints abstract board of integers to ilustrate whats going on under the hood.
	  */
    void showOperationalBoard();

	/**
	  *Returns pointer of type Piece* to white king
	  */
    Piece *getWhiteKing();

	/**
	  *Returns pointer of type Piece* to black king
	  */
    Piece *getBlackKing();

	/**
	  *Allows engine to find piece by its coordinates
	  */
	Piece *getPieceWithCoords(int, int);

	/**
	  *Returns time that white has left this game (may be removed)
	  */
	std::string getWhiteTime();

	/**
	  *Returns time that black has left this game (may be removed)
	  */
	std::string getBlackTime();

	/**
	  *Sets current turn. Used to update game.
	  */
	void setCurrentTurn(std::string);

	/**
	  *Returns string "black" or "white" which indicate whose turn is it.
	  */
	std::string getCurrentTurn();

	/**
	  *Returns current flag such as "check", "checkmate", "stalemate", "timeout", "forfeit".
	  *Method not completed neither used yet.
	  */
	std::string getCurrentFlag();

	/**
	  *Creates entire classic set of chess figures. Used to initialize gameboard, and prepare it for game.
	  */
	void createClassicSet(std::string color);



    friend class Piece;
    friend class Rook;
    friend class Knight;
    friend class Bishop;
    friend class King;
    friend class Queen;
    friend class Pawn;
    friend class ChessAnalisys;
};