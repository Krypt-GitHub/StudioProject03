
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

#include "../StudioProject/SceneManager.h"
#include "../StudioProject/MenuScene.h"
#include "../StudioProject/GameScene.h"
#include "../StudioProject/Level1Scene.h"



GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
double Application::mouse_last_x = 0.0, Application::mouse_last_y = 0.0,
	   Application::mouse_current_x = 0.0, Application::mouse_current_y = 0.0,
	   Application::mouse_diff_x = 0.0, Application::mouse_diff_y = 0.0;
double Application::camera_yaw = 0.0, Application::camera_pitch = 0.0;
bool Application::m_bShouldUpdate = false;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

bool Application::GetKeyDown(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool Application::GetMouseUpdate()
{
    glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);

	// Calculate the difference in positions
	mouse_diff_x = mouse_current_x - mouse_last_x;
	mouse_diff_y = mouse_current_y - mouse_last_y;

	//Calculate the yaw and pitch
	camera_yaw = (float) mouse_diff_x * 0.0174555555555556f;// * 3.142f / 180.0f;
	camera_pitch = mouse_diff_y * 0.0174555555555556f;// 3.142f / 180.0f );

	// Do a wraparound if the mouse cursor has gone out of the deadzone
	if ((mouse_current_x < m_window_deadzone) || (mouse_current_x > m_window_width-m_window_deadzone))
	{
		mouse_current_x = m_window_width >> 1;
		glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	}
	if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > m_window_height-m_window_deadzone))
	{
		mouse_current_y = m_window_height >> 1;
		glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	}

	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;

    return false;
}

float Application::GetMousePosX()
{
	glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);
	return mouse_current_x;
}

float Application::GetMousePosY()
{
	glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);
	return mouse_current_y;
}

bool Application::GetMouseDown(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	return glfwGetMouseButton(m_window, key) != 0;
}

int Application::GetWindowHeight()
{
	return m_window_height;
}

int Application::GetWindowWidth()
{
	return m_window_width;
}

bool Application::GetShouldUpdate()
{
	return m_bShouldUpdate;
}

void Application::SetShouldUpdate(bool update)
{
	m_bShouldUpdate = update;
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

	//glfwWindowHint(GLFW_DECORATED, false);

	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(m_window_width, m_window_height, "SUPERCOOL", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread.
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}

void Application::Run()
{
	SceneManager::SetSceneID(0); // temp to set to level1scene
	//Main Loop
	while (SceneManager::GetSceneID() != 10)
	{
		if (SceneManager::GetSceneID() == 0)
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			Scene *scene = new GameScene();
			scene->Init();

			m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
			while (SceneManager::GetSceneID() == 0)
			{
				GetMouseUpdate();
				scene->Update(m_timer.getElapsedTime());
				scene->Render();
				//Swap buffers
				glfwSwapBuffers(m_window);
				//Get and organize events, like keyboard and mouse input, window resizing, etc...
				glfwPollEvents();
				m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.

			} //Check if the ESC key had been pressed or if the window had been closed
			scene->Exit();
			delete scene;
		}
		else if (SceneManager::GetSceneID() == 1)
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			Scene *scene = new MenuScene();
			scene->Init();

			m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
			while (SceneManager::GetSceneID() == 1)
			{
				//GetMouseUpdate();
				scene->Update(m_timer.getElapsedTime());
				scene->Render();
				//Swap buffers
				glfwSwapBuffers(m_window);
				//Get and organize events, like keyboard and mouse input, window resizing, etc...
				glfwPollEvents();
				m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.

			} //Check if the ESC key had been pressed or if the window had been closed
			scene->Exit();
			delete scene;
		}
		else if (SceneManager::GetSceneID() == 2)
		{
			glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
			Scene *scene = new Level1Scene();
			scene->Init();

			m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
			while (SceneManager::GetSceneID() == 2)
			{
				GetMouseUpdate();
				scene->Update(m_timer.getElapsedTime());
				scene->Render();
				//Swap buffers
				glfwSwapBuffers(m_window);
				//Get and organize events, like keyboard and mouse input, window resizing, etc...
				glfwPollEvents();
				m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.

			} //Check if the ESC key had been pressed or if the window had been closed
			scene->Exit();
			delete scene;
		}
	}
}



void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}
