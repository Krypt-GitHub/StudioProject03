#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

class SceneManager
{
private:
	static int sceneID;
public:
	SceneManager();
	~SceneManager();

	static int GetSceneID();
	static void SetSceneID(int _sceneID);
};

#endif
