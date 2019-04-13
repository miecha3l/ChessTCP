#pragma once
#include <iostream>
#include <string>
#include "Chess.h"

class ChessBot
{
private:

public:
	ChessBot();
	Move getMove(Board b, std::string color);
};

