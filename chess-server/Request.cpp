#include "Request.h"
#include "Server.h"
#include "GameInstance.h"

extern std::list<GameInstance*> gameInstances;
extern std::list<Board*> boards;
extern std::list<sf::Thread*> threads;
extern std::map<GameInstance*, Board*> boardOf;

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
			std::string msg = "match completed";
			sf::Packet response;
			response << msg;
			Server::instance()->getPlayer(sender)->getClient()->send(response);
			Server::instance()->getPlayer(receiver)->getClient()->send(response);
		}
		else if (content == "play") {
			Server::instance()->getPlayer(sender)->setPlayersStatus(Player::Status::InGame);
			Server::instance()->getPlayer(receiver)->setPlayersStatus(Player::Status::InGame);

			boards.push_back(new Board());
			boards.back()->createClassicSet("white");
			boards.back()->createClassicSet("black");
			gameInstances.push_back(new GameInstance(Server::instance()->getPlayer(sender), Server::instance()->getPlayer(receiver), boards.back()));
			boardOf[gameInstances.back()] = boards.back();
			threads.push_back(new sf::Thread(&GameInstance::play, gameInstances.back()));

			sf::Packet resp;
			resp << "white";
			Server::instance()->getPlayer(sender)->getClient()->send(resp);

			resp.clear();
			resp << "black";
			Server::instance()->getPlayer(receiver)->getClient()->send(resp);

			threads.back()->launch();
		}
	}
	else if (type == Type::MESSAGE) {
		sf::Packet cont;
		cont << content;
		Server::instance()->getPlayer(receiver)->getClient()->send(cont);
	}
}

Request::~Request()
{
}
