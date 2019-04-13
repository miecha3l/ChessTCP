#pragma once

class Board;
/**
  *\class BoardAnalisys
  *\brief Class that contains all necessary chess algorithms 
  *
  *This class is used as algorithm hub for the engine.
  *Methods contained in this class are neccesary to run game using this chess engine.
  *It is responsible for making sure that all moves proposed to player are 100% legal
  *and to monitor state of any game fed to algorithms.
  */
class BoardAnalisys{

public:
	/**
	  *\brief Assures that all moves proposed to black player are legal
	  *
	  *This is algorithm that analyse all moves of black player and removes
	  *all moves that are illegal i.e. moves that enable enamy to capture players king.
	  *This function modifies passed object on purpose!
	  *@param Board& board to analyse
	  */
    static void revalidateBlackMoves(Board &board);

	/**
	  *\brief Assures that all moves proposed to white player are legal
	  *
	  *This is algorithm that analyse all moves of white player and removes
	  *all moves that are illegal i.e. moves that enable enamy to capture players king.
	  *This function modifies passed object on purpose!
	  *@param Board& board to analyse
	  */
    static void revalidateWhiteMoves(Board &board); 

	/**
	  *\brief Checks if white player is in check
	  *
	  *Used in revalideteBlackMoves() and revalidateWhiteMoves() to determine if certain move
	  *puts player in check.
	  *@param Board board to analyse.
	  */
    static bool isWhiteInCheck(Board board);

	/**
	  *\brief Checks if black player is in check
	  *
	  *Used in revalideteBlackMoves() and revalidateWhiteMoves() to determine if certain move
	  *puts player in check.
	  *@param Board board to analyse.
	  */
    static bool isBlackInCheck(Board board);

	/**
	  *\brief Checks if white player is in checkmate
	  *
	  *Should be called every turn to check if player is in checkmate.
	  *It checks number of overall legal moves and if such a number is 0 it returns true
	  *@param Board board to analyse
	  *@return is white checkmated
	  */
    static bool isWhiteInCheckmate(Board board);

	/**
	  *\brief Checks if black player is in checkmate
	  *
	  *Should be called every turn to check if player is in checkmate.
	  *It checks number of overall legal moves and if such a number is 0 it returns true
	  *@param Board board to analyse
	  *@return is black checkmated
	  */
    static bool isBlackInCheckmate(Board board);

	/**
	  *\brief Checks if white player is in stalemate
	  *
	  *Should be called everyturn to check if player is in stalemate.
	  *It checks number of overall legal moves and if such number is 0 and player IS NOT 
	  *in check it returns true
	  *@param Board board to analyse
	  *@return is white stalemated
	  */
    static bool isWhiteInStalemate(Board board);

	/**
	  *\brief Checks if black player is in stalemate
	  *
	  *Should be called everyturn to check if player is in stalemate.
	  *It checks number of overall legal moves and if such number is 0 and player IS NOT
	  *in check it returns true
	  *@param Board board to analyse
	  *@return is black stalemated
	  */
    static bool isBlackInStalemate(Board board);
};
