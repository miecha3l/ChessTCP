#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <list>

class Queue {
private:
	std::list<sf::Packet> message;

public:
	sf::Packet pop();
	void push(sf::Packet packet);
	int size();
};