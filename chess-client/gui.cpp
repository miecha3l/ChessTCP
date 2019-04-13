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
			setLobbyUI(false);
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

void GuiManager::setIsReady(bool s)
{
	isReady->setEnabled(s);
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

void GuiManager::setInGameUI(bool s)
{
	soloGameLayout->setEnabled(s);
	soloGameLayout->setVisible(s);
}

void GuiManager::setMessageBox(bool s)
{
	showingMessageBox = s;
	messageBoxBody->setEnabled(s);
	messageBoxBody->setVisible(s);
}

void GuiManager::dissmissMsg() {
	std::cout << "dissmissing" << std::endl;
	messageBoxDissmised = true;
	showingMessageBox = false;
	messageBoxBody->setEnabled(false);
	messageBoxBody->setVisible(false);
}

void GuiManager::displayMessage(std::string s)
{
	messageInfo->setText(s);
	setMessageBox(true);
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
	isReady->setEnabled(false);
}



GuiManager::GuiManager() {

	latoBold.loadFromFile("assets/Lato-Bold.ttf");
	latoDefault.loadFromFile("assets/Lato-Regular.ttf");

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
	backToMenu = tgui::Button::create();
	play = tgui::Button::create();
	isReady = tgui::CheckBox::create();

	topRow->insert(0, inputUserName, "inputUserName");
	topRow->insert(1, sendInvite, "sendInvite");
	topRow->insert(2, showInvites, "showInvites");

	bottomRow->insert(0, backToMenu, "backToMenu");
	bottomRow->insert(1, play, "play");
	bottomRow->insert(2, isReady, "isReady");

	lobbyUILayout->insert(0, topRow, "topRow");
	lobbyUILayout->insert(1, invitesPanel, "invitesPanel");
	lobbyUILayout->insert(2, infoBoard, "infoBoard");
	lobbyUILayout->insert(3, bottomRow, "bottomRow");
	lobbyUILayout->setVisible(false);
	lobbyUILayout->setEnabled(false);
	mainUI.add(lobbyUILayout);



	soloGameLayout = tgui::VerticalLayout::create();
	quitGame = tgui::Button::create();
	resetGame = tgui::Button::create();

	soloGameLayout->insert(0, resetGame, "resetGame");
	soloGameLayout->insert(1, quitGame, "quitGame");
	soloGameLayout->setVisible(false);
	soloGameLayout->setEnabled(false);
	mainUI.add(soloGameLayout);

	/*multiGameLayout = tgui::VerticalLayout::create();
	disconnect = tgui::Button::create();
	forfeit = tgui::Button::create();
	currentTurn = tgui::Label::create();*/

	/*multiGameLayout->insert(0, soundsOn, "soundsOn");
	multiGameLayout->insert(1, soundsCheckBoxLabel, "soundsLabel");
	multiGameLayout->insert(2, highlightLegals, "highlightLegals");
	multiGameLayout->insert(3, highlithCheckBoxLabel, "highlightLabel");
	multiGameLayout->insert(4, currentTurn, "currentTurn");
	multiGameLayout->insert(5, forfeit, "forfeit");
	multiGameLayout->insert(6, disconnect, "disconnect");
	multiGameLayout->setVisible(false);
	multiGameLayout->setEnabled(false);*/



	//message box
	messageBoxBody = tgui::Panel::create();
	messageInfo = tgui::Label::create();
	dismiss = tgui::Button::create();

	messageBoxBody->add(messageInfo);
	messageBoxBody->add(dismiss);
	mainUI.add(messageBoxBody);
	messageBoxBody->setEnabled(false);
	messageBoxBody->setVisible(false);
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



bool GuiManager::isShowingMessageBox()
{
	return showingMessageBox;
}

void GuiManager::init()
{
	tgui::Theme theme("themes/TransparentGrey.txt");
	tgui::Theme::setDefault(&theme);

	//main UI setup
	mainUILayout->setSize(300, 160);
	mainUILayout->setPosition( 350, 500 );
	mainUILayout->getRenderer()->setSpaceBetweenWidgets(30);


	playSolo->setSize( "100%", "40%" );
	playSolo->setText("PLAY SOLO");
	playSolo->setTextSize(22);
	playSolo->getRenderer()->setTextColor(darkGrey);
	playSolo->getRenderer()->setFont(latoBold);
	playSolo->getRenderer()->setTextColorDown(lightBlue);
	playSolo->getRenderer()->setBackgroundColor(lightBlue);
	playSolo->getRenderer()->setBackgroundColorHover(darkBlue);
	playSolo->getRenderer()->setBackgroundColorDown(darkBlue);
	playSolo->getRenderer()->setBorderColor(darkBlue);
	playSolo->connect("pressed", [&]() {
		Client::instance()->setCurrentScreen(Client::Screen::OfflineGame);
		Client::instance()->setColor("white");
		setMenuUI(false);
		setInGameUI(true);
	});

	playWithFriend->setSize( "100%", "40%" );
	playWithFriend->setText("PLAY WITH FRIEND");
	playWithFriend->setTextSize(22);
	playWithFriend->getRenderer()->setTextColor(darkGrey);
	playWithFriend->getRenderer()->setFont(latoBold);
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
	inputUserName->getRenderer()->setFont(latoDefault);
	inputUserName->getRenderer()->setBorders(2);
	inputUserName->getRenderer()->setBorderColor(darkBlue);
	inputUserName->getRenderer()->setBackgroundColor(lightBlue);
	inputUserName->getRenderer()->setCaretColor(sf::Color::Black);
	inputUserName->getRenderer()->setTextColor(darkGrey);
	
	sendInvite->setSize("30%", "100%");
	sendInvite->setTextSize(20);
	sendInvite->getRenderer()->setTextColor(darkGrey);
	sendInvite->getRenderer()->setFont(latoDefault);
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
	showInvites->getRenderer()->setFont(latoDefault);
	showInvites->getRenderer()->setTextColorDown(lightBlue);
	showInvites->getRenderer()->setBackgroundColor(lightBlue);
	showInvites->getRenderer()->setBackgroundColorHover(darkBlue);
	showInvites->getRenderer()->setBackgroundColorDown(darkBlue);
	showInvites->getRenderer()->setBorderColor(darkBlue);
	showInvites->setText("Show pending");
	showInvites->connect("pressed", &GuiManager::getPendingInvites, this);

	invitesPanel->setSize("100%", "5%");
	invitesPanel->setPosition("0%", "7%");
	invitesPanel->getRenderer()->setFont(latoDefault);
	invitesPanel->getRenderer()->setBackgroundColor(darkBlue);
	invitesPanel->setVisible(false);
	invitesPanel->setEnabled(false);

	infoBoard->setSize("100%", "40%");
	infoBoard->setPosition("0%", "25%");
	infoBoard->getRenderer()->setFont(latoDefault);
	infoBoard->getRenderer()->setBackgroundColor(darkBlue);
	infoBoard->getRenderer()->setBorders(4);
	infoBoard->getRenderer()->setBorderColor(darkGrey);
	setInfoBoardInfo();

	backToMenu->setSize("20%", "40%");
	backToMenu->setPosition("0%", "30%");
	backToMenu->setTextSize(20);
	backToMenu->getRenderer()->setFont(latoBold);
	backToMenu->getRenderer()->setBackgroundColor(niceRed);
	backToMenu->getRenderer()->setBackgroundColorHover(darkerRed);
	backToMenu->getRenderer()->setBackgroundColorDown(darkerRed);
	backToMenu->getRenderer()->setTextColor(lightBlue);
	backToMenu->setText("BACK");
	backToMenu->connect("pressed", [&]() {
		Client::instance()->setCurrentScreen(Client::Screen::Menu);
		setLobbyUI(false);
		setMenuUI(true);
	});

	play->setSize("55%", "40%");
	play->setPosition("25%", "30%");
	play->setTextSize(25);
	play->getRenderer()->setFont(latoBold);
	play->getRenderer()->setBackgroundColor(niceGreen);
	play->getRenderer()->setBackgroundColorHover(darkerGreen);
	play->getRenderer()->setBackgroundColorDown(darkerGreen);
	play->getRenderer()->setTextColor(lightBlue);
	play->setText("P L A Y");
	play->connect("pressed", &GuiManager::playReq, this);

	isReady->setSize("10%", "40%");
	isReady->setPosition("85%", "30%");
	isReady->connect("checked", &GuiManager::toggleReady, this);
	isReady->connect("unchecked", &GuiManager::toggleReady, this);



	//solo UI setup
	soloGameLayout->setSize(250, 560);
	soloGameLayout->setPosition(700, 200);

	resetGame->setPosition("5%", "45%");
	resetGame->setSize("95%", "8%");
	resetGame->setText("Restart");
	resetGame->setTextSize(22);
	resetGame->getRenderer()->setTextColor(darkGrey);
	resetGame->getRenderer()->setFont(latoDefault);
	resetGame->getRenderer()->setTextColorDown(lightBlue);
	resetGame->getRenderer()->setBackgroundColor(lightBlue);
	resetGame->getRenderer()->setBackgroundColorHover(niceRed);
	resetGame->getRenderer()->setBackgroundColorDown(darkerRed);
	resetGame->getRenderer()->setBorderColor(darkerRed);
	resetGame->connect("pressed", [&]() {
		Client::instance()->resetSoloGame();
	});

	quitGame->setPosition("5%", "55%");
	quitGame->setSize("95%", "8%");
	quitGame->setText("Quit");
	quitGame->setTextSize(22);
	quitGame->getRenderer()->setTextColor(darkGrey);
	quitGame->getRenderer()->setFont(latoDefault);
	quitGame->getRenderer()->setTextColorDown(lightBlue);
	quitGame->getRenderer()->setBackgroundColor(lightBlue);
	quitGame->getRenderer()->setBackgroundColorHover(niceRed);
	quitGame->getRenderer()->setBackgroundColorDown(darkerRed);
	quitGame->getRenderer()->setBorderColor(darkerRed);
	quitGame->connect("pressed", [&]() {
		Client::instance()->resetSoloGame();
		Client::instance()->setColor("");
		Client::instance()->setGameState(Board());
		Client::instance()->setCurrentScreen(Client::Screen::Menu);
		setMenuUI(true);
		setInGameUI(false);
	});


	//message box
	messageBoxBody->setSize(300, 120);
	messageBoxBody->setPosition(350, 380);
	messageBoxBody->getRenderer()->setBackgroundColor(darkBlue);
	
	messageInfo->setAutoSize(true);
	messageInfo->setPosition("25%", "10%");
	messageInfo->setTextSize(20);
	messageInfo->getRenderer()->setFont(latoDefault);

	dismiss->setPosition("35%", "60%");
	dismiss->setSize("30%", "30%");
	dismiss->setText("O K");
	dismiss->setTextSize(18);
	dismiss->getRenderer()->setFont(latoDefault);
	dismiss->getRenderer()->setBackgroundColor(niceGreen);
	dismiss->getRenderer()->setBackgroundColorHover(darkerGreen);
	dismiss->getRenderer()->setBackgroundColorDown(darkerGreen);
	dismiss->getRenderer()->setBorders(0);;
	dismiss->connect("pressed", &GuiManager::dissmissMsg, this);
}


