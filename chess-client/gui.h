#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include <iostream>
#include "GameState.h"

extern sf::RenderWindow client;

class GuiManager {
private:
	GuiManager();
	static GuiManager *gm;

	tgui::Gui mainUI{ client };
	tgui::VerticalLayout::Ptr mainUILayout;
	tgui::Button::Ptr playSolo;
	tgui::Button::Ptr playWithFriend;

	tgui::VerticalLayout::Ptr lobbyUILayout;
	tgui::HorizontalLayout::Ptr topRow;
	tgui::HorizontalLayout::Ptr bottomRow;
	tgui::Button::Ptr sendInvite;
	tgui::Button::Ptr showInvites;
	tgui::Panel::Ptr invitesPanel;
	tgui::TextBox::Ptr inputUserName;
	tgui::Panel::Ptr infoBoard;
	tgui::Button::Ptr play;
	tgui::Button::Ptr backToMenu;
	tgui::CheckBox::Ptr isReady;



	tgui::VerticalLayout::Ptr soloGameLayout;
	tgui::Button::Ptr quitGame;
	tgui::Button::Ptr resetGame;
	

	tgui::VerticalLayout::Ptr multiGameLayout;
	tgui::Button::Ptr disconnect;
	tgui::Label::Ptr currentTurn;
	

	tgui::Panel::Ptr messageBoxBody;
	tgui::Label::Ptr messageInfo;
	tgui::Button::Ptr dismiss;


	void inv();
	void playReq();
	
	

	sf::Font latoDefault;
	sf::Font latoBold;
	sf::Color lightBlue = sf::Color(219, 226, 229, 255);
	sf::Color darkGrey = sf::Color(56, 56, 56, 255);
	sf::Color blue = sf::Color(24, 25, 29, 255);
	sf::Color darkBlue = sf::Color(146, 150, 155, 255);
	sf::Color niceGreen = sf::Color(87, 181, 107, 255);
	sf::Color darkerGreen = sf::Color(48, 99, 59, 255);
	sf::Color niceRed = sf::Color(178, 50, 66, 255);
	sf::Color darkerRed = sf::Color(130, 37, 49, 255);
	sf::Color borders = sf::Color(15, 15, 15, 255);

	bool lockGuiDraw = false;
	bool showingMessageBox = false;
	bool messageBoxDissmised = true;
	void dissmissMsg();
	bool updatingInfoBoard = false;

public:
	void getPendingInvites();
	static GuiManager* instance();
	tgui::Gui* getMainUIHandle();

	//default
	void drawGui();

	//draw only with bg
	void drawGui(sf::Sprite bg);

	//draw with bg & dimm
	void drawGui(sf::Sprite bg, sf::RectangleShape);

	//draw with gs 
	void drawGui(sf::Sprite bg, void(*f)(GameState, std::string color), GameState p1, std::string p3);

	//draw with gs & dimm
	void drawGui(sf::Sprite bg, sf::RectangleShape dimm, void(*f)(GameState, std::string color), GameState p1, std::string p3);

	//draw with gs & legals
	void drawGui(sf::Sprite bg, void(*drawGs)(GameState, std::string color), GameState p1, std::string p3,
					void(*drawLegals)(CompressedPiece, std::string), CompressedPiece p4, std::string p5);

	//draw with gs & legals & dimm
	void drawGui(sf::Sprite bg, sf::RectangleShape dimm, void(*drawGs)(GameState, std::string color), GameState p1, std::string p3,
		void(*drawLegals)(CompressedPiece, std::string), CompressedPiece p4, std::string p5);


	bool mainUIHandleFree();


	void highlightPendingButton();
	void setInfoBoardInfo();
	void setInfoBoard(bool);

	void setLobbyUI(bool);
	void setLobbyUI(bool, bool);

	void setMenuUI(bool);
	void setMenuUI(bool, bool);

	void setSoloGameUI(bool);
	void setSoloGameUI(bool, bool);

	void setMultiGameUI(bool);
	void setMultiGameUI(bool, bool);

	void setCurrentTurnLabel(std::string);

	void setDrawLock(bool);

	void setMessageBox(bool);
	void displayMessage(std::string);
	bool isShowingMessageBox();
	void init();
};