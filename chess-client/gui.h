#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include <iostream>

extern sf::RenderWindow client;

extern tgui::Gui mainMenu;
extern tgui::VerticalLayout::Ptr layoutA;
extern tgui::Button::Ptr playWithFriend;
extern tgui::Button::Ptr playSolo;

extern tgui::Gui gui;
void initGui();
void getPlayerList();
extern tgui::VerticalLayout::Ptr layoutB;
extern tgui::Label::Ptr playerName;
extern tgui::Label::Ptr matchNameLabel;
extern tgui::TextBox::Ptr inputUserName;
extern tgui::Button::Ptr invButton;
extern tgui::Button::Ptr playButton;
extern tgui::ScrollablePanel::Ptr invitesPanel;
extern tgui::Button::Ptr showInvites;