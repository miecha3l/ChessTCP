#include "board.h"
#include "BoardAnalisys.h"


Board::Board(){
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            m_operationalBoard[i][j] = 0;
        }
    }
	currentTurn = "white";
	currentFlag = "playing";
	whiteTime = "0:00";
	blackTime = "0:00";
}

void Board::addPiece(Piece *p){
    if(p->color == "white"){
        white_pieces.push_back(p);
        m_operationalBoard[p->board_pos.y][p->board_pos.x] = 2;
    }
    else if (p->color == "black"){
        black_pieces.push_back(p);
        m_operationalBoard[p->board_pos.y][p->board_pos.x] = 1;
    }
}

void Board::showOperationalBoard(){
    for(int i = 0; i < 8; ++i){
        for(int j = 0; j < 8; ++j){
            std::cout << m_operationalBoard[i][j];
        }
        std::cout << "\n";
    }
}

Piece *Board::getWhiteKing(){
    for(auto p : white_pieces){
        if(p->name == "king") return p;
    }
    return NULL;
}
Piece *Board::getBlackKing(){
    for(auto p : black_pieces){
        if(p->name == "king") return p;
    }
    return NULL;
}

//void Board::undoLastMove(Board &b) {
//	auto lastMove = b.moveStack.back();
//	lastMove.owner->undoMove(lastMove, b);
//}

Piece *Board::getPieceWithCoords(int x, int y) {
	for (auto p : white_pieces) if (p->board_pos.x == x && p->board_pos.y == y) return p;
	for (auto p : black_pieces) if (p->board_pos.x == x && p->board_pos.y == y) return p;
	return NULL;
}


std::string Board::getBlackTime() { return blackTime; }
std::string Board::getWhiteTime() { return whiteTime; }
std::string Board::getCurrentTurn() { return currentTurn; }
void Board::setCurrentTurn(std::string t) { currentTurn = t; }
std::string Board::getCurrentFlag() { return currentFlag; }