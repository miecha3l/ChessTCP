#include "board.h"
#include "BoardAnalisys.h"
#include "Chess.h"


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

Piece *Board::getPieceWithCoords(int x, int y) {
	for (auto p : white_pieces) if (p->board_pos.x == x && p->board_pos.y == y) return p;
	for (auto p : black_pieces) if (p->board_pos.x == x && p->board_pos.y == y) return p;
	return NULL;
}

void Board::createClassicSet(std::string color) {
	if (color == "white") {
		for (int i = 0; i < 8; ++i) {
			addPiece((new Pawn(i, 6, "white")));
		}
		addPiece(new Rook(0, 7, "white"));
		addPiece(new Rook(7, 7, "white"));
		addPiece(new Knight(1, 7, "white"));
		addPiece(new Knight(6, 7, "white"));
		addPiece(new Bishop(2, 7, "white"));
		addPiece(new Bishop(5, 7, "white"));
		addPiece(new Queen(3, 7, "white"));
		addPiece(new King(4, 7, "white"));
	}
	else if (color == "black") {
		for (int i = 0; i < 8; ++i) {
			addPiece((new Pawn(i, 1, "black")));
		}
		addPiece(new Rook(0, 0, "black"));
		addPiece(new Rook(7, 0, "black"));
		addPiece(new Knight(1, 0, "black"));
		addPiece(new Knight(6, 0, "black"));
		addPiece(new Bishop(2, 0, "black"));
		addPiece(new Bishop(5, 0, "black"));
		addPiece(new Queen(3, 0, "black"));
		addPiece(new King(4, 0, "black"));
	}
	else {
		std::cout << "Invalid color";
	}
}


std::string Board::getBlackTime() { return blackTime; }
std::string Board::getWhiteTime() { return whiteTime; }
std::string Board::getCurrentTurn() { return currentTurn; }
void Board::setCurrentTurn(std::string t) { currentTurn = t; }
std::string Board::getCurrentFlag() { return currentFlag; }

void Board::setCurrentFlag(std::string s)
{
	currentFlag = s;
}
