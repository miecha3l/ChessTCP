#pragma once
#include <iostream>
#include <string>
#include "CompressedPiece.h"
#include "Chess.h"
#include <SFML/Graphics.hpp>



class GameState
{
private:
	std::vector<CompressedPiece> whitePieces;
	std::vector<CompressedPiece> blackPieces;
	std::string currentGameTurn, currentFlag;
	std::string whiteTimeLeft;
	std::string blackTimeLeft;

public:
	GameState();
	GameState(std::string);
	GameState(Board);
	void setWhiteTimeLeft(std::string);
	std::string getWhiteTimeLeft();
	void setBlackTimeLeft(std::string);
	std::string getBlackTimeLeft();
	std::string getCurrentGameTurn();
	void setCurrentGameTurn(std::string);
	std::vector<CompressedPiece> getWhitePieces();
	std::vector<CompressedPiece> getBlackPieces();


	std::string parseGameStateToString();
	void print();

	~GameState();
};

