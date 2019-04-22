#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/Audio.hpp"
#include "Chess.h"
#include "gui.h"
#include "GameState.h"
#include "Response.h"
#include "ChessBot.h"
#include <iostream>
#include "Client.h"
#include "SoloGame.h"

const int tileDims = 70;
const int offset = 100;

void drawGameState(GameState gs, std::string color);
CompressedPiece getPieceClicked(sf::RenderWindow &w, GameState gs, std::string color);
int getClickedMove(CompressedPiece &p, std::string color);
void drawLegalMoves(CompressedPiece p, std::string color);
void windowThread();