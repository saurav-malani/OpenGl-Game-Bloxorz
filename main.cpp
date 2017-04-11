
#include "config.hpp"

#include "globalVars.hpp"

WindowManager *wm;
GameControl *gc;

int main()
{
	wm = new WindowManager("Game", 1024, 768);
	wm->setCurrentContext();
	wm->setKeyCallback(GameControl::keyPressCallback);
	wm->setMouseCallback(GameControl::mouseMoveCallback, GameControl::mouseButtonCallback, GameControl::scrollCallback);

	/* initialise glew */
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
		return -1;

	gc = new GameControl();
	gc->initScene(1);

	while(!wm->checkWindowClosed())
	{
		wm->pollEvents();

		gc->renderScene();

		wm->swapBuffers();
	}

	return 0;
}
