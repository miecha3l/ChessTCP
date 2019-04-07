#include "GameInstance.h"
extern std::map<GameInstance*, Board*> boardOf;
GameInstance::GameInstance(Player *w, Player *b, Board *board) {
	ID = rand() % 94924895 + 3412;
	gameBoard = board;
	white = w;
	black = b;
	
}

void GameInstance::play() {
	srand(time(NULL));
	bool clientDisconnected = false;
	bool firstTurn = true;

	white->getClient()->setBlocking(true);
	black->getClient()->setBlocking(true);

	/*sf::Packet control; std::string debug;
	clientBlack->receive(control);
	clientWhite->receive(control);
	debug.clear();
	control.clear();*/

	for (auto p : gameBoard->black_pieces) p->findLegalMoves(*gameBoard);
	for (auto p : gameBoard->white_pieces) p->findLegalMoves(*gameBoard);
	do{
		GameState gs(*gameBoard);

		std::cout << "\n\n\n" << gs.parseGameStateToString() << std::endl;

		sf::Packet message;
		message << gs.parseGameStateToString();
		if (firstTurn) {
			white->getClient()->send(message);
			black->getClient()->send(message);
			firstTurn = false;
		}
		else {
			if (gameBoard->getCurrentTurn() == "black") {
				black->getClient()->send(message);
			}
			else {
				white->getClient()->send(message);
			}
		}


		sf::Packet response;
		std::string container;
		if (gs.getCurrentGameTurn() == "black") {
			if (black->getClient()->receive(response) == sf::Socket::Status::Done) {
				std::cout << "received properly" << std::endl;
			}
			else std::cout << "not actually recived" << std::endl;
		}
		else {
			if (white->getClient()->receive(response) == sf::Socket::Status::Done) {
				std::cout << "received properly" << std::endl;
			}
			else std::cout << "not actually recived" << std::endl;
		}
		response >> container;
		std::cout << container << std::endl;
		response.clear();

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

			container.clear();

			if (gameBoard->getCurrentTurn() == "white") {
				gameBoard->setCurrentTurn("black");
				for (auto p : gameBoard->black_pieces) p->findLegalMoves(*gameBoard);
				BoardAnalisys::revalidateBlackMoves(*gameBoard);

				gs = GameState(*gameBoard);
				sf::Packet message;
				message << gs.parseGameStateToString();
				white->getClient()->send(message);
			}
			else if (gameBoard->getCurrentTurn() == "black") {
				gameBoard->setCurrentTurn("white");
				for (auto p : gameBoard->white_pieces) p->findLegalMoves(*gameBoard);
				BoardAnalisys::revalidateWhiteMoves(*gameBoard);

				gs = GameState(*gameBoard);
				sf::Packet message;
				message << gs.parseGameStateToString();
				black->getClient()->send(message);
			}


		} 
		catch (std::invalid_argument &e) {
			std::cout << "whops" << std::endl;
			clientDisconnected = true;
			white->setPlayersStatus(Player::Status::Idle);
			black->setPlayersStatus(Player::Status::Idle);
		}
	} while (!clientDisconnected);
}

GameInstance::~GameInstance() {
	delete boardOf[this];
	delete gameBoard;
}