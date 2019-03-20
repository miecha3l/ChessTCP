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
	std::string currentGameState;
	std::string whiteTimeLeft;
	std::string blackTimeLeft;

public:
	GameState();
	GameState(std::string);
	GameState(Board, std::string, std::string, std::string);
	void setWhiteTimeLeft();
	std::string getWhiteTimeLeft();
	void setBlackTimeLeft();
	std::string getBlackTimeLeft();
	void getCurrentGameState();
	std::string setCurrentGameState();

	std::string parseGameStateToString();
	void print();

	~GameState();
};

