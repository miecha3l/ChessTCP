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
std::string ipAddr;
const int tileDims = 70;
const int offset = 100;
std::string response;
sf::Texture backgroundTexture;
sf::Sprite background;

tgui::Gui gui{ menu };
tgui::TextBox::Ptr connectionAddrInput;
tgui::Button::Ptr submitInput;

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
			if (evnt.type == sf::Event::MouseButtonPressed && gs.getCurrentGameTurn() == color) {
				if (isPieceSelected && pieceSelected.name != "not_found") {
					int moveId = getClickedMove(pieceSelected);
					if (moveId > 0) response = std::to_string(moveId);
				}

				CompressedPiece piece = getPieceClicked(client, gs);
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

int main() {
	sf::Thread clientWindow(&windowThread);
	sf::TcpSocket serverConnection;
	int port;
	sf::Packet serverMessage;
	std::string messageContainer;
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

		//get port on which the game will be played
		Connection gameServer(ipAddr, 1111);
		serverConnection.connect(gameServer.ipAddr, gameServer.port);
		serverConnection.receive(serverMessage);
		serverMessage >> messageContainer >> color;
		port = std::stoi(messageContainer);
		serverConnection.disconnect();
		serverMessage.clear();
		messageContainer.clear();
		gameServer.port = port;
		serverConnection.connect(gameServer.ipAddr, gameServer.port);

		clientWindow.launch();
		while (true) {
			response.clear();

			//receive gamestate
			serverConnection.receive(serverMessage);
			serverMessage >> messageContainer;
			gs = GameState(messageContainer);
			serverMessage.clear();
			messageContainer.clear();

			if (color == "black" && gs.getCurrentGameTurn() == "white") continue;

			//wait for user to pick a move
			while (response.size() == 0) {
				system("cls");
				std::cout << "waiting for move" << std::endl;
			}

			//send response back to server
			serverMessage << response;
			serverConnection.send(serverMessage);
			serverMessage.clear();

			//get visual update
			serverConnection.receive(serverMessage);
			serverMessage >> messageContainer;
			gs = GameState(messageContainer);
			serverMessage.clear();
			messageContainer.clear();
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