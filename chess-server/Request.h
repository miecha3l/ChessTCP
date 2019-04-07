#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
class Request
{
public:
	enum Type {
		REQUEST,
		MESSAGE,
		OTHER
	};

	Request(Type, std::string, std::string, std::string);
	//Request(std::string);
	~Request();
	void handle();

private:
	Type type;
	std::string content;
	int sender;
	int receiver;
};

