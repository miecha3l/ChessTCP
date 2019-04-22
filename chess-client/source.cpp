#include "Core.h"
int main() {
	GuiManager::instance()->init();
	sf::Thread clientWindow(&windowThread);
	clientWindow.launch();
	Client::instance()->init();

	return 0;
}