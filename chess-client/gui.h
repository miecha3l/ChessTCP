#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"
#include <iostream>

extern sf::RenderWindow client;
extern tgui::Gui gui;
void initGui();
void getPlayerList();
extern tgui::Label::Ptr playerName;
extern tgui::Label::Ptr matchNameLabel;
extern tgui::TextBox::Ptr inputUserName;
extern tgui::Button::Ptr invButton;
extern tgui::Button::Ptr playButton;
extern tgui::ScrollablePanel::Ptr invitesPanel;
extern tgui::Button::Ptr refresh;