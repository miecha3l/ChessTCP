#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "Chess.h"
#include "gui.h"
#include "GameState.h"
#include <iostream>

GameState gs;
sf::VideoMode resolution(1000, 760);
sf::RenderWindow client;
sf::RenderWindow menu;

std::string color;
int tileDims = 70;
int offset = 100;
std::string response;
sf::Texture backgroundTexture;
sf::Sprite background;

sf::TcpSocket serverConnection;
struct Connection {
	std::string ipAddr;
	int port;
	Connection(std::string ip, int port) : ipAddr(ip), port(port){}
};

void drawGameState(GameState gs, sf::RenderWindow &w);
CompressedPiece getPieceClicked(sf::RenderWindow &w);
int getClickedMove(CompressedPiece p);
void drawLegalMoves(CompressedPiece p);

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

			if (evnt.type == sf::Event::MouseButtonPressed && gs.getCurrentGameTurn() == color) {
				if (isPieceSelected && pieceSelected.name != "not_found") {
					int moveId = getClickedMove(pieceSelected);
					if (moveId > 0) response = std::to_string(moveId);
				}

				CompressedPiece piece = getPieceClicked(client);
				pieceSelected = piece;
				if (piece.name != "not_found") isPieceSelected = true;
				else isPieceSelected = false;
			}
		}


		client.clear();
		client.draw(background);
		if (isPieceSelected) {
			drawLegalMoves(pieceSelected);
		}
		drawGameState(gs, client);
		client.display();
	}
}

sf::Thread clientWindow(&windowThread);
tgui::Gui gui{menu};
tgui::TextBox::Ptr connectionAddrInput;
tgui::Button::Ptr submitInput;
std::string ipAddr;
int port;

int main() {

	initGui();
	while (true) {
		menu.create(resolution, "Chess - menu");
		while (menu.isOpen()) {
			sf::Event evnt;
			while (menu.pollEvent(evnt)) {
				if (evnt.type == sf::Event::Closed) menu.close();
				gui.handleEvent(evnt);
			}

			menu.clear();
			gui.draw();
			menu.display();
		}

		Connection gameServer(ipAddr, 1111);
		serverConnection.connect(gameServer.ipAddr, gameServer.port);
		sf::Packet portInfo; std::string msgCont;
		serverConnection.receive(portInfo);
		portInfo >> msgCont >> color;
		port = std::stoi(msgCont);

		serverConnection.disconnect();

		gameServer.port = port;
		serverConnection.connect(gameServer.ipAddr, gameServer.port);

		clientWindow.launch();
		while (true) {
			response.clear();

			sf::Packet initialMessage;
			std::string initialContainer;
			serverConnection.receive(initialMessage);
			initialMessage >> initialContainer;
			gs = GameState(initialContainer);

			if (color == "black" && gs.getCurrentGameTurn() == "white") continue;

			while (response.size() == 0) {
				system("cls");
				std::cout << "waiting for move" << std::endl;
			}


			sf::Packet message;
			message << response;
			serverConnection.send(message);

			sf::Packet serverCallback;
			std::string container;
			serverConnection.receive(message);
			message >> container;
			gs = GameState(container);

		}
	}
		
	system("pause");
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

CompressedPiece getPieceClicked(sf::RenderWindow &w) {
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