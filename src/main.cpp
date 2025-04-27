#include "Game.h"

int main(int argc, char** argv) {
	Game g("src/config.txt");
	
	g.run();

	std::cout << "Press any key to Continue...\n";
	std::getchar();

	return 0;
}