#include "Client.h"
#include "Response.h"
#include <Windows.h>
#include "gui.h"
Client* Client::obj = NULL;
extern sf::RenderWindow client;

Client::Client()
{
	ip = "127.0.0.1";
	port = 8888;
	color = "";
	matchName = "";
	gameOverReason = "";
}

Client* Client::instance() {
	if (obj == NULL) obj = new Client();
	else return obj;
}

void Client::init() {
	sf::Thread send(&Client::sendRequest, this);
	sf::Thread receive(&Client::receiveResponse, this);
	sf::Thread handle(&Client::handleResponse, this);

	sf::Packet initial;
	std::cout << "connecting with server at: " << ip << std::endl;
	serverConnection.connect(ip, port);
	serverConnection.receive(initial);
	initial >> playerName;
	std::cout << "connection accomplished" << std::endl;
	serverConnection.setBlocking(false);

	send.launch();
	receive.launch();
	handle.launch();
}
std::string Client::getColor() {
	return color;
}
std::string Client::getName() {
	return playerName;
}
std::string Client::getMatchName() {
	return matchName;
}

void Client::sendRequest() {
	while (true) {
		if (requests.size() > 0) {
			sf::Packet req;
			std::string raw = requests.pop();
			std::cout << "Req: " << raw << std::endl;
			req << raw;
			serverConnection.send(req);
			Sleep(250);
			GuiManager::instance()->setIsReady(true);
		}
		Sleep(250);
	}
}

void Client::receiveResponse() {
	while (true) {
		sf::Packet response;
		serverConnection.receive(response);
		if (response.getDataSize() > 0) responses.push(response);
		Sleep(250);
	}
}

void Client::handleResponse() {
	while (true) {
		if (responses.size() > 0) {
			std::string resp;
			responses.pop() >> resp;
			Response response = Response::parse(resp);
			switch (response.getType()) {
			case Response::Type::GameInit:
				for (int i = 0; i < 5; ++i) color.push_back(response.handle()[i]);
				for (int i = 6; i < response.handle().size(); i++) initialGs.push_back(response.handle()[i]);
				gs = GameState(initialGs);
				inGame = true;
				serverConnection.setBlocking(true);
				break;

			case Response::Type::GameUpdate:
				gs = GameState(response.handle());
				break;

			case Response::Type::GameOver:
				gameOverReason = response.handle();
				inGame = false;
				gs = GameState("");
				matchName.clear();
				isMatchReady = false;
				isReady = false;
				color.clear();
				currentScreen = Screen::Menu;
				GuiManager::instance()->setMenuUI(true);
				GuiManager::instance()->setLobbyUI(false);
				serverConnection.setBlocking(false);
				gameOverReason.clear();
				break;

			case Response::Type::Match:
				if (response.handle() != "acc" && response.handle() != "dec") {
					matchReq = true;
					GuiManager::instance()->highlightPendingButton();
					requester = response.handle();
				}
				if (response.handle() == "acc") {
					if (matchName.empty()) {
						std::cout << "match accepted" << std::endl;
						matchReq = false;
						matchName = requester;
						GuiManager::instance()->setInfoBoardInfo();
						requester.clear();
					}
				}
				if (response.handle() == "dec") {
					matchReq = false;
					std::cout << "match declined" << std::endl;
					requester.clear();
				}
				
				break;

			case Response::Type::Notification:
				if (response.handle() == "is_notready") {
					isMatchReady = false;
					GuiManager::instance()->setInfoBoardInfo();
				}
				else if (response.handle() == "is_ready") {
					isMatchReady = true;
					GuiManager::instance()->setInfoBoardInfo();
				}
				break;

			case Response::Type::Plist:
				onlinePlayers.clear();
				std::string entirelist = response.handle();
				std::string player = "";
				for (char c : entirelist) {
					if (c == ';') {
						onlinePlayers.push_back(player);
						player.clear();
						continue;
					}
					player.push_back(c);
				}
				onlinePlayersUpToDate = true;
				break;
			}	
		}
		Sleep(250);
	}
}

void Client::addReqToQueue(std::string req) {
	requests.push(req);
}

GameState Client::getGameState() {
	return gs;
}

void Client::setGameState(Board b)
{
	gs = GameState(b);
}

void Client::resetSoloGame()
{
	game = SoloGame();
}

SoloGame *Client::getSoloGameInstance()
{
	SoloGame *instance = &game;
	return instance;
}

void Client::initSoloGame()
{
	game = SoloGame(color);
}

bool Client::isInGame() { return inGame; }

std::list<std::string> Client::getOnlinePlayersList()
{
	return onlinePlayers;
}

void Client::setOnlinePlayersListUpToDate(bool s)
{
	onlinePlayersUpToDate = s;
}

bool Client::isPlayersListUpToDate()
{
	return onlinePlayersUpToDate;
}

bool Client::isMatchReqPending()
{
	return matchReq;
}

std::string Client::getRequester()
{
	return requester;
}

void Client::setMatchAcc(bool s)
{
	matchAcc = s;
}

void Client::setMatchReq(bool s)
{
	matchReq = s;
}

void Client::setMatchName(std::string n)
{
	matchName = n;
}

void Client::setRequester(std::string r)
{
	requester = r;
}

void Client::setIsReady(bool s)
{
	isReady = s;
}

bool Client::isPlayerReady()
{
	return isReady;
}

void Client::setIsMatchReady(bool s)
{
	isMatchReady = s;
}

bool Client::isPlayersMatchReady()
{
	return isMatchReady;
}

Client::Screen Client::getCurrentScreen()
{
	return currentScreen;
}

void Client::setCurrentScreen(Client::Screen s)
{
	currentScreen = s;
}

void Client::setColor(std::string c)
{
	color = c;
}
