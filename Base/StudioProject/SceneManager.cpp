#include "SceneManager.h"

int SceneManager::sceneID = 0;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

int SceneManager::GetSceneID()
{
	return sceneID;
}

void SceneManager::SetSceneID(int _sceneID)
{
	sceneID = _sceneID;
}
