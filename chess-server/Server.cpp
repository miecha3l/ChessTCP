#include "Server.h"
#include "Windows.h"
#include "GameInstance.h"
#include <list>
#include "Request.h"
Server* Server::obj = NULL;
std::list<GameInstance*> gameInstances;
std::map<Player*, GameInstance*> gameOf;
extern Queue<sf::Packet> communicationQueue;
extern Queue<Player*> playersToRemove;

GameInstance* getGameOf(Player*p) {
	auto outcome = gameOf.find(p);
	if (outcome == gameOf.end()) {
		return NULL;
	}
	else return outcome->second;
}

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
		if (client->getRemotePort() <= 0) continue;

		players.push_back(new Player(client, std::to_string(client->getRemotePort())));
		playerThreads.push_back(new sf::Thread(&Player::communicate, players.back()));
		playerThreads.back()->launch();
		threadOf[players.back()] = playerThreads.back();
		playerOf[players.back()->getId()] = players.back();

		sf::Packet welcomemsg;
		welcomemsg << std::to_string(players.back()->getId());
		players.back()->getClient()->send(welcomemsg);
		client = new sf::TcpSocket();
		Sleep(200);
	}
}

void Server::removeMatch(int id) {
	matchOf.erase(getPlayer(id));
}

void Server::updatePlayersAndGamesList() {
	while (true) {
		if (playersToRemove.size() > 0) {
			Player *p = playersToRemove.pop();
			std::cout << "Deleting: \n";
			std::cout << "\n" << p->getId() << "\n";
			deletePlayer(p);
		}
		Sleep(100);
	}
}

void Server::deletePlayer(Player*p) {
	//delete clients thread
	for (auto trd = playerThreads.begin(); trd != playerThreads.end(); trd++) {
		if (*trd == getThreadOf(p)) {
			delete *trd;
			threadOf.erase(p);
			playerThreads.erase(trd);
			break;
		}
	}


	//delete players game
	if (getGameOf(p) != NULL) {
		sf::Packet cont;
		cont << "game_over/enemy_disconnected";
		getPlayerMatch(p)->getClient()->send(cont);
		getPlayerMatch(p)->setPlayersStatus(Player::Status::Idle);
		gameOf.erase(getPlayerMatch(p));
		for (auto game : gameInstances) {
			if (game == getGameOf(p)) {
				bool deleted = false;
				for (auto g = gameInstances.begin(); g != gameInstances.end(); g++) {
					if (*g == game) {
						delete *g;
						gameInstances.erase(g);
						deleted = true;
						break;
					}
				}
				if (deleted) break;
			}
		}
	}


	//deleting all connections
	if (getPlayerMatch(p) != NULL) {
		getPlayerMatch(p)->setPlayersStatus(Player::Status::Idle);
		matchOf.erase(getPlayerMatch(p));
	}
	matchOf.erase(p);
	playerOf.erase(p->getId());
	gameOf.erase(p);


	//deleting players obj
	delete p;
	for (auto plr = players.begin(); plr != players.end(); plr++) {
		if (*plr == p) {
			players.erase(plr);
			break;
		}
	}
}

void Server::handleMessages() {
	while (true) {
		if (communicationQueue.size() > 0){
			std::string container;
			sf::Packet msg = communicationQueue.pop();
			msg >> container;
			std::cout << "Request: \n";
			std::cout << "\n" << container << "\n";
			try {
				Request req = Request::parse(container);
				if (req.isValid()) req.handle();
			}
			catch (std::invalid_argument &e) {}
			catch (int &e) {}
			catch(...){}
		}
		Sleep(200);
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
		else if (input == "debuginfo") {
			std::cout << "========================DEBUG INFO================================\n";
			std::cout << "matchOf map size: " << matchOf.size() << std::endl;
			std::cout << "gameOf map size: " << gameOf.size() << std::endl;
			std::cout << "playerOf map size: " << playerOf.size() << std::endl;

			std::cout << "players: " << players.size() << std::endl;
			std::cout << "players threads: " << playerThreads.size() << std::endl;
			std::cout << "games: " << gameInstances.size() << std::endl;
			std::cout << "==================================================================\n\n";
		}
		else if (input == "?") {
			std::cout << "========================HELP INFO================================\n";
			std::cout << "? - show this help info \n";
			std::cout << "players - show players list \n";
			std::cout << "players-v - show detailed players list \n";
			std::cout << "games - show games list \n";
			std::cout << "clear - clear screen \n";
			std::cout << "debuginfo - shows server state \n";
			std::cout << "==================================================================\n\n";
		}
		else {
			std::cout << "Wrong command.\n\n";
		}
	}
}

void Server::printASCII() {
	std::cout << R"(                        ___________________  _____ _____ )" << std::endl;
	std::cout << R"(                       / ____| |  | |  ____|/ ____/ ____|)" << std::endl;
	std::cout << R"(                      | |    | |__| | |__  | (___| (___  )" << std::endl;
	std::cout << R"(                      | |    |  __  |  __|  \___ \\___ \ )" << std::endl;
	std::cout << R"(                      | |____| |  | | |____ ____) |___) |)" << std::endl;
	std::cout << R"(                     __\_____|_|_ |_|______|_____/_____/_____ )" << std::endl;
	std::cout << R"(                    / ____|  ____|  __ \ \    / /  ____|  __ \ )" << std::endl;
	std::cout << R"(                   | (___ | |__  | |__) \ \  / /| |__  | |__) |)" << std::endl;
	std::cout << R"(                    \___ \|  __| |  _  / \ \/ / |  __| |  _  / )" << std::endl;
	std::cout << R"(                    ____) | |____| | \ \  \  /  | |____| | \ \ )" << std::endl;
	std::cout << R"(                   |_____/|______|_|  \_\  \/   |______|_|  \_\)" << std::endl;
	std::cout << R"(                   --------------v1.0 by miecha3l--------------)" << std::endl;
	std::cout << R"(                                                               )" << std::endl;
	std::cout << R"(type "?" to see help info                                      )" << std::endl;
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
	std::cout << "=============================" << players.size()<< " ONLINE==============================\n";
	for (auto p : players) {
		std::cout << "Player: " << p->getId() << std::endl;
	}
	std::cout << "==================================================================\n\n";
}

void Server::printPlayerListWithInfo()
{
	std::cout << "=============================" << players.size() << " ONLINE==============================\n";
	for (auto p : players) {
		std::string status;
		std::cout << "-----------------------------------------------\n";
		std::cout << "Player: " << p->getId() << std::endl;
		switch (p->getPlayersStatus()) {
		case Player::Status::InGame:
			status = "in game";
			std::cout << "Player is: " << status << " of id: " << gameOf[p]->getId() << std::endl;
			break;

		case Player::Status::Idle:
			status = "idle";
			break;

		case Player::Status::InLobbyNotReady:
			status = "inLobbyNotReady";
			break;

		case Player::Status::InLobbyReady:
			status = "inLobbyNotReady";
			break;

		default: 
			status = "unknown";
			break;
		}
		std::cout << "Player is: " << status << std::endl;
		if (matchOf[p] != NULL) {
			std::cout << "Players match: " << matchOf[p]->getId() << std::endl;
		}
		else std::cout << "Player doesnt have a match" << std::endl;
		
		std::cout << "-----------------------------------------------\n";
	}
	std::cout << "==================================================================\n\n";
}

void Server::printGameList()
{
	std::cout << "=============================" << gameInstances.size() << " GAMES==============================\n";
	for (auto g : gameInstances) {
		std::cout << "-----------------------------------------------\n";
		std::cout << "Game: " << g->getId() << std::endl;
		std::cout << "Players: \n" << g->getBlack()->getId() << "(black)\n" << g->getWhite()->getId() << "(white)\n";
		std::cout << "-----------------------------------------------\n";
	}
	std::cout << "==================================================================\n\n";
}


int Server::playersCount() {
	return players.size();
}

int Server::messagesCount() {
	return communicationQueue.size();
}

Player* Server::getPlayer(int id) {
	auto outcome = playerOf.find(id);
	if (outcome == playerOf.end()) {
		return NULL;
	}
	else return outcome->second;
}

Player *Server::getPlayerMatch(Player *p) {
	auto outcome = matchOf.find(p);
	if (outcome == matchOf.end()) {
		return NULL;
	}
	else return outcome->second;
}

sf::Thread* Server::getThreadOf(Player*p) {
	auto outcome = threadOf.find(p);
	if (outcome == threadOf.end()) {
		return NULL;
	}
	else return outcome->second;
}

void Server::matchPlayers(int a, int b){
	matchOf[getPlayer(a)] = getPlayer(b);
	matchOf[getPlayer(b)] = getPlayer(a);
}

std::list<Player*> Server::getPlayersList() {
	return players;
}

Server::~Server()
{
}
