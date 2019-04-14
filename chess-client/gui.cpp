#include "gui.h"
#include "Client.h"
#include "Windows.h"

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
		Client::instance()->addReqToQueue(body.append(Client::instance()->getMatchName()).append("/").append(Client::instance()->getName()));
		setLobbyUI(false);
	}
}

void GuiManager::setInfoBoardInfo()
{
	infoBoard->removeAllWidgets();
	setInfoBoard(false);
	if (!Client::instance()->getMatchName().empty()) {
		infoBoard->setSize("100%", "40%");
		auto matchLabel = tgui::Label::create();
		infoBoard->add(matchLabel);
		matchLabel->setSize("80%", "25%");
		matchLabel->setPosition("0%", "67%");
		std::string text = "Match: ";
		matchLabel->setText(text.append(Client::instance()->getMatchName()));
		matchLabel->setTextSize(28);
		matchLabel->getRenderer()->setTextColor(darkGrey);

		auto unmatchButton = tgui::Button::create();
		infoBoard->add(unmatchButton);
		unmatchButton->setSize("8%", "10%");
		unmatchButton->setPosition("85%", "70%");
		unmatchButton->setText("X");
		unmatchButton->setTextSize(20);
		unmatchButton->getRenderer()->setBackgroundColor(niceRed);
		unmatchButton->getRenderer()->setBackgroundColorHover(darkerRed);
		unmatchButton->getRenderer()->setBackgroundColorDown(darkerRed);
		unmatchButton->getRenderer()->setFont(latoBold);
		unmatchButton->getRenderer()->setTextColor(lightBlue);
		unmatchButton->getRenderer()->setBorders(false);
		unmatchButton->connect("pressed", [&](){
			if (!Client::instance()->getMatchName().empty()) {
				std::string msg = "req/unmatch/";
				Client::instance()->addReqToQueue(msg.append(Client::instance()->getMatchName()).append("/").append(Client::instance()->getName()));
			}
		});
		
	}
	else {
		infoBoard->setSize("100%", "20%");
	}

	auto playerLabel = tgui::Label::create();
	infoBoard->add(playerLabel);
	playerLabel->setSize("80%", "25%");
	playerLabel->setPosition("0%", "23%");
	std::string text = "You: ";
	playerLabel->setText(text.append(Client::instance()->getName()));
	playerLabel->setTextSize(28);
	playerLabel->getRenderer()->setTextColor(darkGrey);
	setInfoBoard(true);
}

void GuiManager::setInfoBoard(bool s)
{
	infoBoard->setEnabled(s);
	infoBoard->setVisible(s);
}

void GuiManager::setLobbyUI(bool s)
{
	setInfoBoardInfo();
	lobbyUILayout->setEnabled(s);
	lobbyUILayout->setVisible(s);
}
void GuiManager::setLobbyUI(bool s1, bool s2)
{
	lobbyUILayout->setEnabled(s1);
	lobbyUILayout->setVisible(s2);
}

void GuiManager::setMenuUI(bool s)
{
	mainUILayout->setEnabled(s);
	mainUILayout->setVisible(s);
}
void GuiManager::setMenuUI(bool s1, bool s2)
{
	mainUILayout->setEnabled(s1);
	mainUILayout->setVisible(s2);
}

void GuiManager::setSoloGameUI(bool s)
{
	soloGameLayout->setEnabled(s);
	soloGameLayout->setVisible(s);
}

void GuiManager::setSoloGameUI(bool s1, bool s2)
{
	soloGameLayout->setEnabled(s1);
	soloGameLayout->setVisible(s2);
}

void GuiManager::setMultiGameUI(bool s)
{
	multiGameLayout->setEnabled(s);
	multiGameLayout->setVisible(s);
}

void GuiManager::setMultiGameUI(bool s1, bool s2)
{
	multiGameLayout->setEnabled(s1);
	multiGameLayout->setVisible(s2);
}

void GuiManager::setCurrentTurnLabel(std::string s)
{
	std::string con = "Current turn: ";
	currentTurn->setVisible(false);
	currentTurn->setText(con.append(s));
	currentTurn->setVisible(true);
}

void GuiManager::setDrawLock(bool s)
{
	lockGuiDraw = s;
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
	switch (Client::instance()->getCurrentScreen()) {
	case Client::Screen::Lobby:
		setLobbyUI(true);
		setMultiGameUI(false);
		setSoloGameUI(false);
		setMenuUI(false);
		if (Client::instance()->getName().empty()) {
			Client::instance()->setCurrentScreen(Client::Screen::Menu);
			setLobbyUI(false);
			setMenuUI(true);
		}
		break;

	case Client::Screen::Menu:
		setMenuUI(true);
		setMultiGameUI(false);
		setSoloGameUI(false);
		setLobbyUI(false);
		break;

	case Client::Screen::OfflineGame:
		setSoloGameUI(true);
		setMultiGameUI(false);
		setMenuUI(false);
		setLobbyUI(false);
		break;

	case Client::Screen::OnlineGame:
		setLobbyUI(true);
		setMenuUI(false);
		setSoloGameUI(false);
		Client::instance()->setCurrentScreen(Client::Screen::Lobby);
		break;
	}
}


void GuiManager::displayMessage(std::string s)
{
	messageInfo->setText(s);
	setMessageBox(true);
}

void GuiManager::getPendingInvites() {
	showInvites->getRenderer()->setBackgroundColor(lightBlue);
	showInvites->getRenderer()->setBorderColor(borders);
	showInvites->getRenderer()->setBackgroundColorHover(darkBlue);
	if (Client::instance()->isMatchReqPending()) {
		Client::instance()->setOnlinePlayersListUpToDate(false);
		std::string body = "req/get_plist/server/";
		Client::instance()->addReqToQueue(body.append(Client::instance()->getName()));
		invitesPanel->removeAllWidgets();

		invitesPanel->setSize("100%", 50);
		while(!Client::instance()->isPlayersListUpToDate()){}

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
				acceptButton->getRenderer()->setTextColor(lightBlue);
				acceptButton->getRenderer()->setTextColorDown(lightBlue);
				acceptButton->getRenderer()->setBackgroundColor(darkGrey);
				acceptButton->getRenderer()->setBackgroundColorHover(niceGreen);
				acceptButton->getRenderer()->setBackgroundColorDown(darkerGreen);
				acceptButton->getRenderer()->setBorders(false);
				acceptButton->setTextSize(13);
				acceptButton->connect("pressed", [&]() {
					if (Client::instance()->getMatchName().empty()) {
						std::string msg = "req/match_acc/";
						Client::instance()->addReqToQueue(msg.append(Client::instance()->getRequester()).append("/").append(Client::instance()->getName()));
						invitesPanel->setEnabled(false);
						invitesPanel->setVisible(false);
						invitesPanel->removeAllWidgets();
						setInfoBoardInfo();
					}
				});

				auto declineButton = tgui::Button::create();
				invitesPanel->add(declineButton);
				declineButton->setSize("25%", 25);
				declineButton->setPosition("65%", yPos + 10);
				declineButton->setText("Decline");
				declineButton->setTextSize(13);
				declineButton->getRenderer()->setTextColor(lightBlue);
				declineButton->getRenderer()->setTextColorDown(lightBlue);
				declineButton->getRenderer()->setBackgroundColor(darkGrey);
				declineButton->getRenderer()->setBackgroundColorHover(niceRed);
				declineButton->getRenderer()->setBackgroundColorDown(darkerRed);
				declineButton->getRenderer()->setBorders(false);
				declineButton->connect("pressed", [&]() {
					std::string msg = "req/match_dec/";
					Client::instance()->setMatchReq(false);
					Client::instance()->addReqToQueue(msg.append(Client::instance()->getRequester()).append("/").append(Client::instance()->getName()));
					invitesPanel->setEnabled(false);
					invitesPanel->setVisible(false);
					invitesPanel->removeAllWidgets();
				});
			}
		}
	}
	invitesPanel->setVisible(!invitesPanel->isVisible());
	invitesPanel->setEnabled(!invitesPanel->isEnabled());
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

	topRow->insert(0, inputUserName, "inputUserName");
	topRow->insert(1, sendInvite, "sendInvite");
	topRow->insert(2, showInvites, "showInvites");

	bottomRow->insert(0, backToMenu, "backToMenu");
	bottomRow->insert(1, play, "play");

	lobbyUILayout->insert(0, topRow, "topRow");
	lobbyUILayout->insert(1, invitesPanel, "invitesPanel");
	lobbyUILayout->insert(2, infoBoard, "infoBoard");
	lobbyUILayout->insert(3, bottomRow, "bottomRow");
	lobbyUILayout->setVisible(false);
	lobbyUILayout->setEnabled(false);
	mainUI.add(lobbyUILayout);



	//solo game UI init
	soloGameLayout = tgui::VerticalLayout::create();
	quitGame = tgui::Button::create();
	resetGame = tgui::Button::create();

	soloGameLayout->insert(0, resetGame, "resetGame");
	soloGameLayout->insert(1, quitGame, "quitGame");
	soloGameLayout->setVisible(false);
	soloGameLayout->setEnabled(false);
	mainUI.add(soloGameLayout);



	//multi game UI init
	multiGameLayout = tgui::VerticalLayout::create();
	disconnect = tgui::Button::create();
	forfeit = tgui::Button::create();
	currentTurn = tgui::Label::create();

	multiGameLayout->insert(0, currentTurn, "currentTurn");
	multiGameLayout->insert(1, forfeit, "forfeit");
	multiGameLayout->insert(2, disconnect, "disconnect");
	multiGameLayout->setVisible(false);
	multiGameLayout->setEnabled(false);
	mainUI.add(multiGameLayout);



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

void GuiManager::drawGui()
{
	if (!lockGuiDraw) {
		mainUI.draw();
	}
}

bool GuiManager::mainUIHandleFree()
{
	return !updatingInfoBoard;
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
	playSolo->getRenderer()->setBorders(false);
	playSolo->connect("pressed", [&]() {
		Client::instance()->setCurrentScreen(Client::Screen::OfflineGame);
		Client::instance()->setColor("white");
		setMenuUI(false);
		setSoloGameUI(true);
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
	playWithFriend->getRenderer()->setBorders(false);
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
	inputUserName->getRenderer()->setBorders(3);
	inputUserName->getRenderer()->setBorderColor(borders);
	inputUserName->getRenderer()->setBackgroundColor(lightBlue);
	inputUserName->getRenderer()->setCaretColor(sf::Color::Black);
	inputUserName->getRenderer()->setTextColor(borders);
	
	sendInvite->setSize("30%", "100%");
	sendInvite->setTextSize(20);
	sendInvite->getRenderer()->setTextColor(darkGrey);
	sendInvite->getRenderer()->setFont(latoDefault);
	sendInvite->getRenderer()->setTextColorDown(lightBlue);
	sendInvite->getRenderer()->setBackgroundColor(lightBlue);
	sendInvite->getRenderer()->setBackgroundColorHover(darkBlue);
	sendInvite->getRenderer()->setBackgroundColorDown(darkBlue);
	sendInvite->getRenderer()->setBorderColor(borders);
	sendInvite->getRenderer()->setBorders(false);
	sendInvite->setText("Invite");
	sendInvite->connect("pressed", &GuiManager::inv, this);

	showInvites->setSize("30%", "100%");
	showInvites->setTextSize(20);
	showInvites->getRenderer()->setTextColor(darkGrey);
	showInvites->getRenderer()->setFont(latoDefault);
	showInvites->getRenderer()->setTextColorDown(lightBlue);
	showInvites->getRenderer()->setBackgroundColor(lightBlue);
	showInvites->getRenderer()->setBackgroundColorHover(darkBlue);
	showInvites->getRenderer()->setBackgroundColorDown(darkBlue);
	showInvites->getRenderer()->setBorderColor(borders);
	showInvites->getRenderer()->setBorders(false);
	showInvites->setText("Show pending");
	showInvites->connect("pressed", &GuiManager::getPendingInvites, this);

	invitesPanel->setSize("100%", "5%");
	invitesPanel->setPosition("0%", "7%");
	invitesPanel->getRenderer()->setFont(latoDefault);
	invitesPanel->getRenderer()->setBackgroundColor(lightBlue);
	invitesPanel->getRenderer()->setBorderColor(borders);
	invitesPanel->setVisible(false);
	invitesPanel->setEnabled(false);

	infoBoard->setSize("100%", "40%");
	infoBoard->setPosition("0%", "25%");
	infoBoard->getRenderer()->setFont(latoDefault);
	infoBoard->getRenderer()->setBackgroundColor(lightBlue);
	infoBoard->getRenderer()->setBorders(4);
	infoBoard->getRenderer()->setBorderColor(borders);

	backToMenu->setSize("20%", "40%");
	backToMenu->setPosition("0%", "30%");
	backToMenu->setTextSize(20);
	backToMenu->getRenderer()->setFont(latoBold);
	backToMenu->getRenderer()->setBackgroundColor(niceRed);
	backToMenu->getRenderer()->setBackgroundColorHover(darkerRed);
	backToMenu->getRenderer()->setBackgroundColorDown(darkerRed);
	backToMenu->getRenderer()->setTextColor(lightBlue);
	backToMenu->getRenderer()->setBorders(false);
	backToMenu->setText("BACK");
	backToMenu->connect("pressed", [&]() {
		Client::instance()->setCurrentScreen(Client::Screen::Menu);
		setLobbyUI(false);
		setMenuUI(true);
		if (!Client::instance()->getMatchName().empty()) {
			std::string msg = "req/unmatch/";
			Client::instance()->addReqToQueue(msg.append(Client::instance()->getMatchName()).append("/").append(Client::instance()->getName()));
		}
	});

	play->setSize("55%", "40%");
	play->setPosition("25%", "30%");
	play->setTextSize(25);
	play->getRenderer()->setFont(latoBold);
	play->getRenderer()->setBackgroundColor(lightBlue);
	play->getRenderer()->setBackgroundColorHover(darkBlue);
	play->getRenderer()->setBackgroundColorDown(darkBlue);
	play->getRenderer()->setTextColor(darkGrey);
	play->getRenderer()->setBorders(false);
	play->setText("P L A Y");
	play->connect("pressed", &GuiManager::playReq, this);





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
	resetGame->getRenderer()->setBorders(false);
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
	quitGame->getRenderer()->setBorders(false);
	quitGame->connect("pressed", [&]() {
		Client::instance()->resetSoloGame();
		Client::instance()->setColor("");
		Client::instance()->setGameState(Board());
		Client::instance()->setCurrentScreen(Client::Screen::Menu);
		setMenuUI(true);
		setSoloGameUI(false);
	});



	//multi UI setup
	multiGameLayout->setSize(250, 560);
	multiGameLayout->setPosition(700, 200);

	currentTurn->setText("Current turn: White");
	currentTurn->setTextSize(22);
	currentTurn->getRenderer()->setFont(latoBold);
	currentTurn->getRenderer()->setTextColor(lightBlue);
	currentTurn->setSize("95%", "7%");
	currentTurn->setPosition("5%", "5%");

	forfeit->setPosition("5%", "45%");
	forfeit->setSize("95%", "8%");
	forfeit->setText("Forfeit");
	forfeit->setTextSize(22);
	forfeit->getRenderer()->setTextColor(darkGrey);
	forfeit->getRenderer()->setFont(latoDefault);
	forfeit->getRenderer()->setTextColorDown(lightBlue);
	forfeit->getRenderer()->setBackgroundColor(lightBlue);
	forfeit->getRenderer()->setBackgroundColorHover(niceRed);
	forfeit->getRenderer()->setBackgroundColorDown(darkerRed);
	forfeit->getRenderer()->setBorders(false);
	forfeit->connect("pressed", [&]() {
		//send forfeit message to server
	});

	disconnect->setPosition("5%", "55%");
	disconnect->setSize("95%", "8%");
	disconnect->setText("Disconnect");
	disconnect->setTextSize(22);
	disconnect->getRenderer()->setTextColor(darkGrey);
	disconnect->getRenderer()->setFont(latoDefault);
	disconnect->getRenderer()->setTextColorDown(lightBlue);
	disconnect->getRenderer()->setBackgroundColor(lightBlue);
	disconnect->getRenderer()->setBackgroundColorHover(niceRed);
	disconnect->getRenderer()->setBackgroundColorDown(darkerRed);
	disconnect->getRenderer()->setBorders(false);
	disconnect->connect("pressed", [&]() {
		std::string req = "req/disconnect/";
		Client::instance()->addReqToQueue(req.append(Client::instance()->getMatchName()).append("/").append(Client::instance()->getName()));
	});



	//message box
	messageBoxBody->setSize(300, 120);
	messageBoxBody->setPosition(350, 380);
	messageBoxBody->getRenderer()->setBackgroundColor(lightBlue);
	messageBoxBody->getRenderer()->setBorders(2);
	messageBoxBody->getRenderer()->setBorderColor(borders);
	
	messageInfo->setAutoSize(true);
	messageInfo->setPosition("25%", "10%");
	messageInfo->setTextSize(20);
	messageInfo->getRenderer()->setFont(latoDefault);

	dismiss->setPosition("35%", "60%");
	dismiss->setSize("30%", "30%");
	dismiss->setText("O K");
	dismiss->setTextSize(18);
	dismiss->getRenderer()->setTextColor(lightBlue);
	dismiss->getRenderer()->setFont(latoDefault);
	dismiss->getRenderer()->setBackgroundColor(darkGrey);
	dismiss->getRenderer()->setBackgroundColorHover(sf::Color(20,20,20,255));
	dismiss->getRenderer()->setBackgroundColorDown(sf::Color(15, 15, 15, 255));
	dismiss->getRenderer()->setBorders(false);
	dismiss->connect("pressed", &GuiManager::dissmissMsg, this);
}