
#include "config.hpp"

#ifndef WINDOW_MANAGER
#define WINDOW_MANAGER

class WindowManager {

private:
	GLFWwindow *window;
	string name;
	int width;
	int height;

public:
	void setWindowOptions();
	WindowManager(string windowName, int width, int height);
	void setKeyCallback(GLFWkeyfun cbfunc);
	void setMouseCallback(GLFWcursorposfun cursorPosCallbackFunc, GLFWmousebuttonfun mouseCallbackFunc, GLFWscrollfun scrollCallbackFunc);

	void setCurrentContext();
	void close();
	GLFWwindow* getCurrentWindow();
	bool checkWindowClosed();
	void pollEvents();
	void swapBuffers();

};

#endif
