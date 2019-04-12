#include "Response.h"

Response::Response(Type t, std::string cont)
	:type(t), content(cont)
{
}

std::string Response::handle() {
	return content;
}

Response Response::parse(std::string container) {
	std::string sType;
	Type eType;
	std::string contnt;
	int slashes = 0;
	for (char c : container) {
		if (c == '/') {
			slashes++;
			continue;
		}
		if (slashes == 0) sType.push_back(c);
		else if (slashes == 1) contnt.push_back(c);
	}

	if (sType == "match") eType = Type::Match;
	else if (sType == "game_init") eType = Type::GameInit;
	else if (sType == "game_update") eType = Type::GameUpdate;
	else if (sType == "game_over") eType = Type::GameOver;
	else if (sType == "plist") eType = Type::Plist;
	else if (sType == "notification") eType = Type::Notification;
	else eType = Type::Other;

	return Response(eType, contnt);
}

Response::Type Response::getType()
{
	return type;
}

bool Response::isValid()
{
	return !(content.empty());
}

Response::~Response()
{
}
