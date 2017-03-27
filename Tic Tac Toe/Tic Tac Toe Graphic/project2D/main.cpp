#include "Application2D.h"

//TODO cleanup code

int main() {
	
	auto app = new Application2D();
	app->run("AIE", 960, 720, false);
	delete app;

	return 0;
}