#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "Chess.h"
#include "gui.h"
#include "GameState.h"
#include <iostream>


sf::VideoMode resolution(1000, 760);
sf::RenderWindow client;
sf::Texture backgroundTexture;
sf::Sprite background;
tgui::Gui gui{ client };
tgui::TextBox::Ptr connectionAddrInput;
tgui::Button::Ptr submitInput;

sf::TcpSocket serverConnection;
std::string playerName;
bool inGame = false;
GameState gs;
std::string color;
std::string response;
const int tileDims = 70;
const int offset = 100;



void drawGameState(GameState gs, sf::RenderWindow &w);
CompressedPiece getPieceClicked(sf::RenderWindow &w, GameState gs);
int getClickedMove(CompressedPiece p);
void drawLegalMoves(CompressedPiece p);


struct Connection {
	std::string ipAddr;
	int port;
	Connection(std::string ip, int port) : ipAddr(ip), port(port){}
};


void windowThread() {
	bool isPieceSelected = false;
	CompressedPiece pieceSelected("not_found;00;;");
	std::string backgroundPath = color == "black" ? "assets/backgroundBlack.png" : "assets/backgroundWhite.png";
	backgroundTexture.loadFromFile(backgroundPath);
	background.setTexture(backgroundTexture);

	client.create(resolution, "Chess");
	while (client.isOpen()) {
		sf::Event evnt;
		while (client.pollEvent(evnt)) {
			if (evnt.type == sf::Event::Closed) client.close();

			//player creating response
			if (evnt.type == sf::Event::MouseButtonPressed && gs.getCurrentGameTurn() == color && inGame) {
				if (isPieceSelected && pieceSelected.name != "not_found") {
					int moveId = getClickedMove(pieceSelected);
					std::cout << moveId << std::endl;
					if (moveId > 0) response = std::to_string(moveId);
				}

				CompressedPiece piece = getPieceClicked(client, gs);
				pieceSelected = piece;
				if (piece.name != "not_found") {
					isPieceSelected = true;
				}
				else {
					isPieceSelected = false;
				}
			}

		}

		client.clear();
		if (inGame) {
			client.draw(background);
			if (isPieceSelected) {
				drawLegalMoves(pieceSelected);
			}
			drawGameState(gs, client);
		}
		else {

		}
		client.display();
	}
}

void getResponse() {
	while (!inGame) {
		sf::Packet srvMsg;
		std::string data;
		serverConnection.receive(srvMsg);
		srvMsg >> data;
		if (!data.empty()) {
			if (data == "black" || data == "white") {
				color = data;
				inGame = true;
			}
			else std::cout << "Response: " << data << std::endl;
		}
		data.clear();
	}
}

void sendReq() {
	while (!inGame) {
		sf::Packet srvMsg;
		std::string data;
		std::cin >> data;
		if (serverConnection.receive(srvMsg) != sf::Socket::Status::Done) {
			srvMsg << data;
			serverConnection.send(srvMsg);
		}
	}
}

int main() {
	sf::Thread clientWindow(&windowThread);
	sf::Thread recieve(&getResponse);
	sf::Thread send(&sendReq);
	sf::Packet serverMessage;
	std::string data;
	initGui();

	//initial connection
	Connection gameServer("127.0.0.1", 8888);
	serverConnection.connect(gameServer.ipAddr, gameServer.port);
	serverConnection.receive(serverMessage);
	serverMessage >> playerName;
	std::cout << "Welcome " << playerName << std::endl;
	clientWindow.launch();
	
	//outer client loop
	while (true) {
		std::cout << "MENU" << std::endl;
		//client in menu
		serverConnection.setBlocking(false);
		recieve.launch();
		send.launch();

		while (!inGame) {}

		recieve.terminate();
		send.terminate();

		data.clear();
		serverMessage.clear();

		std::cout << "GAME" << std::endl;
		serverConnection.setBlocking(true);


		//game loop
		 do {
			response.clear();

			//receive gamestate
			serverConnection.receive(serverMessage);
			serverMessage >> data;
			gs = GameState(data);
			serverMessage.clear();
			data.clear();

			if (color == "black" && gs.getCurrentGameTurn() == "white") continue;
			else if (color == "white" && gs.getCurrentGameTurn() == "black") continue;

			//wait for user to pick a move
			while (response.empty()) {}

			std::cout << "Response: " << response << std::endl;

			//send response back to server
			serverMessage << response;
			if (serverConnection.send(serverMessage) == sf::Socket::Status::Done) {
				std::cout << "sent properly" << std::endl;
			}
			else std::cout << "sent not properly" << std::endl;
			serverMessage.clear();


			//get visual update
			serverConnection.receive(serverMessage);
			serverMessage >> data;
			gs = GameState(data);
			serverMessage.clear();
			data.clear();
		 } while (inGame);
		inGame = false;
	}
	


	return 0;
}

void drawGameState(GameState gs, sf::RenderWindow &w) {
	
	std::string texturePathRoot = "assets/";
	for (auto p : gs.getWhitePieces()) {
		sf::Vector2f screenPos;
		if (color == "white") {
			screenPos = sf::Vector2f((p.boardPos.x * tileDims) + offset, (p.boardPos.y * tileDims) + offset);
		}
		else {
			screenPos = sf::Vector2f((p.boardPos.x * tileDims) + offset, ((7 - p.boardPos.y) * tileDims) + offset);
		}
		sf::Texture texture;
		sf::Sprite piece;

		std::string completePath = texturePathRoot;
		completePath.append("white/");
		completePath.append(p.name);
		completePath.append(".png");

		texture.loadFromFile(completePath);
		piece.setTexture(texture);
		piece.setPosition(screenPos);

		w.draw(piece);
	}

	for (auto p : gs.getBlackPieces()) {
		sf::Vector2f screenPos;
		if (color == "white") {
			screenPos = sf::Vector2f((p.boardPos.x * tileDims) + offset, (p.boardPos.y * tileDims) + offset);
		}
		else{
			screenPos = sf::Vector2f((p.boardPos.x * tileDims) + offset, ((7 - p.boardPos.y) * tileDims) + offset);
		}
		
		sf::Texture texture;
		sf::Sprite piece;

		std::string completePath = texturePathRoot;
		completePath.append("black/");
		completePath.append(p.name);
		completePath.append(".png");

		texture.loadFromFile(completePath);
		piece.setTexture(texture);
		piece.setPosition(screenPos);

		w.draw(piece);
	}
}

CompressedPiece getPieceClicked(sf::RenderWindow &w, GameState gs) {
	sf::Vector2i posClicked = sf::Mouse::getPosition(w);
	
	if (color == "white") {
		for (auto p : gs.getWhitePieces()) {
			sf::Vector2f screenPos = sf::Vector2f((p.boardPos.x * tileDims) + offset, (p.boardPos.y * tileDims) + offset);
			if (posClicked.x >= screenPos.x && posClicked.x <= screenPos.x + tileDims &&
				posClicked.y >= screenPos.y && posClicked.y <= screenPos.y + tileDims) {
				return p;
			}
		}
		return CompressedPiece("not_found;00;;");
	}
	else {
		for (auto p : gs.getBlackPieces()) {
			sf::Vector2f screenPos = sf::Vector2f((p.boardPos.x * tileDims) + offset, ((7 - p.boardPos.y) * tileDims) + offset);
			if (posClicked.x >= screenPos.x && posClicked.x <= screenPos.x + tileDims &&
				posClicked.y >= screenPos.y && posClicked.y <= screenPos.y + tileDims) {
				return p;
			}
		}
		return CompressedPiece("not_found;00;;");
	}
}

void drawLegalMoves(CompressedPiece p) {
	sf::Texture markText;
	markText.loadFromFile("assets/moveMark.png");

	for (auto m : p.legalMoves) {
		sf::Vector2f screenPos;
		if (color == "white") {
			screenPos = sf::Vector2f((m.moveCoords.x * tileDims) + offset, (m.moveCoords.y * tileDims) + offset);
		}
		else {
			screenPos = sf::Vector2f((m.moveCoords.x * tileDims) + offset, ((7 - m.moveCoords.y) * tileDims) + offset);
		}
		sf::Sprite mark;
		mark.setTexture(markText);
		mark.setPosition(screenPos);

		client.draw(mark);
	}
}

int getClickedMove(CompressedPiece p) {
	sf::Vector2i posClicked = sf::Mouse::getPosition(client);
	for (auto m : p.legalMoves) {
		sf::Vector2f screenPos;
		if (color == "white") {
			screenPos = sf::Vector2f((m.moveCoords.x * tileDims) + offset, (m.moveCoords.y * tileDims) + offset);
		}
		else {
			screenPos = sf::Vector2f((m.moveCoords.x * tileDims) + offset, ((7 - m.moveCoords.y) * tileDims) + offset);
		}
		if (posClicked.x >= screenPos.x && posClicked.x <= screenPos.x + tileDims &&
			posClicked.y >= screenPos.y && posClicked.y <= screenPos.y + tileDims) {
			return m.id;
		}
	}
	return -9999;
}