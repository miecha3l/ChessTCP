#pragma once
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
class Request
{
public:
	enum Type {
		REQUEST,
		GAME_REQ,
		MESSAGE,
		OTHER
	};

	Request(Type, std::string, std::string, std::string);
	~Request();
	void handle();
	static Request parse(std::string);
	bool isValid();

private:
	Type type;
	std::string content;
	int sender;
	int receiver;
};

