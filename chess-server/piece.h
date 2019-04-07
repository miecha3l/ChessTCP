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

class Piece{
//protected variables
protected:
    sf::Vector2i board_pos;
    sf::Vector2f screen_pos;
    std::string color;
    bool isActive;
    int moves;
    std::string name;

//public variables
public:
	std::list<Move> legalMoves;
	const static float SPRITE_SCALE;

//virtual methods
public:
    
	/**
	  *This method is unique for every piece. It iteratively searches for every legal move that piece can make.
	  */
    virtual void findLegalMoves(Board b) = 0;
    

//public methods
public:
	/**
	  *ensures that move can be made, removes knocked pieces
	  */
	bool validateMove(Move&, Board &b);


	/**
	  *finalizes move, make changes to gameboard
	  */
    void pushMove(Move, Board&);


	/**
	  *undo changes made by pushMove()
	  */
    void undoMove(Move, Board&);

	/**
	  *remove move from legalMoves
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
	  */
    void setBoardPosition(sf::Vector2i);

	/**
	  *sets screen position
	  */
    void setScreenPosition(sf::Vector2f);
    
	/**
	  *returns pieces name
	  */
	std::string getName();

    friend class Board;
	friend struct Player;
};