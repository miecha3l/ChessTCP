#include "Response.h"

Response::Response(Type t, std::string cont)
	:type(t), content(cont)
{
}

std::string Response::handle() {
	if (type == Type::Match) {
		std::string matchName;
		bool match = false;
		for (char c : content) {
			if (c == '/') {
				match = true;
				continue;
			}
			else if (match) {
				matchName.push_back(c);
			}
		}

		return matchName;
	}

	else if (type == Type::GameInit) {
		return content;
	}
	else if (type == Type::GameUpdate) {
		return content;
	}

	return "";
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

	if (sType == "matched") eType = Type::Match;
	else if (sType == "game_init") eType = Type::GameInit;
	else if (sType == "game_update") eType = Type::GameUpdate;
	else if (sType == "plist") eType = Type::Plist;
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
