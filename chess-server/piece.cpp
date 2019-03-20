#include "piece.h"
#include "board.h"

Piece::Piece() {}

void Piece::setBoardPosition(sf::Vector2i np){
    board_pos.x = np.x;
    board_pos.y = np.y;
}

void Piece::setScreenPosition(sf::Vector2f np){
    screen_pos.x = np.x;
    screen_pos.y = np.y;
}

sf::Vector2i Piece::getBoardPosition(){
    return board_pos;
}

sf::Vector2f Piece::getScreenPosition(){
    return screen_pos;
}


void Piece::pushMove(Move m, Board &b) {
	this->moves += 1;
	int temp = b.m_operationalBoard[m.originalBoardCoords.y][m.originalBoardCoords.x];
	b.m_operationalBoard[m.originalBoardCoords.y][m.originalBoardCoords.x] = 0;
	b.m_operationalBoard[m.moveBoardCoords.y][m.moveBoardCoords.x] = temp;
	
	this->board_pos = m.moveBoardCoords;

	b.moveStack.push_back(m);
}

void Piece::undoMove(Move m, Board &b) {
	this->moves -= 1;
	int temp = b.m_operationalBoard[m.moveBoardCoords.y][m.moveBoardCoords.x];
	b.m_operationalBoard[m.moveBoardCoords.y][m.moveBoardCoords.x] = m.i_knockedOut;
	b.m_operationalBoard[m.originalBoardCoords.y][m.originalBoardCoords.x] = temp;

	this->board_pos = m.originalBoardCoords;

	b.moveStack.pop_back();
}

void Piece::eraseMove(Move m) {
	for (auto m_it = this->legalMoves.begin(); m_it != this->legalMoves.end(); m_it++) {
		if (*m_it == m) {
			this->legalMoves.erase(m_it);
			break;
		}
	}
}


bool Piece::validateMove(Move &potMove, Board &b){
    
    for(auto m : this->legalMoves){
        if(potMove.moveBoardCoords.x == m.moveBoardCoords.x && potMove.moveBoardCoords.y == m.moveBoardCoords.y)
        {
            if(this->color == "white" && b.m_operationalBoard[potMove.moveBoardCoords.y][potMove.moveBoardCoords.x] % 2 == 1)
            {
                for(auto p : b.black_pieces){
					bool removed = false;
                    if(potMove.moveBoardCoords.x == p->getBoardPosition().x && potMove.moveBoardCoords.y == p->getBoardPosition().y){
                        for(auto it = b.black_pieces.begin(); it != b.black_pieces.end(); ++it){
							if (*it == p) {
								b.black_pieces.erase(it);
								removed = true;
								break;
							}
                        }
						if (removed) break;
                    }
                }
            }

            if(this->color == "black" && b.m_operationalBoard[potMove.moveBoardCoords.y][potMove.moveBoardCoords.x] == 2 && b.m_operationalBoard[potMove.moveBoardCoords.y][potMove.moveBoardCoords.x] != 0)
            {
                for(auto p : b.white_pieces){
					bool removed = false;
                    if(potMove.moveBoardCoords.x == p->getBoardPosition().x && potMove.moveBoardCoords.y == p->getBoardPosition().y){
                        for(auto it = b.white_pieces.begin(); it != b.white_pieces.end(); ++it){
							if (*it == p) {
								b.white_pieces.erase(it);
								removed = true;
								break;
							}
                        }
						if (removed) break;
                    }
                }
            }
            return true;
        }
    }
    return false;
}

std::string Piece::getName() {
	return name;
}