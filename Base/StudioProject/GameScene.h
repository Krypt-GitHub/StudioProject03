#ifndef OUR_SCENE_H
#define OUR_SCENE_H

//#include "../Core/Scene.h"
#include "SceneBase.h"

#include "Mtx44.h"
#include "../Source/Mesh.h"
#include "MatrixStack.h"
#include "../Source/Particle.h"
#include <vector>
#include "../Source/LoadHmap.h"
#include "../Source/DepthFBO.h"

#include "GOList.h"
#include "GOFactory.h"

#include "EnemyGO.h"
#include "PlayerGO.h"
//#include "PistolGO.h"

class GameScene : public SceneBase
{
public:
	GameScene();
	~GameScene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void InitGO(GameObject* go);
	void UpdateGO(GameObject* go, double dt);
	void RenderGO(GameObject* go);

	void RenderWorld();
	void RenderWater();
	void RenderPassGPass();
	void RenderPassMain();
	//void RenderTerrain();

private:
	GOList &gl = GOList::GetInstance();

	GOFactory goFactory;
	PlayerGO *Player;
};

#endif