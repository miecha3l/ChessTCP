#include "rook.h"
#include "board.h"

Rook::Rook(int x, int y, std::string col){
	color = col;
    board_pos = sf::Vector2i(x, y);
    moves = 0;
    name = "rook";
}

void Rook::findLegalMoves(Board b){
    this->legalMoves.clear();
    int other = 0;
    if(this->color == "white"){
        other = 1;
    } else if(this->color == "black"){
        other = 2;
    }

    //na prawo
    for(int x = board_pos.x + 1; x < 8; ++x){
        if(b.m_operationalBoard[board_pos.y][x] == 0){
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, board_pos.y), this, false, 0));
        }
        else if(b.m_operationalBoard[board_pos.y][x] != 0 && b.m_operationalBoard[board_pos.y][x] == other){
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, board_pos.y), this, true, other, b.getPieceWithCoords(x, board_pos.y)));
            break;
        }
        else if(b.m_operationalBoard[board_pos.y][x] != 0 && b.m_operationalBoard[board_pos.y][x] != other){
            break;
        }
    }

    //na lewo
    for(int x = board_pos.x - 1; x >= 0; --x){
        if(b.m_operationalBoard[board_pos.y][x] == 0){
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, board_pos.y), this, false, 0));
        }
        else if(b.m_operationalBoard[board_pos.y][x] != 0 && b.m_operationalBoard[board_pos.y][x] == other){
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(x, board_pos.y), this, true, other, b.getPieceWithCoords(x, board_pos.y)));
            break;
        }
        else if(b.m_operationalBoard[board_pos.y][x] != 0 && b.m_operationalBoard[board_pos.y][x] != other){
            break;
        }
    }

        //gora
    for(int y = board_pos.y - 1; y >= 0; --y){
        if(b.m_operationalBoard[y][board_pos.x] == 0){
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x, y), this, false, 0));
        }
        else if(b.m_operationalBoard[y][board_pos.x] != 0 && b.m_operationalBoard[y][board_pos.x] == other){
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x, y), this, true, other, b.getPieceWithCoords(board_pos.x, y)));
            break;
        }
        else if(b.m_operationalBoard[y][board_pos.x] != 0 && b.m_operationalBoard[y][board_pos.x] != other){
            break;
        }
    }

    //dol
    for(int y = board_pos.y + 1; y < 8; ++y){
        if(b.m_operationalBoard[y][board_pos.x] == 0){
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x, y), this, false, 0));
        }
        else if(b.m_operationalBoard[y][board_pos.x] != 0 && b.m_operationalBoard[y][board_pos.x] == other){
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x, y), this, true, other, b.getPieceWithCoords(board_pos.x, y)));
            break;
        }
        else if(b.m_operationalBoard[y][board_pos.x] != 0 && b.m_operationalBoard[y][board_pos.x] != other){
            break;
        }
    }
}
