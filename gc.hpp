
#include "config.hpp"

#include "wm.hpp"

#ifndef GAME_CONTROL
#define GAME_CONTROL

class GameControl {

	static bool mouseKey[3];
	static double mousePosition[2];

public:
	Scene *currentScene;
	
	GameControl();

	void initScene(int level);
	void renderScene();

	static void keyPressCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

};

#endif
