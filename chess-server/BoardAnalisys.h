#pragma once

class Board;

class BoardAnalisys{

public:
    static void revalidateBlackMoves(Board &board); //delegalize moves that can put player in check
    static void revalidateWhiteMoves(Board &board); 
    static bool isWhiteInCheck(Board board);
    static bool isBlackInCheck(Board board);
    static bool isWhiteInCheckmate(Board board);
    static bool isBlackInCheckmate(Board board);
    static bool isWhiteInStalemate(Board board);
    static bool isBlackInStalemate(Board board);
};
