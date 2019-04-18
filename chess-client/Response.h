#pragma once
#include <iostream>
#include <string>
class Response
{
public:
	enum Type {
		Plist,
		GameInit,
		Match,
		GameUpdate,
		GameOver,
		Notification,
		Other
	};
	Response(Type, std::string);
	void handle();
	static Response parse(std::string);
	Type getType();
	bool isValid();
	~Response();

private:
	Type type;
	std::string content;
};

