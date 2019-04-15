#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include <vector>
#include "piece.h"

/**
  *\class Board
  *\brief This class holds all data for engine to process.
  *
  *It is wrapper that holds pieces data, current state of the game and move stack and
  *provides possibility to process this data in easy way.
  *
  */
class Board{
private:
	/**
	  *\brief This array represents gameboard that is understandable for an algorithm
	  *
	  *It basically holds set of integer which represents gameboard (1 is for black and 2 is for white).
	  *Its job is to enable algoritm to determine if certain square is empty/can be captured.
	  */
    int m_operationalBoard[8][8];

	///Holds info about whoose turn it is.
	std::string currentTurn;

	/**
	  *\brief Holds info about current gamestate.
	  *
	  *States can be: "check", "checkmate", "stalemate", "disconnected"
	  *Its job is to communicate those states to client.
	  */
	std::string currentFlag;


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
	  *@param int x coordinate
	  *@param int y coordinate
	  *@return Piece on given coorinates
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
	  *@param std::string new turn as string
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
	  *Sets current flag parameter
	  */
	void setCurrentFlag(std::string);

	/**
	  *Creates entire classic set of chess figures. Used to initialize gameboard, and prepare it for game.
	  *@param std::string color what color to create set in
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