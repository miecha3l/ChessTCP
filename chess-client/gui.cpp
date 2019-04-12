#include "gui.h"
#include "Client.h"

sf::Color lightBlue = sf::Color(219, 226, 229, 255);
sf::Color darkGrey = sf::Color(8, 7, 5, 255);
sf::Color blue = sf::Color(24, 25, 29, 255);
sf::Color darkBlue = sf::Color(125, 152, 161, 255);
sf::Color niceGreen = sf::Color(87, 181, 107, 255);
sf::Color niceRed = sf::Color(178, 50, 66, 255);

void inv() {
	std::string body = "req/match/";
	if (inputUserName->getText().getSize() > 0) {
		Client::instance()->setRequester(inputUserName->getText().toAnsiString());
		Client::instance()->addReqToQueue(body.append(inputUserName->getText().toAnsiString()).append("/").append(Client::instance()->getName()));
		inputUserName->setText("");
	}
}

void play() {
	std::string body = "req/play/";
	if (!Client::instance()->getMatchName().empty()) {
		Client::instance()->addReqToQueue(body.append(Client::instance()->getMatchName()).append("/").append(Client::instance()->getName()));
	}
}

void getPlayerList() {
	if (Client::instance()->isMatchReqPending()) {
		Client::instance()->setOnlinePlayersListUpToDate(false);
		std::string body = "req/get_plist/server/";
		Client::instance()->addReqToQueue(body.append(Client::instance()->getName()));
		invitesPanel->removeAllWidgets();
	
		while(!Client::instance()->isPlayersListUpToDate()){}

		invitesPanel->setSize(200, Client::instance()->getOnlinePlayersList().size() * 25);

		for (auto p : Client::instance()->getOnlinePlayersList()) {
			if (p == Client::instance()->getName()) continue;
			if (Client::instance()->getRequester() == p) {
				auto playersLabel = tgui::Label::create();
				invitesPanel->add(playersLabel);
				int yPos = (invitesPanel->getWidgets().size() - 1) * 25;
				playersLabel->setSize("45%", 25);
				playersLabel->setPosition("0%", yPos);
				playersLabel->getRenderer()->setTextColor(lightBlue);
				playersLabel->setTextSize(13);
				playersLabel->setText(p);

			
				auto acceptButton = tgui::Button::create();
				invitesPanel->add(acceptButton);
				acceptButton->setSize("25%", 25);
				acceptButton->setPosition("46%", yPos);
				acceptButton->setText("Accept");
				acceptButton->getRenderer()->setTextColor(darkGrey);
				acceptButton->getRenderer()->setTextColorDown(lightBlue);
				acceptButton->getRenderer()->setBackgroundColor(lightBlue);
				acceptButton->getRenderer()->setBackgroundColorHover(darkBlue);
				acceptButton->getRenderer()->setBackgroundColorDown(darkBlue);
				acceptButton->getRenderer()->setBorderColor(darkBlue);
				acceptButton->setTextSize(13);
				acceptButton->connect("pressed", [&]() {
					std::string msg = "req/match_acc/";
					Client::instance()->setMatchAcc(true);
					Client::instance()->setMatchReq(false);
					Client::instance()->setMatchName(Client::instance()->getRequester());
					std::string text = "Match: ";
					matchNameLabel->setText(text.append(Client::instance()->getMatchName()));
					Client::instance()->addReqToQueue(msg.append(Client::instance()->getMatchName()).append("/").append(Client::instance()->getName()));
					invitesPanel->removeAllWidgets();
					invitesPanel->setEnabled(false);
					invitesPanel->setVisible(false);
				});

				auto declineButton = tgui::Button::create();
				invitesPanel->add(declineButton);
				declineButton->setSize("25%", 25);
				declineButton->setPosition("75%", yPos);
				declineButton->setText("Decline");
				declineButton->setTextSize(13);
				declineButton->getRenderer()->setTextColor(darkGrey);
				declineButton->getRenderer()->setTextColorDown(lightBlue);
				declineButton->getRenderer()->setBackgroundColor(lightBlue);
				declineButton->getRenderer()->setBackgroundColorHover(darkBlue);
				declineButton->getRenderer()->setBackgroundColorDown(darkBlue);
				declineButton->getRenderer()->setBorderColor(darkBlue);
				declineButton->connect("pressed", [&]() {
					std::string msg = "req/match_dec/";
					Client::instance()->setMatchAcc(false);
					Client::instance()->setMatchReq(false);
					Client::instance()->addReqToQueue(msg.append(Client::instance()->getRequester()).append("/").append(Client::instance()->getName()));
					invitesPanel->removeAllWidgets();
					invitesPanel->setEnabled(false);
					invitesPanel->setVisible(false);
				});
			}
		}
	}
	invitesPanel->setVisible(!invitesPanel->isVisible());
	invitesPanel->setEnabled(!invitesPanel->isEnabled());
}

void initGui() {
	tgui::Theme theme("themes/TransparentGrey.txt");
	tgui::Theme::setDefault(&theme);

	layoutB = tgui::VerticalLayout::create();
	gui.add(layoutB);
	layoutB->setSize(300, 600);
	layoutB->setPosition(50, 150);
	layoutB->getRenderer()->setSpaceBetweenWidgets(30);

	invButton = tgui::Button::create();
	gui.add(invButton);
	layoutB->add(invButton);
	invButton->setSize("100%", 40);
	invButton->setTextSize(20);
	invButton->getRenderer()->setTextColor(darkGrey);
	invButton->getRenderer()->setTextColorDown(lightBlue);
	invButton->getRenderer()->setBackgroundColor(lightBlue);
	invButton->getRenderer()->setBackgroundColorHover(darkBlue);
	invButton->getRenderer()->setBackgroundColorDown(darkBlue);
	invButton->getRenderer()->setBorderColor(darkBlue);
	invButton->setText("Invite");
	invButton->connect("pressed", &inv);


	playButton = tgui::Button::create();
	gui.add(playButton);
	layoutB->add(playButton);
	playButton->setSize("100%", 40);
	playButton->setTextSize(20);
	playButton->setText("Play");
	playButton->getRenderer()->setTextColor(darkGrey);
	playButton->getRenderer()->setTextColorDown(lightBlue);
	playButton->getRenderer()->setBackgroundColor(lightBlue);
	playButton->getRenderer()->setBackgroundColorHover(darkBlue);
	playButton->getRenderer()->setBackgroundColorDown(darkBlue);
	playButton->getRenderer()->setBorderColor(darkBlue);
	playButton->connect("pressed", &play);

	inputUserName = tgui::TextBox::create();
	layoutB->add(inputUserName);
	gui.add(inputUserName);
	inputUserName->setSize("100%", 40);
	inputUserName->setTextSize(21);
	inputUserName->getRenderer()->setBorders(2);
	inputUserName->getRenderer()->setBorderColor(darkBlue);
	inputUserName->getRenderer()->setBackgroundColor(lightBlue);
	inputUserName->getRenderer()->setTextColor(darkGrey);

	playerName = tgui::Label::create();
	gui.add(playerName);
	playerName->setSize(200, 30);
	playerName->setPosition(50, 850);
	playerName->setTextSize(20);
	std::string textp = "Player: ";
	playerName->setText(textp.append(Client::instance()->getName()));

	matchNameLabel = tgui::Label::create();
	gui.add(matchNameLabel);
	matchNameLabel->setSize(200, 30);
	matchNameLabel->setPosition(50, 890);
	matchNameLabel->setTextSize(20);
	std::string textm = "Match: ";
	matchNameLabel->setText(textm.append("none"));

	showInvites = tgui::Button::create();
	gui.add(showInvites);
	layoutB->add(showInvites);
	showInvites->setSize("100%", 40);
	showInvites->setText("Invites");
	showInvites->setTextSize(20);
	showInvites->getRenderer()->setTextColor(darkGrey);
	showInvites->getRenderer()->setTextColorDown(lightBlue);
	showInvites->getRenderer()->setBackgroundColor(lightBlue);
	showInvites->getRenderer()->setBackgroundColorHover(darkBlue);
	showInvites->getRenderer()->setBackgroundColorDown(darkBlue);
	showInvites->getRenderer()->setBorderColor(darkBlue);
	showInvites->connect("pressed", &getPlayerList);

	invitesPanel = tgui::ScrollablePanel::create();
	gui.add(invitesPanel);
	invitesPanel->setSize(200, 100);
	invitesPanel->setPosition(showInvites->getPosition().x, showInvites->getPosition().y + showInvites->getPosition().y + 10);
	invitesPanel->getRenderer()->setBackgroundColor(blue);
	invitesPanel->setVisible(false);
	invitesPanel->setEnabled(false);
}