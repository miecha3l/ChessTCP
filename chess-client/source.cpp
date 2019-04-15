#include "SFML/Graphics.hpp"
#include "SFML/Network.hpp"
#include "SFML/Audio.hpp"
#include "Chess.h"
#include "gui.h"
#include "GameState.h"
#include "Response.h"
#include "ChessBot.h"
#include <iostream>
#include "Client.h"
#include "SoloGame.h"
#include <Windows.h>

sf::VideoMode resolution(1000, 760);
sf::RenderWindow client;
sf::Texture bgForWhite;
sf::Texture bgForBlack;
sf::Texture splashArt;
sf::Texture lobbyBg;
sf::Sprite splash;
sf::Sprite lobby;
sf::Sprite background;
std::string response;
sf::SoundBuffer moveSoundBuffer;
sf::Sound moveSound;


const int tileDims = 70;
const int offset = 100;

void drawGameState(GameState gs, std::string color);
CompressedPiece getPieceClicked(sf::RenderWindow &w, GameState gs, std::string color);
int getClickedMove(CompressedPiece &p, std::string color);
void drawLegalMoves(CompressedPiece p, std::string color);

void windowThread() {
	bool isPieceSelected = false;
	bool bgLoaded = false;
	CompressedPiece pieceSelected("not_found;00;;");
	sf::RectangleShape dimm(sf::Vector2f(1000, 760));
	dimm.setFillColor(sf::Color(20, 20, 20, 150));
	bgForWhite.loadFromFile("assets/backgroundWhite.png");
	bgForBlack.loadFromFile("assets/backgroundBlack.png");
	splashArt.loadFromFile("assets/splash.png");
	lobbyBg.loadFromFile("assets/lobbybg.png");
	splash.setTexture(splashArt);
	lobby.setTexture(lobbyBg);
	moveSoundBuffer.loadFromFile("assets/move.wav");
	moveSound.setBuffer(moveSoundBuffer);
	client.create(resolution, "Chess");
	while (client.isOpen()) {
		if (!bgLoaded && !Client::instance()->getColor().empty()) {
			if (Client::instance()->getColor() == "white") background.setTexture(bgForWhite);
			else if (Client::instance()->getColor() == "black") background.setTexture(bgForBlack);
		}
		

		//solo game stuff
		if (Client::instance()->getCurrentScreen() == Client::Screen::OfflineGame) {
			if (!Client::instance()->getSoloGameInstance()->isInitialized()) {
				Client::instance()->initSoloGame();
				Client::instance()->getSoloGameInstance()->turnChecks();
			}
			else if (!Client::instance()->getSoloGameInstance()->turnChecksDone()) {
				if (Client::instance()->getSoloGameInstance()->turnChecks()) {
					if (Client::instance()->getSoloGameInstance()->getCurrentTurn() != Client::instance()->getColor()) {
						Client::instance()->getSoloGameInstance()->botMove();
					}
				}
				else {
					GuiManager::instance()->displayMessage(Client::instance()->getSoloGameInstance()->getGameOver());
				}
			}
		}



		sf::Event evnt;
		while (client.pollEvent(evnt)) {
			if (evnt.type == sf::Event::Closed) client.close();

			//online game events
			if (evnt.type == sf::Event::MouseButtonPressed && Client::instance()->getGameState().getCurrentGameTurn() == Client::instance()->getColor() && Client::instance()->isInGame()) {
				if (isPieceSelected && pieceSelected.name != "not_found") {
					int moveId = getClickedMove(pieceSelected, Client::instance()->getColor());
					if (moveId > 0) {
						moveSound.play();
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

			//solo game events
			if (evnt.type == sf::Event::MouseButtonPressed && Client::instance()->getColor() == Client::instance()->getSoloGameInstance()->getCurrentTurn() && Client::instance()->getCurrentScreen() == Client::Screen::OfflineGame) {
				if (isPieceSelected && pieceSelected.name != "not_found") {
					int moveId = getClickedMove(pieceSelected, Client::instance()->getColor());
					if (moveId > 0) {
						moveSound.play();
						Client::instance()->getSoloGameInstance()->getPlayersMove(moveId);
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
			if (GuiManager::instance()->mainUIHandleFree()) {
				GuiManager::instance()->getMainUIHandle()->handleEvent(evnt);
			}
		}


		if (Client::instance()->getCurrentScreen() == Client::Screen::OnlineGame) {
			if (isPieceSelected) GuiManager::instance()->drawGui(background, &drawGameState, Client::instance()->getGameState(), Client::instance()->getColor(),
				&drawLegalMoves, pieceSelected, Client::instance()->getColor());


			else if (GuiManager::instance()->isShowingMessageBox()) {
				if (isPieceSelected)GuiManager::instance()->drawGui(background, dimm, &drawGameState, Client::instance()->getGameState(), Client::instance()->getColor(),
					&drawLegalMoves, pieceSelected, Client::instance()->getColor());

				else GuiManager::instance()->drawGui(background, &drawGameState, Client::instance()->getGameState(), Client::instance()->getColor());
			}
			else GuiManager::instance()->drawGui(background, &drawGameState, Client::instance()->getGameState(), Client::instance()->getColor());

		}

		else {

			if (bgLoaded) bgLoaded = false;


			if (Client::instance()->getCurrentScreen() == Client::Screen::Menu) {
				if (GuiManager::instance()->isShowingMessageBox()) {
					GuiManager::instance()->drawGui(splash, dimm);
					GuiManager::instance()->setMenuUI(false, true);
				}
				else GuiManager::instance()->drawGui(splash);
			}


			else if (Client::instance()->getCurrentScreen() == Client::Screen::Lobby) {

				if (GuiManager::instance()->isShowingMessageBox()) {
					GuiManager::instance()->drawGui(lobby, dimm);
					GuiManager::instance()->setLobbyUI(false, true);
				}
				else GuiManager::instance()->drawGui(lobby);

				if (Client::instance()->getName().empty()) GuiManager::instance()->displayMessage("You're offline, sorry");
			}


			else if (Client::instance()->getCurrentScreen() == Client::Screen::OfflineGame) {

				if (isPieceSelected) GuiManager::instance()->drawGui(background, &drawGameState, Client::instance()->getGameState(), Client::instance()->getColor(),
																		&drawLegalMoves, pieceSelected, Client::instance()->getColor());


				else if (GuiManager::instance()->isShowingMessageBox()) {			
					if(isPieceSelected)GuiManager::instance()->drawGui(background, dimm, &drawGameState, Client::instance()->getGameState(), Client::instance()->getColor(),
																			&drawLegalMoves, pieceSelected, Client::instance()->getColor());

					else GuiManager::instance()->drawGui(background, &drawGameState, Client::instance()->getGameState(), Client::instance()->getColor());
				}
				else GuiManager::instance()->drawGui(background, &drawGameState, Client::instance()->getGameState(), Client::instance()->getColor());


				if (Client::instance()->getSoloGameInstance()->getCurrentTurn() != Client::instance()->getColor()) {
					isPieceSelected = false;
					pieceSelected = CompressedPiece("not_found;00;;");
				}
			}
		}
		client.display();
	}
}


int main() {
	GuiManager::instance()->init();
	sf::Thread clientWindow(&windowThread);

	clientWindow.launch();
	Client::instance()->init();

	return 0;
}

void drawGameState(GameState gs, std::string color) {
	
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

		client.draw(piece);
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

		client.draw(piece);
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