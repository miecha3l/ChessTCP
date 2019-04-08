#include "Queue.h"

sf::Packet Queue::pop() {
	if (message.size() > 0) {
		sf::Packet toReturn = message.back();
		message.pop_back();
		return toReturn;
	}
	else return sf::Packet();
}

void Queue::push(sf::Packet packet){
	message.push_back(packet);
}

int Queue::size(){
	return message.size();
}