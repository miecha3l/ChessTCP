#include "GameInstance.h"

GameInstance::GameInstance(sf::TcpSocket *w, sf::TcpSocket *b, Board *board) {
	ID = rand() % 94924895 + 3412;
	gameBoard = board;
	clientWhite = w;
	clientBlack = b;
	
}

void GameInstance::play() {
	bool clientDisconnected = false;
	bool firstTurn = true;
	sf::Packet control;
	clientBlack->receive(control);
	clientWhite->receive(control);
	control.clear();

	for (auto p : gameBoard->black_pieces) p->findLegalMoves(*gameBoard);
	for (auto p : gameBoard->white_pieces) p->findLegalMoves(*gameBoard);
	while (!clientDisconnected) {
		
		GameState gs(*gameBoard);

		std::cout << "\n\n\n" << gs.parseGameStateToString() << std::endl;

		sf::Packet message;
		message << gs.parseGameStateToString();
		if (firstTurn) {
			clientBlack->send(message);
			clientWhite->send(message);
			firstTurn = false;
		}
		else {
			if (gameBoard->getCurrentTurn() == "black") {
				clientBlack->send(message);
			}
			else {
				clientWhite->send(message);
			}
		}


		sf::Packet response;
		std::string container;
		if (gameBoard->getCurrentTurn() == "black") {
			std::cout << "waiting for white response" << std::endl;
			clientBlack->receive(response);
			std::cout << "received" << std::endl;
		}
		else {
			std::cout << "waiting for white response" << std::endl;
			clientWhite->receive(response);
			std::cout << "received" << std::endl;
		}
		response >> container;
		std::cout << container << std::endl;

		try {
			int moveId = std::stoi(container);
			if (gameBoard->getCurrentTurn() == "white") {
				for (auto p : gameBoard->white_pieces) {
					for (auto m : p->legalMoves) {
						if (m.id == moveId) {
							if (p->validateMove(m, *gameBoard)) p->pushMove(m, *gameBoard);
						}
					}
				}
			}
			else {
				for (auto p : gameBoard->black_pieces) {
					for (auto m : p->legalMoves) {
						if (m.id == moveId) {
							if (p->validateMove(m, *gameBoard)) p->pushMove(m, *gameBoard);
						}
					}
				}
			}



			if (gameBoard->getCurrentTurn() == "white") {
				gameBoard->setCurrentTurn("black");
				for (auto p : gameBoard->black_pieces) p->findLegalMoves(*gameBoard);
				BoardAnalisys::revalidateBlackMoves(*gameBoard);

				gs = GameState(*gameBoard);
				sf::Packet message;
				message << gs.parseGameStateToString();
				clientWhite->send(message);
			}
			else if (gameBoard->getCurrentTurn() == "black") {
				gameBoard->setCurrentTurn("white");
				for (auto p : gameBoard->white_pieces) p->findLegalMoves(*gameBoard);
				BoardAnalisys::revalidateWhiteMoves(*gameBoard);

				gs = GameState(*gameBoard);
				sf::Packet message;
				message << gs.parseGameStateToString();
				clientBlack->send(message);
			}


		}
		catch (std::invalid_argument &e) {
			clientDisconnected = true;
		}
	}
}