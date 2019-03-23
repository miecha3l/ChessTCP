#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/TGUI.hpp"

extern sf::RenderWindow menu;
extern int port;
extern tgui::Gui gui;

void initGui();

extern tgui::TextBox::Ptr connectionAddrInput;
extern tgui::Button::Ptr submitInput;
