#include "GameInstance.h"

GameInstance::GameInstance(int portWhite, int portBlack, Board b) {
	ID = rand() % 94924895 + 3412;
	gameBoard = b;
	
	sf::TcpListener listener;
	std::cout << "Waiting for white on port " << portWhite << std::endl;
	listener.listen(portWhite);
	listener.accept(clientWhite);
	std::cout << "White connected" << std::endl;

	std::cout << "Waiting for black on port " << portBlack << std::endl;
	listener.listen(portBlack);
	listener.accept(clientBlack);
	std::cout << "Black connected" << std::endl;
}

void GameInstance::play() {
	bool clientDisconnected = false;
	bool firstTurn = true;

	while (!clientDisconnected) {
		GameState gs(gameBoard);

		sf::Packet message;
		message << gs.parseGameStateToString();
		if (firstTurn) {
			clientBlack.send(message);
			clientWhite.send(message);
			firstTurn = false;
		}
		else {
			if (gameBoard.getCurrentTurn() == "black") {
				clientBlack.send(message);
			}
			else {
				clientWhite.send(message);
			}
		}


		sf::Packet response;
		std::string container;
		if (gameBoard.getCurrentTurn() == "black") {
			clientBlack.receive(response);
		}
		else {
			clientWhite.receive(response);
		}
		response >> container;

		try {
			int moveId = std::stoi(container);
			if (gameBoard.getCurrentTurn() == "white") {
				for (auto p : gameBoard.white_pieces) {
					for (auto m : p->legalMoves) {
						if (m.id == moveId) {
							if (p->validateMove(m, gameBoard)) p->pushMove(m, gameBoard);
						}
					}
				}
			}
			else {
				for (auto p : gameBoard.black_pieces) {
					for (auto m : p->legalMoves) {
						if (m.id == moveId) {
							if (p->validateMove(m, gameBoard)) p->pushMove(m, gameBoard);
						}
					}
				}
			}



			if (gameBoard.getCurrentTurn() == "white") {
				gameBoard.setCurrentTurn("black");
				for (auto p : gameBoard.black_pieces) p->findLegalMoves(gameBoard);
				BoardAnalisys::revalidateBlackMoves(gameBoard);

				gs = GameState(gameBoard);
				sf::Packet message;
				message << gs.parseGameStateToString();
				clientWhite.send(message);
			}
			else if (gameBoard.getCurrentTurn() == "black") {
				gameBoard.setCurrentTurn("white");
				for (auto p : gameBoard.white_pieces) p->findLegalMoves(gameBoard);
				BoardAnalisys::revalidateWhiteMoves(gameBoard);

				gs = GameState(gameBoard);
				sf::Packet message;
				message << gs.parseGameStateToString();
				clientBlack.send(message);
			}


		}
		catch (std::invalid_argument &e) {
			clientDisconnected = true;
		}
	}
}