#include "pawn.h"
#include "board.h"

Pawn::Pawn(int x, int y, std::string col){
	color = col;
    board_pos = sf::Vector2i(x, y);
    moves = 0;
    name = "pawn";
}

void Pawn::findLegalMoves(Board b){
    this->legalMoves.clear();
    if(this->color == "white"){

		if (board_pos.y - 1 >= 0 && b.m_operationalBoard[board_pos.y - 1][board_pos.x] == 0) {
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x, board_pos.y - 1), this, false, 0));
		}

		if (board_pos.y - 1 >= 0 && b.m_operationalBoard[board_pos.y - 2][board_pos.x] == 0 && b.m_operationalBoard[board_pos.y - 1][board_pos.x] == 0 && moves == 0) {
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x, board_pos.y - 2), this, false, 0));
		}

		if ((board_pos.y - 1 >= 0 && board_pos.x - 1 >= 0 && board_pos.x - 1 <= 7) && b.m_operationalBoard[board_pos.y - 1][board_pos.x - 1] == 1) {
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x - 1, board_pos.y - 1), this, true, 1, b.getPieceWithCoords(board_pos.x - 1, board_pos.y - 1)));
		}

		if ((board_pos.y - 1 >= 0 && board_pos.x + 1 >= 0 && board_pos.x + 1 <= 7) && b.m_operationalBoard[board_pos.y - 1][board_pos.x + 1] == 1) {
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x + 1, board_pos.y - 1), this, true, 1, b.getPieceWithCoords(board_pos.x + 1, board_pos.y - 1)));
		}
    }

    else if(this->color == "black"){

		if (board_pos.y + 1 < 8 && b.m_operationalBoard[board_pos.y + 1][board_pos.x] == 0) {
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x, board_pos.y + 1), this, false, 0));
		}

		if (board_pos.y + 1 < 8 && b.m_operationalBoard[board_pos.y + 2][board_pos.x] == 0 && b.m_operationalBoard[board_pos.y + 1][board_pos.x] == 0 && moves == 0) {
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x, board_pos.y + 2), this, false, 0));
		}

		if ((board_pos.y + 1 < 8 && board_pos.x - 1 >= 0 && board_pos.x - 1 <= 7) && b.m_operationalBoard[board_pos.y + 1][board_pos.x - 1] == 2) {
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x - 1, board_pos.y + 1), this, true, 2, b.getPieceWithCoords(board_pos.x - 1, board_pos.y - 1)));
		}

		if ((board_pos.y + 1 < 8 && board_pos.x + 1 >= 0 && board_pos.x + 1 <= 7) && b.m_operationalBoard[board_pos.y + 1][board_pos.x + 1] == 2) {
			this->legalMoves.push_back(Move(this->getBoardPosition(), sf::Vector2i(board_pos.x + 1, board_pos.y + 1), this, true, 2, b.getPieceWithCoords(board_pos.x + 1, board_pos.y - 1)));
		}
    }
}
