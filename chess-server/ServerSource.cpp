#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Chess.h"
#include "GameInstance.h"
#include "Server.h"

//std::list<Board*> boards;
//std::list<GameInstance*> gameInstances;
//std::list<sf::Thread*> threads;
//std::map<GameInstance*, Board*> boardOf;

int main() {
	srand(time(NULL));
	Server::instance()->init();
	return 0;
}

