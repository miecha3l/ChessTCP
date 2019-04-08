#include "Server.h"
#include "Windows.h"
#include "GameInstance.h"
#include <list>
#include "Request.h"
Server* Server::obj = NULL;
std::list<GameInstance*> gameInstances;
std::list<Board*> boards;
std::map<Player*, GameInstance*> gameOf;
extern Queue communicationQueue;


Server::Server()
{
	client = new sf::TcpSocket();
	port = 8888;
	exists = true;
}

Server *Server::instance() {
	if (Server::obj == NULL) obj = new Server();
	return obj;
}

void Server::acceptClients() {
	srand(time(NULL));
	while (true) {
		listener.listen(port);
		listener.accept(*client);
		if (client->getRemotePort() == 0) continue;

		players.push_back(new Player(client, std::to_string(client->getRemotePort())));
		playerThreads.push_back(new sf::Thread(&Player::communicate, players.back()));
		playerThreads.back()->launch();
		threadOf[players.back()] = playerThreads.back();
		playerOf[players.back()->getId()] = players.back();

		sf::Packet welcomemsg;
		welcomemsg << std::to_string(players.back()->getId());
		players.back()->getClient()->send(welcomemsg);
		client = new sf::TcpSocket();
	}
}

void Server::updatePlayersAndGamesList() {
	while (true) {
		for (auto game : gameInstances) {
			if (game->isAnyPlayerDisconnected()) {
				bool deleted = false;
				for (auto g = gameInstances.begin(); g != gameInstances.end(); g++) {
					if (*g == game) {
						if (game->getWhite() != NULL) {
							sf::Packet cont;
							cont << "game_over/enemy_disconnected";
							matchOf[game->getWhite()]->setPlayersStatus(Player::Status::Idle);
							matchOf[game->getWhite()]->getClient()->send(cont);
							game->getWhite()->getClient()->send(cont);
						}
						else if(game->getWhite() != NULL){
							sf::Packet cont;
							cont << "game_over/enemy_disconnected";
							matchOf[game->getBlack()]->setPlayersStatus(Player::Status::Idle);
							matchOf[game->getBlack()]->getClient()->send(cont);
							game->getBlack()->getClient()->send(cont);
						}
						delete *g;
						gameInstances.erase(g);
					}
				}
				if (deleted) break;
			}
		}

		for (auto plr : players) {
			bool deleted = false;
			if (plr->getPlayersStatus() == Player::Status::Disconnected) {

				//remove thread
				for (auto t : playerThreads) {
					bool deleted = false;
					if (threadOf[plr] == t) {
						for (auto trd = playerThreads.begin(); trd != playerThreads.end(); trd++) {
							if (*trd == t) {
								delete *trd;
								playerThreads.erase(trd);
								deleted = true;
								break;
							}
						}
					}
					if (deleted) break;
				}
			
				//remove player obj
				for (auto p = players.begin(); p != players.end(); p++) {
					if (*p == plr) {

						threadOf.erase(plr);
						matchOf.erase(matchOf[plr]);
						matchOf.erase(plr);
						playerOf.erase(plr->getId());
						gameOf.erase(plr);
						delete *p;
						players.erase(p);
						deleted = true;
						break;
					}
				}
			}
			if (deleted) break;
		}

		Sleep(2000);
	}
}

void Server::handleMessages() {
	while (true) {
		if (communicationQueue.size() > 0){
			std::string container;
			sf::Packet msg = communicationQueue.pop();
			msg >> container;

			Request req = Request::parse(container);
			if (req.isValid()) req.handle();
		}
	}
}

void Server::handleInput() {
	while (true) {
		std::string input;
		std::cout << "admin# ";
		std::cin >> input;
		if (input == "players") {
			Server::instance()->printPlayerList();
		}
		else if (input == "players-v") {
			Server::instance()->printPlayerListWithInfo();
		}
		else if (input == "games") {
			Server::instance()->printGameList();
		}
		else if (input == "clear") {
			system("cls");
			printASCII();
		}
		else if (input == "?") {
			std::cout << "? - show this help info \n";
			std::cout << "players - show players list \n";
			std::cout << "players-v - show detailed players list \n";
			std::cout << "games - show games list \n";
			std::cout << "clear - clear screen \n";
		}
		else {
			std::cout << "Wrong command.\n\n";
		}
	}
}

void Server::printASCII() {
	std::cout << R"(                       / ____| |  | |  ____|/ ____/ ____|)" << std::endl;
	std::cout << R"(                      | |    | |__| | |__  | (___| (___  )" << std::endl;
	std::cout << R"(                      | |    |  __  |  __|  \___ \\___ \ )" << std::endl;
	std::cout << R"(                      | |____| |  | | |____ ____) |___) |)" << std::endl;
	std::cout << R"(                       \_____|_|  |_|______|_____/_____/ )" << std::endl;
	std::cout << R"(                    / ____|  ____|  __ \ \    / /  ____|  __ \ )" << std::endl;
	std::cout << R"(                   | (___ | |__  | |__) \ \  / /| |__  | |__) |)" << std::endl;
	std::cout << R"(                    \___ \|  __| |  _  / \ \/ / |  __| |  _  / )" << std::endl;
	std::cout << R"(                    ____) | |____| | \ \  \  /  | |____| | \ \ )" << std::endl;
	std::cout << R"(                   |_____/|______|_|  \_\  \/   |______|_|  \_\)" << std::endl;

}

void Server::init() {
	sf::Thread accept(&Server::acceptClients, this);
	sf::Thread update(&Server::updatePlayersAndGamesList, this);
	sf::Thread handleMsg(&Server::handleMessages, this);
	sf::Thread cli(&Server::handleInput, this);

	printASCII();

	accept.launch();
	update.launch();
	handleMsg.launch();
	cli.launch();
}

void Server::printPlayerList()
{
	std::cout << "======================================================================\n";
	std::cout << "There is currently " << players.size() << " players online" << std::endl;
	for (auto p : players) {
		std::cout << "Player: " << p->getId() << std::endl;
	}
	std::cout << "======================================================================\n\n";
}

void Server::printPlayerListWithInfo()
{
	std::cout << "======================================================================\n";
	std::cout << "There are currently " << players.size() << " players online" << std::endl;
	for (auto p : players) {
		std::string status;
		std::cout << "Player: " << p->getId() << std::endl;
		switch (p->getPlayersStatus()) {
		case Player::Status::InGame:
			status = "in game";
			std::cout << "Player is: " << status << " of id: " << gameOf[p]->getId() << std::endl;
			break;

		case Player::Status::Idle:
			status = "idle";
			std::cout << "Player is: " << status << std::endl;
			break;

		default: 
			status = "unknown";
			break;
		}
		if (matchOf[p] != NULL) {
			std::cout << "Players match: " << matchOf[p]->getId() << std::endl;
		}
		else std::cout << "Player doesnt have a match" << std::endl;
		
		
	}
	std::cout << "======================================================================\n\n";
}

void Server::printGameList()
{
	std::cout << "======================================================================\n";
	std::cout << "There are currently " << gameInstances.size() << " games being played" << std::endl;
	for (auto g : gameInstances) {
		std::cout << "Game: " << g->getId() << std::endl;
		std::cout << "Players: \n" << g->getBlack()->getId() << "(black)\n" << g->getWhite()->getId() << "(white)\n";
		std::cout << "\n";
	}
	std::cout << "======================================================================\n\n";
}


int Server::playersCount() {
	return players.size();
}

int Server::messagesCount() {
	return communicationQueue.size();
}

Player* Server::getPlayer(int id) {
	return playerOf[id];
}

Player *Server::getPlayerMatch(Player *p) {
	return matchOf[p];
}

void Server::matchPlayers(int a, int b){
	matchOf[getPlayer(a)] = getPlayer(b);
	matchOf[getPlayer(b)] = getPlayer(a);
}

Server::~Server()
{
}
