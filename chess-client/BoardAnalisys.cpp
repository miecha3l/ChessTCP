#include "BoardAnalisys.h"
#include "board.h"

bool BoardAnalisys::isWhiteInCheck(Board board)
{
	sf::Vector2i w_kingPos = board.getWhiteKing()->getBoardPosition();
	for (auto p : board.black_pieces) {
		for (auto m : p->legalMoves) {
			if (m.moveBoardCoords.x == w_kingPos.x && m.moveBoardCoords.y == w_kingPos.y) {
				return true;
			}
		}
	}
	return false;
}

bool BoardAnalisys::isBlackInCheck(Board board)
{
	sf::Vector2i b_kingPos = board.getBlackKing()->getBoardPosition();
	for (auto p : board.white_pieces) {
		for (auto m : p->legalMoves) {
			if (m.moveBoardCoords.x == b_kingPos.x && m.moveBoardCoords.y == b_kingPos.y) {
				return true;
			}
		}
	}
	return false;
}

void BoardAnalisys::revalidateBlackMoves(Board &board) {
	sf::Vector2i b_kingPos = board.getBlackKing()->getBoardPosition();
	if (isBlackInCheck(board)) {
		for (auto bp : board.black_pieces) {
			std::vector<Move> movesToRemove;
			if (bp != board.getBlackKing()) {
				for (auto m : bp->legalMoves) {
					for (auto other_m : m.p_knockedOut->legalMoves) {
						if (other_m.p_knockedOut != board.getBlackKing()) {
							movesToRemove.push_back(m);
						}
					}
				}
			}
			for (auto m : movesToRemove) bp->eraseMove(m);
			movesToRemove.clear();
		}
	}
	else {
		for (auto bp : board.black_pieces) {
			std::vector<Move> movesToRemove;
			for (auto m : bp->legalMoves) {
				bool skip = false;
				bp->pushMove(m, board);

				for (auto wp : board.white_pieces) {
					wp->findLegalMoves(board);

					if (isBlackInCheck(board)) {
						bp->undoMove(m, board);
						skip = true;
						wp->legalMoves.clear();

						if (wp == m.p_knockedOut) break;
						movesToRemove.push_back(m);
						break;
					}
				}
				if (skip) continue;
				bp->undoMove(m, board);
			}
			for (auto mv : movesToRemove) bp->eraseMove(mv);
			movesToRemove.clear();
		}
	}


}

void BoardAnalisys::revalidateWhiteMoves(Board &board) {
	sf::Vector2i w_kingPos = board.getWhiteKing()->getBoardPosition();
	if (isWhiteInCheck(board)) {
		for (auto wp : board.white_pieces) {
			std::vector<Move> movesToRemove;
			if (wp != board.getWhiteKing()) {
				for (auto m : wp->legalMoves) {
					for (auto other_m : m.p_knockedOut->legalMoves) {
						if (other_m.p_knockedOut != board.getWhiteKing()) {
							movesToRemove.push_back(m);
						}
					}
				}
			}
			for (auto m : movesToRemove) wp->eraseMove(m);
			movesToRemove.clear();
		}
	}
	else {
		for (auto wp : board.white_pieces) {
			std::vector<Move> movesToRemove;
			for (auto m : wp->legalMoves) {
				bool skip = false;
				wp->pushMove(m, board);

				for (auto bp : board.black_pieces) {
					bp->findLegalMoves(board);

					if (isWhiteInCheck(board)) {
						wp->undoMove(m, board);
						skip = true;
						bp->legalMoves.clear();

						if (bp == m.p_knockedOut) break;
						movesToRemove.push_back(m);
						break;
					}
				}
				if (skip) continue;
				wp->undoMove(m, board);
			}
			for (auto mv : movesToRemove) wp->eraseMove(mv);
			movesToRemove.clear();
		}
	}
}

bool BoardAnalisys::isBlackInCheckmate(Board b) {
	int totalPlayersMoves = 0;
	for (auto bp : b.black_pieces) {
		totalPlayersMoves += bp->legalMoves.size();
	}
	if (isBlackInCheck(b) && totalPlayersMoves == 0) return true;
	return false;
}

bool BoardAnalisys::isWhiteInCheckmate(Board b) {
	int totalPlayersMoves = 0;
	for (auto wp : b.white_pieces) {
		totalPlayersMoves += wp->legalMoves.size();
	}
	if (isWhiteInCheck(b) && totalPlayersMoves == 0) return true;
	return false;
}

bool BoardAnalisys::isBlackInStalemate(Board b) {
	int totalPlayersMoves = 0;
	for (auto bp : b.black_pieces) {
		totalPlayersMoves += bp->legalMoves.size();
	}
	if (!isBlackInCheck(b) && totalPlayersMoves == 0) return true;
	return false;
}

bool BoardAnalisys::isWhiteInStalemate(Board b) {
	int totalPlayersMoves = 0;
	for (auto wp : b.white_pieces) {
		totalPlayersMoves += wp->legalMoves.size();
	}
	if (!isWhiteInCheck(b) && totalPlayersMoves == 0) return true;
	return false;
}