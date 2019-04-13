#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include <iostream>

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
	tgui::Button::Ptr forfeit;
	tgui::Label::Ptr currentTurn;
	

	tgui::Panel::Ptr messageBoxBody;
	tgui::Label::Ptr messageInfo;
	tgui::Button::Ptr dismiss;


	void inv();
	void playReq();
	void getPendingInvites();
	void toggleReady();
	

	sf::Font latoDefault;
	sf::Font latoBold;
	sf::Color lightBlue = sf::Color(219, 226, 229, 255);
	sf::Color darkGrey = sf::Color(8, 7, 5, 255);
	sf::Color blue = sf::Color(24, 25, 29, 255);
	sf::Color darkBlue = sf::Color(125, 152, 161, 255);
	sf::Color niceGreen = sf::Color(87, 181, 107, 255);
	sf::Color darkerGreen = sf::Color(48, 99, 59, 255);
	sf::Color niceRed = sf::Color(178, 50, 66, 255);
	sf::Color darkerRed = sf::Color(130, 37, 49, 255);

	bool showingMessageBox = false;
	bool messageBoxDissmised = true;
	void dissmissMsg();

public:
	static GuiManager* instance();
	tgui::Gui* getMainUIHandle();
	void highlightPendingButton();
	void setInfoBoardInfo();
	void setIsReady(bool);
	void setLobbyUI(bool);
	void setMenuUI(bool);
	void setInGameUI(bool);
	void setMessageBox(bool);
	void displayMessage(std::string);
	bool isShowingMessageBox();
	void init();
};