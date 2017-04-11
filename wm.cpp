
#include "config.hpp"

#include "globalVars.hpp"

WindowManager::WindowManager(string windowName, int width, int height)
{
	glfwInit();

	this->width = width;
	this->height = height;
	this->name = windowName;

	this->setWindowOptions();
	this->window = glfwCreateWindow(width, height, windowName.c_str(), NULL, NULL);
}

void WindowManager::setKeyCallback(GLFWkeyfun cbfunc)
{
	glfwSetKeyCallback(this->window, cbfunc);
}

void WindowManager::setMouseCallback(GLFWcursorposfun cursorPosCallbackFunc, GLFWmousebuttonfun mouseCallbackFunc, GLFWscrollfun scrollCallbackFunc)
{
	glfwSetCursorPosCallback(this->window, cursorPosCallbackFunc);
	glfwSetMouseButtonCallback(this->window, mouseCallbackFunc);
	glfwSetScrollCallback(this->window, scrollCallbackFunc);
}

void WindowManager::setWindowOptions()
{
	glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void WindowManager::close()
{
	glfwSetWindowShouldClose(this->window, GL_TRUE);
}

void WindowManager::setCurrentContext()
{
	glfwMakeContextCurrent(this->window);
}

GLFWwindow* WindowManager::getCurrentWindow()
{
	return this->window;
}

bool WindowManager::checkWindowClosed()
{
	return glfwWindowShouldClose(this->window);
}

void WindowManager::pollEvents()
{
	glfwPollEvents();
}

void WindowManager::swapBuffers()
{
	glfwSwapBuffers(this->window);
}
