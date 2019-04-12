#include "gui.h"
#include "Client.h"

GuiManager* GuiManager::gm = NULL;

void GuiManager::inv() {
	std::string body = "req/match/";
	if (inputUserName->getText().getSize() > 0) {
		Client::instance()->setRequester(inputUserName->getText().toAnsiString());
		Client::instance()->addReqToQueue(body.append(inputUserName->getText().toAnsiString()).append("/").append(Client::instance()->getName()));
		inputUserName->setText("");
	}
}

void GuiManager::playReq() {
	std::string body = "req/play/";
	if (!Client::instance()->getMatchName().empty()) {
		if (Client::instance()->isPlayerReady() && Client::instance()->isPlayersMatchReady()) {
			Client::instance()->addReqToQueue(body.append(Client::instance()->getMatchName()).append("/").append(Client::instance()->getName()));
		}
	}
}

void GuiManager::setInfoBoardInfo()
{
	infoBoard->removeAllWidgets();

	if (!Client::instance()->getMatchName().empty()) {
		infoBoard->setSize("100%", "40%");
		auto matchLabel = tgui::Label::create();
		infoBoard->add(matchLabel);
		matchLabel->setSize("100%", "25%");
		matchLabel->setPosition("0%", "67%");
		std::string text = "Match: ";
		std::string ready = Client::instance()->isPlayersMatchReady() ? "    Is ready" : "    Not ready";
		matchLabel->setText(text.append(Client::instance()->getMatchName()).append(ready));
		matchLabel->setTextSize(28);
		matchLabel->getRenderer()->setTextColor(niceRed);
		if (Client::instance()->isPlayersMatchReady()) matchLabel->getRenderer()->setTextColor(darkerGreen);
	}
	else {
		infoBoard->setSize("100%", "20%");
	}

	auto playerLabel = tgui::Label::create();
	infoBoard->add(playerLabel);
	playerLabel->setSize("100%", "25%");
	playerLabel->setPosition("0%", "23%");
	std::string text = "You: ";
	std::string ready = Client::instance()->isPlayerReady() ? "    Is ready" : "    Not ready";
	playerLabel->setText(text.append(Client::instance()->getName()).append(ready));
	playerLabel->setTextSize(28);
	playerLabel->getRenderer()->setTextColor(niceRed);
	if (Client::instance()->isPlayerReady()) playerLabel->getRenderer()->setTextColor(darkerGreen);
}

void GuiManager::setLobbyUI(bool s)
{
	lobbyUILayout->setEnabled(s);
	lobbyUILayout->setVisible(s);
}

void GuiManager::setMenuUI(bool s)
{
	mainUILayout->setEnabled(s);
	mainUILayout->setVisible(s);
}

void GuiManager::getPendingInvites() {
	showInvites->getRenderer()->setBackgroundColor(lightBlue);
	showInvites->getRenderer()->setBorderColor(darkBlue);
	showInvites->getRenderer()->setBackgroundColorHover(darkBlue);
	if (Client::instance()->isMatchReqPending()) {
		Client::instance()->setOnlinePlayersListUpToDate(false);
		std::string body = "req/get_plist/server/";
		Client::instance()->addReqToQueue(body.append(Client::instance()->getName()));
		invitesPanel->removeAllWidgets();

		while (!Client::instance()->isPlayersListUpToDate()) {}

		invitesPanel->setSize("100%", Client::instance()->getOnlinePlayersList().size() * 25);

		for (auto p : Client::instance()->getOnlinePlayersList()) {
			if (p == Client::instance()->getName()) continue;
			if (Client::instance()->getRequester() == p) {
				auto playersLabel = tgui::Label::create();
				invitesPanel->add(playersLabel);
				int yPos = (invitesPanel->getWidgets().size() - 1) * 25;
				playersLabel->setSize("45%", 25);
				playersLabel->setPosition("0%", yPos + 10);
				playersLabel->getRenderer()->setTextColor(darkGrey);
				playersLabel->setTextSize(17);
				playersLabel->setText(p);


				auto acceptButton = tgui::Button::create();
				invitesPanel->add(acceptButton);
				acceptButton->setSize("25%", 25);
				acceptButton->setPosition("35%", yPos + 10);
				acceptButton->setText("Accept");
				acceptButton->getRenderer()->setTextColor(darkGrey);
				acceptButton->getRenderer()->setTextColorDown(lightBlue);
				acceptButton->getRenderer()->setBackgroundColor(lightBlue);
				acceptButton->getRenderer()->setBackgroundColorHover(niceGreen);
				acceptButton->getRenderer()->setBackgroundColorDown(darkerGreen);
				acceptButton->getRenderer()->setBorderColor(darkBlue);
				acceptButton->setTextSize(13);
				acceptButton->connect("pressed", [&]() {
					if (Client::instance()->getMatchName().empty()) {
						std::string msg = "req/match_acc/";
						Client::instance()->setMatchAcc(true);
						Client::instance()->setMatchReq(false);
						Client::instance()->setMatchName(Client::instance()->getRequester());
						Client::instance()->addReqToQueue(msg.append(Client::instance()->getMatchName()).append("/").append(Client::instance()->getName()));
						setInfoBoardInfo();
						invitesPanel->removeAllWidgets();
						invitesPanel->setEnabled(false);
						invitesPanel->setVisible(false);
					}
				});

				auto declineButton = tgui::Button::create();
				invitesPanel->add(declineButton);
				declineButton->setSize("25%", 25);
				declineButton->setPosition("65%", yPos + 10);
				declineButton->setText("Decline");
				declineButton->setTextSize(13);
				declineButton->getRenderer()->setTextColor(darkGrey);
				declineButton->getRenderer()->setTextColorDown(lightBlue);
				declineButton->getRenderer()->setBackgroundColor(lightBlue);
				declineButton->getRenderer()->setBackgroundColorHover(niceRed);
				declineButton->getRenderer()->setBackgroundColorDown(darkerRed);
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

void GuiManager::toggleReady()
{
	Client::instance()->setIsReady(isReady->isChecked());
	setInfoBoardInfo();
	std::string req = isReady->isChecked() ? "req/is_ready/" : "req/is_notready/";
	Client::instance()->addReqToQueue(req.append(Client::instance()->getMatchName()).append("/").append(Client::instance()->getName()));
}



GuiManager::GuiManager() {
	//main UI init
	mainUILayout = tgui::VerticalLayout::create();
	playSolo = tgui::Button::create();
	playWithFriend = tgui::Button::create();

	
	mainUILayout->insert(0, playWithFriend, "Play with friend");
	mainUILayout->insert(1, playSolo, "Play solo");
	mainUI.add(mainUILayout);

	//lobby UI init
	lobbyUILayout = tgui::VerticalLayout::create();
	topRow = tgui::HorizontalLayout::create();
	bottomRow = tgui::HorizontalLayout::create();
	sendInvite = tgui::Button::create();
	showInvites = tgui::Button::create();
	invitesPanel = tgui::Panel::create();
	inputUserName = tgui::TextBox::create();
	infoBoard = tgui::Panel::create();
	play = tgui::Button::create();
	isReady = tgui::CheckBox::create();

	topRow->insert(0, inputUserName, "inputUserName");
	topRow->insert(1, sendInvite, "sendInvite");
	topRow->insert(2, showInvites, "showInvites");

	bottomRow->insert(0, play, "play");
	bottomRow->insert(1, isReady, "isReady");

	lobbyUILayout->insert(0, topRow, "topRow");
	lobbyUILayout->insert(1, invitesPanel, "invitesPanel");
	lobbyUILayout->insert(2, infoBoard, "infoBoard");
	lobbyUILayout->insert(3, bottomRow, "bottomRow");
	lobbyUILayout->setVisible(false);
	lobbyUILayout->setEnabled(false);
	mainUI.add(lobbyUILayout);
}

GuiManager * GuiManager::instance()
{
	if(gm == NULL) gm = new GuiManager();
	else return gm;
}

tgui::Gui * GuiManager::getMainUIHandle()
{
	tgui::Gui *p = &mainUI;
	return p;
}

void GuiManager::highlightPendingButton()
{
	showInvites->getRenderer()->setBackgroundColor(niceRed);
	showInvites->getRenderer()->setBorderColor(darkerRed);
	showInvites->getRenderer()->setBackgroundColorHover(darkerRed);
}



void GuiManager::init()
{
	tgui::Theme theme("themes/TransparentGrey.txt");
	tgui::Theme::setDefault(&theme);

	//main UI setup
	mainUILayout->setSize(300, 160);
	mainUILayout->setPosition( 350, 650 );
	mainUILayout->getRenderer()->setSpaceBetweenWidgets(30);


	playSolo->setSize( "100%", "40%" );
	playSolo->setText("PLAY SOLO");
	playSolo->setTextSize(22);
	playSolo->getRenderer()->setTextColor(darkGrey);
	playSolo->getRenderer()->setTextStyle(sf::Text::Bold);
	playSolo->getRenderer()->setTextColorDown(lightBlue);
	playSolo->getRenderer()->setBackgroundColor(lightBlue);
	playSolo->getRenderer()->setBackgroundColorHover(darkBlue);
	playSolo->getRenderer()->setBackgroundColorDown(darkBlue);
	playSolo->getRenderer()->setBorderColor(darkBlue);
	playSolo->connect("pressed", [&]() {
		//switch screen to solo game settings
	});

	playWithFriend->setSize( "100%", "40%" );
	playWithFriend->setText("PLAY WITH FRIEND");
	playWithFriend->setTextSize(22);
	playWithFriend->getRenderer()->setTextColor(darkGrey);
	playWithFriend->getRenderer()->setTextStyle(sf::Text::Bold);
	playWithFriend->getRenderer()->setTextColorDown(lightBlue);
	playWithFriend->getRenderer()->setBackgroundColor(lightBlue);
	playWithFriend->getRenderer()->setBackgroundColorHover(darkBlue);
	playWithFriend->getRenderer()->setBackgroundColorDown(darkBlue);
	playWithFriend->getRenderer()->setBorderColor(darkBlue);
	playWithFriend->connect("pressed", [&]() {
		mainUILayout->setEnabled(false);
		mainUILayout->setVisible(false);
		lobbyUILayout->setEnabled(true);
		lobbyUILayout->setVisible(true);
		Client::instance()->setCurrentScreen(Client::Screen::Lobby);
		setInfoBoardInfo();
	});




	//lobbyUI setup
	lobbyUILayout->setSize(600, 800);
	lobbyUILayout->setPosition(200, 80);

	topRow->setSize("100%", "6%");
	topRow->getRenderer()->setSpaceBetweenWidgets(10);

	inputUserName->setSize("30%", "100%");
	inputUserName->setTextSize(30);
	inputUserName->getRenderer()->setBorders(2);
	inputUserName->getRenderer()->setBorderColor(darkBlue);
	inputUserName->getRenderer()->setBackgroundColor(lightBlue);
	inputUserName->getRenderer()->setCaretColor(sf::Color::Black);
	inputUserName->getRenderer()->setTextColor(darkGrey);
	
	sendInvite->setSize("30%", "100%");
	sendInvite->setTextSize(20);
	sendInvite->getRenderer()->setTextColor(darkGrey);
	sendInvite->getRenderer()->setTextColorDown(lightBlue);
	sendInvite->getRenderer()->setBackgroundColor(lightBlue);
	sendInvite->getRenderer()->setBackgroundColorHover(darkBlue);
	sendInvite->getRenderer()->setBackgroundColorDown(darkBlue);
	sendInvite->getRenderer()->setBorderColor(darkBlue);
	sendInvite->setText("Invite");
	sendInvite->connect("pressed", &GuiManager::inv, this);

	showInvites->setSize("35%", "100%");
	showInvites->setTextSize(20);
	showInvites->getRenderer()->setTextColor(darkGrey);
	showInvites->getRenderer()->setTextColorDown(lightBlue);
	showInvites->getRenderer()->setBackgroundColor(lightBlue);
	showInvites->getRenderer()->setBackgroundColorHover(darkBlue);
	showInvites->getRenderer()->setBackgroundColorDown(darkBlue);
	showInvites->getRenderer()->setBorderColor(darkBlue);
	showInvites->setText("Show pending");
	showInvites->connect("pressed", &GuiManager::getPendingInvites, this);

	invitesPanel->setSize("100%", "5%");
	invitesPanel->setPosition("0%", "7%");
	invitesPanel->getRenderer()->setBackgroundColor(darkBlue);
	invitesPanel->setVisible(false);
	invitesPanel->setEnabled(false);

	infoBoard->setSize("100%", "40%");
	infoBoard->setPosition("0%", "25%");
	infoBoard->getRenderer()->setBackgroundColor(darkBlue);
	infoBoard->getRenderer()->setBorders(4);
	infoBoard->getRenderer()->setBorderColor(darkGrey);
	setInfoBoardInfo();

	play->setSize("70%", "40%");
	play->setPosition("8%", "30%");
	play->setTextSize(25);
	play->getRenderer()->setBackgroundColor(niceGreen);
	play->getRenderer()->setBackgroundColorHover(darkerGreen);
	play->getRenderer()->setBackgroundColorDown(darkerGreen);
	play->getRenderer()->setTextColor(lightBlue);
	play->getRenderer()->setTextStyle(sf::Text::Bold);
	play->setText("P L A Y");
	play->connect("pressed", &GuiManager::playReq, this);

	isReady->setSize("10%", "40%");
	isReady->setPosition("85%", "30%");
	isReady->connect("checked", &GuiManager::toggleReady, this);
	isReady->connect("unchecked", &GuiManager::toggleReady, this);
}


