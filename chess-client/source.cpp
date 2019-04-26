#include "Core.h"
int main() {
	srand(time(NULL));
	GuiManager::instance()->init();
	sf::Thread clientWindow(&windowThread);
	clientWindow.launch();
	Client::instance()->init();
	return 0;
}