
#include "config.hpp"

#include "globalVars.hpp"

bool GameControl::mouseKey[3] = {false, false, false};
double GameControl::mousePosition[2] = {0.0, 0.0};

GameControl::GameControl()
{

}

void GameControl::initScene(int level)
{
	int bgColor[] = { 40, 40, 40 };
	this->currentScene = new Scene(bgColor, level);

}

void GameControl::renderScene()
{
	this->currentScene->render();
}



void GameControl::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if(action == GLFW_PRESS)
	{
		if(button == GLFW_MOUSE_BUTTON_MIDDLE)
		{
			GameControl::mouseKey[1] = true;
			glfwGetCursorPos(window, &GameControl::mousePosition[0], &GameControl::mousePosition[1]);
		}
	}
	else if(action == GLFW_RELEASE)
	{
		if(button == GLFW_MOUSE_BUTTON_MIDDLE)
		{
			GameControl::mouseKey[1] = false;

			GameControl::mousePosition[0] = 0.0;
			GameControl::mousePosition[1] = 0.0;
		}
	}
}

void GameControl::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	gc->currentScene->defaultCamera->zoom(yoffset);
}

void GameControl::mouseMoveCallback(GLFWwindow* window, double xpos, double ypos)
{
	if(GameControl::mouseKey[1] == true)
	{
		double xMag = 180.0f / 1024.0f * (xpos - GameControl::mousePosition[0]);

		GameControl::mousePosition[0] = xpos;
		GameControl::mousePosition[1] = ypos;

		if(gc->currentScene->currentCameraMode == "helicopter")
			gc->currentScene->defaultCamera->rotateAroundTarget(xMag);
	}
}

void GameControl::keyPressCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	string camMode = gc->currentScene->currentCameraMode;

	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  	wm->close();
	else if(key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
	{
		if(camMode == "top" || camMode == "follow")
			gc->currentScene->player->turnZ(true);
		else
			gc->currentScene->player->turnX(true);
	}
	else if(key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
	{
		if(camMode == "top" || camMode == "follow")
			gc->currentScene->player->turnZ(false);
		else
			gc->currentScene->player->turnX(false);
	}
	else if(key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
	{
		if(camMode == "top" || camMode == "follow")
			gc->currentScene->player->turnX(true);
		else
			gc->currentScene->player->turnZ(false);
	}
	else if(key == GLFW_KEY_UP && action == GLFW_RELEASE)
	{
		if(camMode == "top" || camMode == "follow")
			gc->currentScene->player->turnX(false);
		else
			gc->currentScene->player->turnZ(true);
	}
	else if(key == GLFW_KEY_T && action == GLFW_RELEASE)
		gc->currentScene->changeCameraMode("top");
	else if(key == GLFW_KEY_P && action == GLFW_RELEASE)
		gc->currentScene->changeCameraMode("perspective");
	else if(key == GLFW_KEY_H && action == GLFW_RELEASE)
		gc->currentScene->changeCameraMode("helicopter");
	else if(key == GLFW_KEY_F && action == GLFW_RELEASE)
		gc->currentScene->changeCameraMode("follow");

	else if(key == GLFW_KEY_1 && action == GLFW_RELEASE)
 		gc->initScene(1);
	else if(key == GLFW_KEY_2 && action == GLFW_RELEASE)
 		gc->initScene(2);

}
