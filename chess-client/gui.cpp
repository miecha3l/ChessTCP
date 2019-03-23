#include "gui.h"

void initGui() {
	connectionAddrInput = tgui::TextBox::create();
	submitInput = tgui::Button::create();

	gui.add(connectionAddrInput);
	gui.add(submitInput);

	connectionAddrInput->setMaximumCharacters(20);
	connectionAddrInput->setFocused(true);
	connectionAddrInput->setEnabled(true);
	connectionAddrInput->setPosition(450, 350);
	connectionAddrInput->setSize(100, 30);
	
	submitInput->setEnabled(true);
	submitInput->setSize(75, 30);
	submitInput->setPosition(462.5, 380);
	submitInput->setText("Connect");
	submitInput->connect("pressed", [&]() {port = std::stoi(connectionAddrInput->getText().toAnsiString()); menu.close(); });
}