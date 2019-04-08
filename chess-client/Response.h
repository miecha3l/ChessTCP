#pragma once
#include <iostream>
#include <string>
class Response
{
public:
	static enum Type {
		Plist,
		GameInit,
		Match,
		GameUpdate,
		Other
	};
	Response(Type, std::string);
	std::string handle();
	static Response parse(std::string);
	Type getType();
	bool isValid();
	~Response();

private:
	Type type;
	std::string content;
};

