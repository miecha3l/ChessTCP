#include "Server.h"
#include "Windows.h"
#include "GameInstance.h"
#include <list>
#include "Request.h"
Server* Server::obj = NULL;
std::list<GameInstance*> gameInstances;
std::list<Board*> boards;
std::list<sf::Thread*> threads;
std::map<GameInstance*, Board*> boardOf;
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

		std::cout << "New player has connected! Players online: " << players.size() << std::endl;

		sf::Packet welcomemsg;
		welcomemsg << std::to_string(players.back()->getId());
		players.back()->getClient()->send(welcomemsg);
		client = new sf::TcpSocket();
	}
}

void Server::updatePlayersList() {
	while (true) {
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

				threadOf.erase(plr);
				matchOf.erase(matchOf[plr]);
				matchOf.erase(plr);
				playerOf.erase(plr->getId());

				//remove player obj
				for (auto p = players.begin(); p != players.end(); p++) {
					if (*p == plr) {
						delete *p;
						players.erase(p);
						deleted = true;
						break;
					}
				}
				if (deleted) std::cout << "Player has disconnected! Players online: " << players.size() << std::endl;
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
			std::string type;
			std::string from;
			std::string to;
			std::string data;
			
			Request::Type eType;
			msg >> container;
			int slashes = 0;
			for (auto c : container) {
				if (c == '/') {
					slashes++;
					continue;
				}
				if (slashes == 0) from.push_back(c);
				else if (slashes == 1) to.push_back(c);
				else if (slashes == 2) type.push_back(c);
				else if (slashes == 3) data.push_back(c);
			}

			if (type == "req") eType = Request::REQUEST;
			else if (type == "msg") eType = Request::MESSAGE;
			else eType = Request::OTHER;

			if (!data.empty() && !from.empty() && !to.empty()) {
				Request req = Request(eType, data, from, to);
				req.handle();
			}
		}
	}
}

void Server::init() {
	sf::Thread accept(&Server::acceptClients, this);
	sf::Thread update(&Server::updatePlayersList, this);
	sf::Thread handleMsg(&Server::handleMessages, this);

	accept.launch();
	update.launch();
	handleMsg.launch();
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
