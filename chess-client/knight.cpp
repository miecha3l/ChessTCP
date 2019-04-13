#include "knight.h"
#include "board.h"

Knight::Knight(int x, int y, std::string col){
	color = col;
    board_pos = sf::Vector2i(x, y);
    moves = 0;
    name = "knight";
}

void Knight::findLegalMoves(Board b){
	this->legalMoves.clear();

    int other = 0;

    if(this->color == "white"){
        other = 1;
    } else if(this->color == "black"){
        other = 2;
    }
    //gorne L-ki

    if(((board_pos.y - 2 >= 0 && board_pos.x - 1 >= 0) && (b.m_operationalBoard[board_pos.y - 2][board_pos.x - 1] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x - 1, board_pos.y - 2), this, false, 0));
    }
	if ((board_pos.y - 2 >= 0 && board_pos.x - 1 >= 0) && (b.m_operationalBoard[board_pos.y - 2][board_pos.x - 1] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x - 1, board_pos.y - 2), this, true, other, b.getPieceWithCoords(board_pos.x - 1, board_pos.y - 2)));
	}

    if(((board_pos.y - 2 >= 0 && board_pos.x + 1 < 8) && (b.m_operationalBoard[board_pos.y - 2][board_pos.x + 1] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x + 1, board_pos.y - 2), this, false, 0));
    }
	if ((board_pos.y - 2 >= 0 && board_pos.x + 1 < 8) && (b.m_operationalBoard[board_pos.y - 2][board_pos.x + 1] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x + 1, board_pos.y - 2), this, true, other, b.getPieceWithCoords(board_pos.x + 1, board_pos.y - 2)));
	}

    if(((board_pos.y - 1 >= 0 && board_pos.x - 2 >= 0) && (b.m_operationalBoard[board_pos.y - 1][board_pos.x - 2] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x - 2, board_pos.y - 1), this, false, 0));
    }
	if ((board_pos.y - 1 >= 0 && board_pos.x - 2 >= 0) && (b.m_operationalBoard[board_pos.y - 1][board_pos.x - 2] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x - 2, board_pos.y - 1), this, true, other, b.getPieceWithCoords(board_pos.x - 2, board_pos.y - 1)));
	}

    if(((board_pos.y - 1 >= 0 && board_pos.x + 2 < 8) && (b.m_operationalBoard[board_pos.y - 1][board_pos.x + 2] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x + 2, board_pos.y - 1), this, false, 0));
    }
	if ((board_pos.y - 1 >= 0 && board_pos.x + 2 < 8) && (b.m_operationalBoard[board_pos.y - 1][board_pos.x + 2] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x + 2, board_pos.y - 1), this, true, other, b.getPieceWithCoords(board_pos.x + 2, board_pos.y - 1)));
	}

    //dolne L-ki

    if(((board_pos.y + 2 < 8 && board_pos.x - 1 >= 0) && (b.m_operationalBoard[board_pos.y + 2][board_pos.x - 1] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x - 1, board_pos.y + 2), this, false, 0));
    }
	if ((board_pos.y + 2 < 8 && board_pos.x - 1 >= 0) && (b.m_operationalBoard[board_pos.y + 2][board_pos.x - 1] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x - 1, board_pos.y + 2), this, true, other, b.getPieceWithCoords(board_pos.x - 1, board_pos.y + 2)));
	}

    if(((board_pos.y + 2 < 8 && board_pos.x + 1 < 8) && (b.m_operationalBoard[board_pos.y + 2][board_pos.x + 1] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x + 1, board_pos.y + 2), this, false, 0));
    }
	if ((board_pos.y + 2 < 8 && board_pos.x + 1 < 8) && (b.m_operationalBoard[board_pos.y + 2][board_pos.x + 1] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x + 1, board_pos.y + 2), this, true, other, b.getPieceWithCoords(board_pos.x + 1, board_pos.y + 2)));
	}

    if(((board_pos.y + 1 < 8 && board_pos.x - 2 >= 0) && (b.m_operationalBoard[board_pos.y + 1][board_pos.x - 2] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x - 2, board_pos.y + 1), this, false, 0));
    }
	if ((board_pos.y + 1 < 8 && board_pos.x - 2 >= 0) && (b.m_operationalBoard[board_pos.y + 1][board_pos.x - 2] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x - 2, board_pos.y + 1), this, true, other, b.getPieceWithCoords(board_pos.x - 2, board_pos.y + 1)));
	}

    if(((board_pos.y + 1 < 8 && board_pos.x + 2 < 8) && (b.m_operationalBoard[board_pos.y + 1][board_pos.x + 2] == 0))){
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x + 2, board_pos.y + 1), this, false, 0));
    }
	if ((board_pos.y + 1 < 8 && board_pos.x + 2 < 8) && (b.m_operationalBoard[board_pos.y + 1][board_pos.x + 2] == other)) {
		this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x + 2, board_pos.y + 1), this, true, other, b.getPieceWithCoords(board_pos.x + 2, board_pos.y + 1)));
	}
}