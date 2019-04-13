#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

class Board;
class Piece;

struct Move {
	sf::Vector2i originalBoardCoords;
	sf::Vector2i moveBoardCoords;
	Piece *owner;
	bool isKnockingOut;
	int i_knockedOut;
	Piece *p_knockedOut;
	int id;
	Move(){}
	Move(sf::Vector2i original, sf::Vector2i moveCoords, Piece *own, bool knockout, int knockedOutCode) :
		originalBoardCoords(original),
		moveBoardCoords(moveCoords),
		owner(own),
		isKnockingOut(knockout),
		i_knockedOut(knockedOutCode),
		p_knockedOut(NULL)
	{
		id = rand() % 120345 + 1;
	}

	Move(sf::Vector2i original, sf::Vector2i moveCoords, Piece *own, bool knockout, int knockedOutCode, Piece *knockedOutPiece) :
		originalBoardCoords(original),
		moveBoardCoords(moveCoords),
		owner(own),
		isKnockingOut(knockout),
		i_knockedOut(knockedOutCode),
		p_knockedOut(knockedOutPiece)
	{
		id = rand() % 120345 + 1;
	}

	bool operator ==(Move other) {
		if (this->id == other.id) return true;
		else return false;
	}
};


/**
  *\class Piece
  *\brief Holds all neccesary information about one chess piece.
  *
  *This class is root class for all other piece relevant classes.
  *It holds all neccesary information about any kind of piece.
  *Thanks to polymorphysm it enables every piece to define its own move finding algorithm.
  */
class Piece{
//protected variables
protected:
	///holds Board::m_operationalBoard position
    sf::Vector2i board_pos;

	/**
	  *\brief holds calculated screen position
	  *
	  *Screen position can be calculated in any way desired.
	  */
    sf::Vector2f screen_pos;

	///holds color of certain piece
    std::string color;

	///determines if piece was selected by player
    bool isActive;

	///holds number of moves 
    int moves;

	/**
	  *\brief holds name of piece
	  *
	  *Name can be: "rook", "king", "pawn", "queen", "knight", "bishop"
	  */
    std::string name;

//public variables
public:
	/**
	  *\brief  Holds all moves player can legally make.
	  *
	  *This container is modified via findLegalMoves() and BoardAnalisys::revalidateWhiteMoves() / BoardAnalisys::revalidateBlackMoves()
	  */
	std::list<Move> legalMoves;

	/// Holds scale of sprite
	const static float SPRITE_SCALE;

//virtual methods
public:
    
	/**
	  *This method is unique for every piece. It iteratively searches for every legal move that piece can make.
	  *@param Board b board on which game is being played
	  */
    virtual void findLegalMoves(Board b) = 0;
    

//public methods
public:
	/**
	  *ensures that move can be made, removes knocked pieces
	  *@param Move& move to validate
	  *@param Board &b board on which move is being taken
	  *@return determines if the move can be taken
	  */
	bool validateMove(Move&, Board &b);


	/**
	  *finalizes move, make changes to gameboard
	  *@param Move move to push
	  *@param Board& bord on which move is taken
	  */
    void pushMove(Move, Board&);


	/**
	  *undo changes made by pushMove()
	  *@param Move move to undo
	  *@param Board& board on which move was taken
	  */
    void undoMove(Move, Board&);

	/**
	  *remove move from legalMoves
	  *@param Move move to remove from legalMoves
	  */
	void eraseMove(Move);

	/**
	  *default constructor
	  */
    Piece();


	/**
	  *returns board position 
	  */
    sf::Vector2i getBoardPosition();


	/**
	  *returns screeen postion
	  */
    sf::Vector2f getScreenPosition();


	/**
	  *sets board position
	  *@param sf::Vector2i new position
	  */
    void setBoardPosition(sf::Vector2i);

	/**
	  *sets screen position
	  *@param sf::Vector2i new position
	  */
    void setScreenPosition(sf::Vector2f);
    
	/**
	  *returns pieces name
	  */
	std::string getName();

    friend class Board;
	friend struct Player;
};