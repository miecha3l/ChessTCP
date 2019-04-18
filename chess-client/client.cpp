#include "Client.h"
#include "Response.h"
#include <chrono>
#include <thread>
#include "gui.h"
#include <fstream>

Client* Client::obj = NULL;
extern sf::RenderWindow client;

Client::Client()
{
	port = 8888;
	readConfigFile("assets/config.txt");
}

Client* Client::instance() {
	if (obj == NULL) obj = new Client();
	else return obj;
}

void Client::readConfigFile(std::string filepath) {
	std::ifstream configFile(filepath);
	std::string configRaw((std::istreambuf_iterator<char>(configFile)), std::istreambuf_iterator<char>());
	
	std::string token, value;
	bool readingToken = true, readingValue = false;
	for (char character : configRaw) {
		if (character == '=') {
			readingToken = false;
			readingValue = true;
			continue;
		}
		else if (character == ';') {
			if (token == "ipaddr") {
				ip = value;
				if (value.empty()) ip = "127.0.0.1";
			}
			if (token == "console") {
				if (value == "true") console = true;
				else console = false;
			}
			if (token == "sounds") {
				if (value == "true") sounds = true;
				else sounds = false;
			}
			if (token == "show_legals") {
				if (value == "true") highlightLegals = true;
				else highlightLegals = false;
			}
			readingToken = true;
			readingValue = false;
			token.clear();
			value.clear();
			continue;
		}
		else if (character == '\n') continue;

		if (readingToken) token.push_back(character);
		if (readingValue) value.push_back(character);
	}
}

void Client::connect() {
	sf::Packet initial;
	serverConnection.connect(ip, port);
	serverConnection.receive(initial);
	initial >> playerName;
	serverConnection.setBlocking(false);
}

bool Client::showConsole()
{
	return console;
}

void Client::init() {
	send = new sf::Thread(&Client::sendRequest, this);
	receive = new sf::Thread (&Client::receiveResponse, this);
	handle = new sf::Thread(&Client::handleResponse, this);

	connect();

	send->launch();
	receive->launch();
	handle->launch();
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
			req << raw;
			serverConnection.send(req);
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void Client::receiveResponse() {
	while (true) {
		sf::Packet response;
		serverConnection.receive(response);
		if (response.getDataSize() > 0) responses.push(response);
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void Client::handleResponse() {
	while (true) {
		if (responses.size() > 0) {
			
			std::string resp;
			responses.pop() >> resp;
			Response response = Response::parse(resp);
			response.handle();
			
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
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

void Client::setGameState(GameState gameState)
{
	gs = gameState;
}

void Client::setInGame(bool s)
{
	inGame = s;
}

void Client::setServerConnection(bool s)
{
	serverConnection.setBlocking(s);
}

void Client::setGameOverReason(std::string s)
{
	gameOverReason = s;
}

void Client::setOnlinePlayers(std::list<std::string> s)
{
	onlinePlayers = s;
}

void Client::addOnlinePlayer(std::string s)
{
	onlinePlayers.push_back(s);
}

std::string Client::getGameOverReason()
{
	return gameOverReason;
}

void Client::setOnlinePlayersU2D(bool s)
{
	onlinePlayersUpToDate = s;
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

bool Client::playSounds()
{
	return sounds;
}

bool Client::doHighlightLegals()
{
	return highlightLegals;
}

void Client::initSoloGame()
{
	game = SoloGame(color);
}

void Client::killInstance()
{
	send->terminate();
	receive->terminate();
	handle->terminate();
	
	delete send;
	delete receive;
	delete handle;
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
