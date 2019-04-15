#include "Request.h"
#include "Server.h"
#include "GameInstance.h"

extern std::list<GameInstance*> gameInstances;
extern std::map<Player*, GameInstance*> gameOf;


GameInstance* GetGameOf(Player*p) {
	auto outcome = gameOf.find(p);
	if (outcome == gameOf.end()) {
		return NULL;
	}
	else return outcome->second;
}

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
		if (Server::instance()->getPlayer(receiver) == NULL) throw 10;
		if (sender == receiver) throw 11;
		if (receiver < 1000) throw 12;
	}
}

void Request::handle() {
	if (type == Type::REQUEST) {

		if(content == "match"){
			if (Server::instance()->getPlayerMatch(Server::instance()->getPlayer(sender)) == NULL && Server::instance()->getPlayerMatch(Server::instance()->getPlayer(receiver)) == NULL) {
 
				std::string msg = "match/";
				msg.append(std::to_string(sender));
				sf::Packet response;
				response << msg;
				Server::instance()->getPlayer(receiver)->getClient()->send(response);
			}
		}

		else if (content == "match_acc") {
			if (Server::instance()->getPlayerMatch(Server::instance()->getPlayer(sender)) == NULL && Server::instance()->getPlayerMatch(Server::instance()->getPlayer(receiver)) == NULL ) {
				Server::instance()->matchPlayers(sender, receiver);
				Server::instance()->getPlayer(sender)->setPlayersStatus(Player::Status::InLobbyReady);
				Server::instance()->getPlayer(receiver)->setPlayersStatus(Player::Status::InLobbyReady);
				sf::Packet resp;
				resp << "match/acc";
				Server::instance()->getPlayer(receiver)->getClient()->send(resp);
				Server::instance()->getPlayer(sender)->getClient()->send(resp);
			}
		}

		else if (content == "match_dec") {
			sf::Packet resp;
			resp << "match/dec";
			Server::instance()->getPlayer(receiver)->getClient()->send(resp);
		}

		else if (content == "unmatch") {
			if (Server::instance()->getPlayer(sender)->getPlayersStatus() != Player::Status::InGame) {
				Server::instance()->removeMatch(sender);
				Server::instance()->removeMatch(receiver);
				sf::Packet resp;
				resp << "notification/unmatch";
				Server::instance()->getPlayer(sender)->getClient()->send(resp);
				Server::instance()->getPlayer(receiver)->getClient()->send(resp);
			}
		}

		else if (content == "play") {
			if (Server::instance()->getPlayerMatch(Server::instance()->getPlayer(sender))->getId() == receiver && 
				Server::instance()->getPlayer(sender)->getPlayersStatus() == Player::Status::InLobbyReady && Server::instance()->getPlayer(receiver)->getPlayersStatus() == Player::Status::InLobbyReady) {
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
			else {
				sf::Packet pckt;
				pckt << "notification/not_ok";
				Server::instance()->getPlayer(sender)->getClient()->send(pckt);
			}
		}

		else if (content == "status_inlobby") {
			Server::instance()->getPlayer(sender)->setPlayersStatus(Player::Status::InLobbyReady);
		}

		else if (content == "disconnect") {
			if (Server::instance()->getPlayer(sender)->getPlayersStatus() == Player::Status::InGame) {
				GetGameOf(Server::instance()->getPlayer(sender))->disconnectPlayer(Server::instance()->getPlayer(sender));
				GetGameOf(Server::instance()->getPlayer(receiver))->disconnectPlayer(Server::instance()->getPlayer(sender));
				Server::instance()->getPlayer(sender)->setPlayersStatus(Player::Status::Idle);
				Server::instance()->getPlayer(receiver)->setPlayersStatus(Player::Status::Idle);
				Server::instance()->removeMatch(sender);
				Server::instance()->removeMatch(receiver);
				sf::Packet resp;
				resp << "game_over/enemy_disconnected";
				Server::instance()->getPlayer(receiver)->getClient()->send(resp);

				resp.clear();
				resp << "game_over/you_disconnected";
				Server::instance()->getPlayer(sender)->getClient()->send(resp);

				gameInstances.remove(GetGameOf(Server::instance()->getPlayer(sender)));
				delete GetGameOf(Server::instance()->getPlayer(sender));
				gameOf.erase(Server::instance()->getPlayer(sender));
				gameOf.erase(Server::instance()->getPlayer(receiver));
			}
		}

		else if (content == "forfeit") {
			if (Server::instance()->getPlayer(sender)->getPlayersStatus() == Player::Status::InGame) {
				GetGameOf(Server::instance()->getPlayer(sender))->disconnectPlayer(Server::instance()->getPlayer(sender));
				GetGameOf(Server::instance()->getPlayer(receiver))->disconnectPlayer(Server::instance()->getPlayer(sender));
				Server::instance()->getPlayer(sender)->setPlayersStatus(Player::Status::Idle);
				Server::instance()->getPlayer(receiver)->setPlayersStatus(Player::Status::Idle);

				sf::Packet resp;
				resp << "game_over/enemy_forfeited";
				Server::instance()->getPlayer(receiver)->getClient()->send(resp);

				resp.clear();
				resp << "game_over/you_forfeited";
				Server::instance()->getPlayer(sender)->getClient()->send(resp);

				gameInstances.remove(GetGameOf(Server::instance()->getPlayer(sender)));
				delete GetGameOf(Server::instance()->getPlayer(sender));
				gameOf.erase(Server::instance()->getPlayer(sender));
				gameOf.erase(Server::instance()->getPlayer(receiver));
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
			if (updatedGs.getCurrentFlag() == "winner_white" || updatedGs.getCurrentFlag() == "winner_black") {
				delete GetGameOf(Server::instance()->getPlayer(sender));
				gameInstances.remove(GetGameOf(Server::instance()->getPlayer(sender)));
				gameOf.erase(Server::instance()->getPlayer(sender));
				gameOf.erase(Server::instance()->getPlayer(receiver));
			}
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
