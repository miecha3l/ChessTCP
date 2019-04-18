#include "Response.h"
#include "gui.h"
#include "Client.h"

Response::Response(Type t, std::string cont)
	:type(t), content(cont)
{
}

void Response::handle() {
	std::string tempcolor, tempgs;
	switch (this->type) {
	case Response::Type::GameInit:
		GuiManager::instance()->setDrawLock(true);
		GuiManager::instance()->setLobbyUI(false);
		GuiManager::instance()->setMultiGameUI(true);
		
		for (int i = 0; i < 5; ++i) tempcolor.push_back(this->content[i]);
		for (int i = 6; i < this->content.size(); i++) tempgs.push_back(this->content[i]);
		Client::instance()->setGameState(GameState(tempgs));
		Client::instance()->setColor(tempcolor);
		Client::instance()->setCurrentScreen(Client::Screen::OnlineGame);
		Client::instance()->setInGame(true);
		Client::instance()->setServerConnection(true);

		GuiManager::instance()->setDrawLock(false);
		break;

	case Response::Type::GameUpdate:
		Client::instance()->setGameState(GameState(this->content));
		if (Client::instance()->getGameState().getCurrentFlag() == "winner_white" || Client::instance()->getGameState().getCurrentFlag() == "winner_black") {
			std::string message = Client::instance()->getGameState().getCurrentFlag() == "winner_white" ? "White won!" : "Black won!";
			GuiManager::instance()->displayMessage(message);
			Client::instance()->setInGame(false);
			Client::instance()->setColor("");
		}
		GuiManager::instance()->setCurrentTurnLabel(Client::instance()->getGameState().getCurrentGameTurn());
		break;

	case Response::Type::GameOver:
		GuiManager::instance()->setDrawLock(true);
		Client::instance()->setGameOverReason(this->content);
		Client::instance()->setGameState(GameState(""));
		Client::instance()->setInGame(false);
		Client::instance()->setMatchName("");
		if (Client::instance()->getGameOverReason() == "enemy_disconnected") {
			Client::instance()->setCurrentScreen(Client::Screen::Lobby);
			GuiManager::instance()->setMultiGameUI(false);
			GuiManager::instance()->setLobbyUI(false, true);
			GuiManager::instance()->displayMessage("Enemy left");
		}

		else if (Client::instance()->getGameOverReason() == "you_disconnected") {
			Client::instance()->setCurrentScreen(Client::Screen::Menu);
			GuiManager::instance()->setMultiGameUI(false);
			GuiManager::instance()->setMenuUI(true);
		}


		Client::instance()->setColor("");
		Client::instance()->setServerConnection(false);
		Client::instance()->setGameOverReason("");
		GuiManager::instance()->setDrawLock(false);
		break;

	case Response::Type::Match:
		if (this->content != "acc" && this->content != "dec") {
			Client::instance()->setMatchReq(true);
			GuiManager::instance()->highlightPendingButton();
			Client::instance()->setRequester(this->content);
		}
		if (this->content == "acc") {
			if (Client::instance()->getMatchName().empty()) {
				GuiManager::instance()->setDrawLock(true);

				Client::instance()->setMatchReq(false);
				Client::instance()->setMatchName(Client::instance()->getRequester());
				Client::instance()->setRequester("");
				GuiManager::instance()->setInfoBoardInfo();
				while (!GuiManager::instance()->mainUIHandleFree()) {}

				GuiManager::instance()->setDrawLock(false);
			}
		}
		if (this->content == "dec") {
			GuiManager::instance()->setDrawLock(true);
			Client::instance()->setMatchReq(false);
			Client::instance()->setRequester("");
			GuiManager::instance()->displayMessage("Match declined");
			GuiManager::instance()->setDrawLock(false);
		}

		break;

	case Response::Type::Notification:
		if (this->content == "unmatch") {
			GuiManager::instance()->setDrawLock(true);
			Client::instance()->setMatchName("");
			Client::instance()->setMatchReq(false);
			GuiManager::instance()->setInfoBoardInfo();
			GuiManager::instance()->setDrawLock(false);
		}
		break;

	case Response::Type::Plist:
		Client::instance()->setOnlinePlayers(std::list<std::string>());
		std::string entirelist = this->content;
		std::string player = "";
		for (char c : entirelist) {
			if (c == ';') {
				Client::instance()->addOnlinePlayer(player);
				player.clear();
				continue;
			}
			player.push_back(c);
		}
		Client::instance()->setOnlinePlayersU2D(true);
		break;
	}
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
