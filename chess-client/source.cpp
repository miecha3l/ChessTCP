#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "Chess.h"
#include "gui.h"
#include "GameState.h"
#include "Response.h";
#include <iostream>
#include "Client.h"

sf::VideoMode resolution(1000, 720);
sf::RenderWindow client;
sf::Texture bgForWhite;
sf::Texture bgForBlack;
sf::Texture splashArt;
sf::Sprite logo;
sf::Sprite background;
std::string response;
GameState gs;
bool inGame;

tgui::Gui gui{ client };
tgui::VerticalLayout::Ptr layoutB;
tgui::TextBox::Ptr inputUserName;
tgui::Button::Ptr invButton;
tgui::Button::Ptr playButton;
tgui::Label::Ptr playerName;
tgui::Label::Ptr matchNameLabel;
tgui::ScrollablePanel::Ptr invitesPanel;
tgui::Button::Ptr showInvites;

tgui::Gui mainMenu{ client };
tgui::VerticalLayout::Ptr layoutA;
tgui::Button::Ptr playWithFriend;
tgui::Button::Ptr playSolo;

const int tileDims = 70;
const int offset = 100;

void drawGameState(GameState gs, sf::RenderWindow &w, std::string color);
CompressedPiece getPieceClicked(sf::RenderWindow &w, GameState gs, std::string color);
int getClickedMove(CompressedPiece &p, std::string color);
void drawLegalMoves(CompressedPiece p, std::string color);

void windowThread() {
	bool isPieceSelected = false;
	bool bgLoaded = false;
	CompressedPiece pieceSelected("not_found;00;;");
	bgForWhite.loadFromFile("assets/backgroundWhite.png");
	bgForBlack.loadFromFile("assets/backgroundBlack.png");
	initGui();


	client.create(resolution, "Chess");
	while (client.isOpen()) {
		if (!bgLoaded && !Client::instance()->getColor().empty()) {
			if (Client::instance()->getColor() == "white") background.setTexture(bgForWhite);
			else if (Client::instance()->getColor() == "black") background.setTexture(bgForBlack);
		}
		sf::Event evnt;
		while (client.pollEvent(evnt)) {
			if (evnt.type == sf::Event::Closed) client.close();

			//player creating response
			if (evnt.type == sf::Event::MouseButtonPressed && Client::instance()->getGameState().getCurrentGameTurn() == Client::instance()->getColor() && Client::instance()->isInGame()) {
				if (isPieceSelected && pieceSelected.name != "not_found") {
					int moveId = getClickedMove(pieceSelected, Client::instance()->getColor());
					std::cout << moveId << std::endl;
					if (moveId > 0) {
						std::string type = "game_req/";
						Client::instance()->addReqToQueue(type.append(std::to_string(moveId)).append("/").append(Client::instance()->getName()).append("/").append(Client::instance()->getMatchName()));
					}
				}

				CompressedPiece piece = getPieceClicked(client, Client::instance()->getGameState(), Client::instance()->getColor());
				pieceSelected = piece;
				if (piece.name != "not_found") {
					isPieceSelected = true;
				}
				else {
					isPieceSelected = false;
					
				}
			}
			gui.handleEvent(evnt);
		}

		client.clear();
		if (Client::instance()->isInGame()) {
			client.draw(background);
			if (isPieceSelected) {
				drawLegalMoves(pieceSelected, Client::instance()->getColor());
			}
			drawGameState(Client::instance()->getGameState(), client, Client::instance()->getColor());
		}
		else {
			if (bgLoaded) bgLoaded = false;
			gui.draw();
		}
		client.display();
	}
}


int main() {
	sf::Thread clientWindow(&windowThread);

	clientWindow.launch();
	Client::instance()->init();

	
	return 0;
}

void drawGameState(GameState gs, sf::RenderWindow &w, std::string color) {
	
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

CompressedPiece getPieceClicked(sf::RenderWindow &w, GameState gs, std::string color) {
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

void drawLegalMoves(CompressedPiece p, std::string color) {
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

int getClickedMove(CompressedPiece &p, std::string color) {
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
			//fake move
			p.boardPos = m.moveCoords;
			return m.id;
		}
	}
	return -9999;
}