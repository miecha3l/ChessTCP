#include "Server.h"
#include "Windows.h"
#include "GameInstance.h"
#include <list>

Server* Server::obj = NULL;
extern Queue communicationQueue;
std::list<GameInstance*> gameInstances;
std::list<Board*> boards;
std::list<sf::Thread*> threads;
std::map<GameInstance*, Board*> boardOf;

void createClassicSet(std::string color, Board &b) {
	if (color == "white") {
		for (int i = 0; i < 8; ++i) {
			b.addPiece((new Pawn(i, 6, "white")));
		}
		b.addPiece(new Rook(0, 7, "white"));
		b.addPiece(new Rook(7, 7, "white"));
		b.addPiece(new Knight(1, 7, "white"));
		b.addPiece(new Knight(6, 7, "white"));
		b.addPiece(new Bishop(2, 7, "white"));
		b.addPiece(new Bishop(5, 7, "white"));
		b.addPiece(new Queen(3, 7, "white"));
		b.addPiece(new King(4, 7, "white"));
	}
	else if (color == "black") {
		for (int i = 0; i < 8; ++i) {
			b.addPiece((new Pawn(i, 1, "black")));
		}
		b.addPiece(new Rook(0, 0, "black"));
		b.addPiece(new Rook(7, 0, "black"));
		b.addPiece(new Knight(1, 0, "black"));
		b.addPiece(new Knight(6, 0, "black"));
		b.addPiece(new Bishop(2, 0, "black"));
		b.addPiece(new Bishop(5, 0, "black"));
		b.addPiece(new Queen(3, 0, "black"));
		b.addPiece(new King(4, 0, "black"));
	}
	else {
		std::cout << "Invalid color";
	}
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
			if (type == "req") {
				if (data == "match") {
					if (to != "none") {
						int fromId = std::stoi(from);
						int toId = std::stoi(to);
						matchPlayers(fromId, toId);
						std::string msg = "match completed";
						sf::Packet response;
						response << msg;
						getPlayer(fromId)->getClient()->send(response);
						getPlayer(toId)->getClient()->send(response);
					}
				}
				else if (data == "play") {
					if (to != "none" && to != "match") {
						int toId = std::stoi(to);
						int fromId = std::stoi(from);

						getPlayer(fromId)->setPlayersStatus(Player::Status::InGame);
						getPlayer(toId)->setPlayersStatus(Player::Status::InGame);

						boards.push_back(new Board());
						createClassicSet("white", *boards.back());
						createClassicSet("black", *boards.back());
						gameInstances.push_back(new GameInstance(getPlayer(fromId)->getClient(), getPlayer(toId)->getClient(), boards.back()));
						boardOf[gameInstances.back()] = boards.back();
						threads.push_back(new sf::Thread(&GameInstance::play, gameInstances.back()));

						sf::Packet resp;
						resp << "white";
						getPlayer(fromId)->getClient()->send(resp);

						resp.clear();
						resp << "black";
						getPlayer(toId)->getClient()->send(resp);

						threads.back()->launch();
					}
					else if (to == "match") {
						int fromId = std::stoi(from);
						Player *fromPlr = getPlayer(fromId);
					}
				}
			}
			else if (type == "msg") {
				if (to != "none" && to != "match") {
					int toId = std::stoi(to);
					int fromId = std::stoi(from);
					sf::Packet cont;
					cont << data;
					getPlayer(toId)->getClient()->send(cont);
				}
				else if (to == "match") {
					int fromId = std::stoi(from);
					Player *fromPlr = getPlayer(fromId);
					sf::Packet cont;
					cont << data;
					getPlayerMatch(fromPlr)->getClient()->send(cont);
				}
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
