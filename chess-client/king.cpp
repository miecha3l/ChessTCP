#include "king.h"
#include "board.h"

King::King(int x, int y, std::string col){
	color = col;
    board_pos = sf::Vector2i(x, y);
    moves = 0;
    name = "king";
}

void King::findLegalMoves(Board b){
    this->legalMoves.clear();
    int other = 0;
    if(this->color == "white"){
        other = 1;
    }else if(this->color == "black"){
        other = 2;
    }

    int x = board_pos.x, y = board_pos.y;

    //lewo
    if(((x - 1 >= 0) && (b.m_operationalBoard[y][x - 1] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x - 1, y), this, false, 0));
    }
	if ((x - 1 >= 0) && (b.m_operationalBoard[y][x - 1] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x - 1, y), this, true, other, b.getPieceWithCoords(x - 1, y)));
	}

    //prawo
	if (((x + 1 < 8) && (b.m_operationalBoard[y][x + 1] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x + 1, y), this, false, 0));
	}
	if ((x + 1 < 8) && (b.m_operationalBoard[y][x + 1] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x + 1, y), this, true, other, b.getPieceWithCoords(x + 1, y)));
	}

    //gora
	if (((y - 1 >= 0) && (b.m_operationalBoard[y - 1][x] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, y - 1), this, false, 0));
	}
	if ((y - 1 >= 0) && (b.m_operationalBoard[y - 1][x] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, y - 1), this, true, other, b.getPieceWithCoords(x, y - 1)));
	}

    //dol
	if (((y + 1 < 8) && (b.m_operationalBoard[y + 1][x] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, y + 1), this, false, 0));
	}
	if ((y + 1 < 8) && (b.m_operationalBoard[y + 1][x] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, y + 1), this, true, other, b.getPieceWithCoords(x, y + 1)));
	}


    //prawo gora
    if(((y - 1 >= 0 && x + 1 < 8) && (b.m_operationalBoard[y - 1][x + 1] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x + 1, y - 1), this, false, 0));
    }
	if ((y - 1 >= 0 && x + 1 < 8) && (b.m_operationalBoard[y - 1][x + 1] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x + 1, y - 1), this, true, other, b.getPieceWithCoords(x + 1, y - 1)));
	}

    //lewo gora
	if (((y - 1 >= 0 && x - 1 >= 0) && (b.m_operationalBoard[y - 1][x - 1] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x - 1, y - 1), this, false, 0));
	}
	if ((y - 1 >= 0 && x - 1 >= 0) && (b.m_operationalBoard[y - 1][x - 1] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x - 1, y - 1), this, true, other, b.getPieceWithCoords(x - 1, y - 1)));
	}

    //prawo dol
	if (((x + 1 < 8 && y + 1 < 8) && (b.m_operationalBoard[y + 1][x + 1] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x + 1, y + 1), this, false, 0));
	}
	if ((x + 1 < 8 && y + 1 < 8) && (b.m_operationalBoard[y + 1][x + 1] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x + 1, y + 1), this, true, other, b.getPieceWithCoords(x + 1, y + 1)));
	}

    //lewo dol
    if(((x - 1 >= 0 && y + 1 < 8) && (b.m_operationalBoard[y + 1][x - 1] == 0))) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x - 1, y + 1), this, false, 0));
    }
	if ((x - 1 >= 0 && y + 1 < 8) && (b.m_operationalBoard[y + 1][x - 1] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x - 1, y + 1), this, true, other, b.getPieceWithCoords(x - 1, y + 1)));
	}

}