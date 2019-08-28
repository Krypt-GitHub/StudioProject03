#ifndef LEVEL_2_SCENE_H
#define LEVEL_2_SCENE_H

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
#include "Physics.h"

#include "EnemyGO.h"
#include "PlayerGO.h"
#include "PistolGO.h"

#include "PathNode.h"

class Level2Scene : public SceneBase
{
public:
	Level2Scene();
	~Level2Scene();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void InitGO(GameObject* go);
	void UpdateGO(GameObject* go, double dt);
	void RenderPN(PathNode * pn);
	void RenderGO(GameObject* go);

	void RenderWorld();
	void RenderWater();
	void RenderPassGPass();
	void RenderPassMain();
	//void RenderTerrain();

	GOList &gl = GOList::GetInstance();
private:
	GOFactory goFactory;
	PlayerGO *Player;
	Physics PhysicsEngine;
	std::vector <PathNode *> m_nodeList;
	bool endgame;
};

#endif