#include "Request.h"
#include "Server.h"
#include "GameInstance.h"

extern std::list<GameInstance*> gameInstances;
extern std::map<Player*, GameInstance*> gameOf;

Request::Request(Type t, std::string cont, std::string s, std::string r)
	: type(t), content(cont), sender(std::stoi(s)), receiver(NULL)
{
	if (r == "match") {
		receiver = Server::instance()->getPlayerMatch(Server::instance()->getPlayer(sender))->getId();
	}
	else if (r == "server") {
		receiver = 0;
	}
	else {
		receiver = std::stoi(r);
	}
}

void Request::handle() {
	if (type == Type::REQUEST) {
		if(content == "match"){
			if (Server::instance()->getPlayerMatch(Server::instance()->getPlayer(sender)) == NULL) {
				std::string msg = "match/";
				msg.append(std::to_string(sender));
				sf::Packet response;
				response << msg;
				Server::instance()->getPlayer(receiver)->getClient()->send(response);
			}
		}
		else if (content == "match_acc") {
			if (Server::instance()->getPlayerMatch(Server::instance()->getPlayer(sender)) == NULL) {
				Server::instance()->matchPlayers(sender, receiver);
				sf::Packet resp;
				resp << "match/acc";
				Server::instance()->getPlayer(receiver)->getClient()->send(resp);
			}
		}
		else if (content == "match_dec") {
			sf::Packet resp;
			resp << "match/dec";
			Server::instance()->getPlayer(receiver)->getClient()->send(resp);
		}
		else if (content == "is_ready" || content == "is_notready") {
			sf::Packet resp;
			std::string cont = "notification/";
			resp << cont.append(content);
			Server::instance()->getPlayer(receiver)->getClient()->send(resp);
		}
		else if (content == "play") {
			if (Server::instance()->getPlayerMatch(Server::instance()->getPlayer(sender))->getId() == receiver) {
				Server::instance()->getPlayer(sender)->setPlayersStatus(Player::Status::InGame);
				Server::instance()->getPlayer(receiver)->setPlayersStatus(Player::Status::InGame);

				gameInstances.push_back(new GameInstance(Server::instance()->getPlayer(sender), Server::instance()->getPlayer(receiver)));
				gameOf[Server::instance()->getPlayer(sender)] = gameInstances.back();
				gameOf[Server::instance()->getPlayer(receiver)] = gameInstances.back();

				std::string initGs = gameInstances.back()->getCurrentGameState().parseGameStateToString();
				sf::Packet resp;
				int ran = rand() % 2;
				std::string colorA = ran == 1 ?"game_init/white\\" : "game_init/black\\";
				std::string colorB = ran == 1 ?"game_init/black\\" : "game_init/white\\";
				resp << colorA.append(initGs);
				Server::instance()->getPlayer(sender)->getClient()->send(resp);
				resp.clear();
				colorA.clear();

				resp << colorB.append(initGs);
				Server::instance()->getPlayer(receiver)->getClient()->send(resp);
				resp.clear();
				colorB.clear();
			}
		}
		else if (content == "get_plist") {
			std::string response = "plist/";
			sf::Packet resp;
			for (auto p : Server::instance()->getPlayersList()) {
				response.append(std::to_string(p->getId()).append(";"));
			}

			resp << response;
			Server::instance()->getPlayer(sender)->getClient()->send(resp);
		}
	}
	else if (type == Type::MESSAGE) {
		sf::Packet cont;
		std::string response = "message/";
		response.append(std::to_string(sender)).append(";").append(content);
		cont << response;
		Server::instance()->getPlayer(receiver)->getClient()->send(cont);
	}
	else if (type == Type::GAME_REQ) {
		if (Server::instance()->getPlayer(receiver) != NULL) {
			GameState updatedGs = gameOf[Server::instance()->getPlayer(sender)]->updateGameInstance(std::stoi(content));
			sf::Packet cont;
			std::string response = "game_update/";
			cont << response.append(updatedGs.parseGameStateToString());
			Server::instance()->getPlayer(sender)->getClient()->send(cont);
			Server::instance()->getPlayer(receiver)->getClient()->send(cont);
		}
		else {
			sf::Packet cont;
			std::cout << "game_over/enemy_disconnected" << std::endl;
			cont << "game_over/enemy_disconnected";
			Server::instance()->getPlayer(sender)->setPlayersStatus(Player::Status::Idle);
			Server::instance()->getPlayer(sender)->getClient()->send(cont);
		}
	}
}

Request Request::parse(std::string container)
{
	std::string type;
	std::string from;
	std::string to;
	std::string data;
	Type eType;
	int slashes = 0;
	for (auto c : container) {
		if (c == '/') {
			slashes++;
			continue;
		}
		if (slashes == 0) type.push_back(c);
		else if (slashes == 1) data.push_back(c);
		else if (slashes == 2) to.push_back(c);
		else if (slashes == 3) from.push_back(c);
	}

	if (type == "req") eType = Request::REQUEST;
	else if (type == "msg") eType = Request::MESSAGE;
	else if (type == "game_req") eType = Request::GAME_REQ;
	else eType = Request::OTHER;

	return Request(eType, data, from, to);
}

bool Request::isValid() {
	return !(content.empty());
}

Request::~Request()
{
}
