#include "bishop.h"
#include "board.h"

Bishop::Bishop(int x, int y, std::string col){
	color = col;
    board_pos = sf::Vector2i(x, y);
    moves = 0;
    name = "bishop";
}

void Bishop::findLegalMoves(Board b){
    this->legalMoves.clear();

    int other = 0;
    if(this->color == "white"){
        other = 1;
    } else if(this->color == "black"){
        other = 2;
    }

    int x = board_pos.x - 1, y = board_pos.y - 1;
    //lewo gora
    while(x >= 0 && y >= 0){
        if(b.m_operationalBoard[y][x] == 0){
			legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, y), this, false, 0));
        }
        else if(b.m_operationalBoard[y][x] == other){
			legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, y), this, true, other, b.getPieceWithCoords(x, y)));
            break;
        }
        else if(b.m_operationalBoard[y][x] != 0 && b.m_operationalBoard[y][x] != other){
            break;
        }

        x--;
        y--;
    }

    x = board_pos.x + 1;
    y = board_pos.y - 1;
    //prawo gora
    while(x < 8 && y >= 0){
        if(b.m_operationalBoard[y][x] == 0){
			legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, y), this, false, 0));
        }
        else if(b.m_operationalBoard[y][x] == other){
			legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, y), this, true, other, b.getPieceWithCoords(x, y)));
            break;
        }
        else if(b.m_operationalBoard[y][x] != 0 && b.m_operationalBoard[y][x] != other){
            break;
        }

        x++;
        y--;
    }

    x = board_pos.x - 1;
    y = board_pos.y + 1;
    //lewo dol
    while(x >= 0 && y < 8){
        if(b.m_operationalBoard[y][x] == 0){
			legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, y), this, false, 0));
        }
        else if(b.m_operationalBoard[y][x] == other){
			legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, y), this, true, other, b.getPieceWithCoords(x, y)));
            break;
        }
        else if(b.m_operationalBoard[y][x] != 0 && b.m_operationalBoard[y][x] != other){
            break;
        }

        x--;
        y++;
    }

    x = board_pos.x + 1;
    y = board_pos.y + 1;
    //prawo dol
    while(x < 8 && y < 8){
        if(b.m_operationalBoard[y][x] == 0){
			legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, y), this, false, 0));
        }
        else if(b.m_operationalBoard[y][x] == other){
			legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, y), this, true, other, b.getPieceWithCoords(x, y)));
            break;
        }
        else if(b.m_operationalBoard[y][x] != 0 && b.m_operationalBoard[y][x] != other){
            break;
        }

        x++;
        y++;
    }
}