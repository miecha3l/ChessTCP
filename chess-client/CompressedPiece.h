#pragma once
#include <iostream>
#include <string>
#include <list>
#include "Chess.h"
#include <SFML/Graphics.hpp>

struct CompressedMove {
	sf::Vector2i moveCoords;
	int id;
	CompressedMove(sf::Vector2i mc, int id) : moveCoords(mc), id(id) {}
};

struct CompressedPiece {
	std::string name;
	sf::Vector2i boardPos;
	std::vector<CompressedMove> legalMoves;

	CompressedPiece(std::string name, sf::Vector2i pos, std::list<Move> legals) : name(name), boardPos(pos), legalMoves(std::vector<CompressedMove>()) {
		for (auto m : legals) {
			legalMoves.push_back(CompressedMove(m.moveBoardCoords, m.id));
		}
	}
	CompressedPiece(std::string pieceAsString); 
	std::string parseToString();
	void print();
};