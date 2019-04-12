#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <list>

template<typename T>
class Queue {
private:
	std::list<T> message;

public:
	T pop() {
		T pop = message.back();
		message.pop_back();
		return pop;
	}
	void push(T packet) {
		message.push_back(packet);
	}
	int size() { return message.size(); }
};