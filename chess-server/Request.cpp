#include "Request.h"
#include "Server.h"
#include "GameInstance.h"

extern std::list<GameInstance*> gameInstances;
extern std::list<Board*> boards;
extern std::map<GameInstance*, Board*> boardOf;
extern std::map<Player*, GameInstance*> gameOf;

Request::Request(Type t, std::string cont, std::string s, std::string r)
	: type(t), content(cont), sender(std::stoi(s)), receiver(NULL)
{
	if (r == "match") {
		receiver = Server::instance()->getPlayerMatch(Server::instance()->getPlayer(sender))->getId();
	}
	else {
		receiver = std::stoi(r);
	}
}

void Request::handle() {
	if (type == Type::REQUEST) {
		if(content == "match"){
			Server::instance()->matchPlayers(sender, receiver);
			std::string msg = "matched/";
			msg.append(std::to_string(receiver));
			sf::Packet response;
			response << msg;
			Server::instance()->getPlayer(sender)->getClient()->send(response);
			response.clear();
			msg.clear();

			msg = "matched/";
			msg.append(std::to_string(sender));
			Server::instance()->getPlayer(receiver)->getClient()->send(response);
			response.clear();
			msg.clear();
		}
		else if (content == "play") {
			if (Server::instance()->getPlayerMatch(Server::instance()->getPlayer(sender))->getId() == receiver) {
				Server::instance()->getPlayer(sender)->setPlayersStatus(Player::Status::InGame);
				Server::instance()->getPlayer(receiver)->setPlayersStatus(Player::Status::InGame);

				boards.push_back(new Board());
				boards.back()->createClassicSet("white");
				boards.back()->createClassicSet("black");
				gameInstances.push_back(new GameInstance(Server::instance()->getPlayer(sender), Server::instance()->getPlayer(receiver), boards.back()));
				boardOf[gameInstances.back()] = boards.back();
				gameOf[Server::instance()->getPlayer(sender)] = gameInstances.back();
				gameOf[Server::instance()->getPlayer(receiver)] = gameInstances.back();

				std::string initGs = gameInstances.back()->getCurrentGameState().parseGameStateToString();
				sf::Packet resp;
				std::string color = "white/";
				resp << color.append(initGs);
				Server::instance()->getPlayer(sender)->getClient()->send(resp);
				resp.clear();
				color.clear();

				color = "black/";
				resp << color.append(initGs);
				Server::instance()->getPlayer(receiver)->getClient()->send(resp);
				resp.clear();
				color.clear();
			}
		}
	}
	else if (type == Type::MESSAGE) {
		sf::Packet cont;
		cont << content;
		Server::instance()->getPlayer(receiver)->getClient()->send(cont);
	}
	else if (type == Type::GAME_REQ) {
		GameState updatedGs = gameOf[Server::instance()->getPlayer(sender)]->updateGameInstance(std::stoi(content));
		sf::Packet cont;
		cont << updatedGs.parseGameStateToString();
		Server::instance()->getPlayer(sender)->getClient()->send(cont);
		Server::instance()->getPlayer(receiver)->getClient()->send(cont);
	}
}

Request::~Request()
{
}
