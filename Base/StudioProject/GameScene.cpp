#include "GameScene.h"
#include "GL\glew.h"

#include "../Source/shader.hpp"
#include "../Source/MeshBuilder.h"
#include "../Source/Application.h"
#include "../Source/Utility.h"
#include "../Source/LoadTGA.h"
#include <sstream>
#include <time.h>
#include "../Physics/Collider.h"

GameScene::GameScene()
{
	srand(time(NULL));
}

GameScene::~GameScene()
{
}

void GameScene::Init()
{
	SceneBase::Init();

	// GAME OBJECT CREATION
	goFactory.CreateGO("Player", GameObject::GO_PLAYER, true, 70, Vector3(10, 0, 10), Vector3(1, 1, 1), 0);
	goFactory.CreateGO("Enemy", GameObject::GO_ENEMY, true, 70, Vector3(-10, 0, -10), Vector3(2, 2, 2), 0);
	
	
	
	
	// Player Init
	//Player.Init();
	//Enemy.Init();





	// INIT GAME OBJECT
	for (std::vector<GameObject *>::iterator it = gl.m_goList.begin(); it != gl.m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->GetActive())
		{
			InitGO(go);
		}
	}

	Player = static_cast<PlayerGO*>(gl.FetchGO(GameObject::GO_PLAYER));
	Player->camera.target.Set(0, 0, 0);
	Player->camera.up.Set(0, 1, 0);
	camera[1] = Player->camera;

	// Weapon Init
	Pistol.Init("Pistol", GameObject::GO_PISTOL, 10, 15.f, 12);

	//floor = gl.CreateGO("floor", GameObject::GO_FLOOR, true, 0, Vector3(0, 0, 0), Vector3(1000, 1000, 5), 0);
	//floor->obb.upDateAxis(Vector3(1, 0, 0), Vector3(0, 0, 1));
	//floor->obb.upDatePos(Vector3(0, 0, 0));
	//floor->obb.setScale(Vector3(500, 500, 2.5));
	//floor->obb.rotateAxis(90, Vector3(1, 0, 0));

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	bLightEnabled = true;

	// Shadow
	m_lightDepthFBO.Init(4096, 4096);

	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f);
	meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();

	m_parameters[U_ELAPSEDTIME] = glGetUniformLocation(m_programID, "myTime");
	m_parameters[U_MTENABLED] = glGetUniformLocation(m_programID, "enableMT");
	meshList[GEO_WATER] = MeshBuilder::GenerateQuad("water", Color(1, 1, 1), 1.f);
	meshList[GEO_WATER]->textureArray[0] = LoadTGA("Image//sky.tga");
	meshList[GEO_WATER]->textureArray[1] = LoadTGA("Image//water.tga");

	m_parameters[U_ENABLEREFLECT] = glGetUniformLocation(m_programID, "enableReflect");
	m_parameters[U_CAMERAPOS] = glGetUniformLocation(m_programID, "cameraPos");
}

void GameScene::Update(double dt)
{
	SceneBase::Update(dt);

	// UPDATE GAME OBJECT
	for (std::vector<GameObject *>::iterator it = gl.m_goList.begin(); it != gl.m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->GetActive())
		{
			UpdateGO(go, dt);
		}
	}
	//Pistol.Update(dt, (Player.camera.target - Player.camera.position).Normalize());

	if (cameraID == 0)
	{
		camera[0].Update(dt);
	}
	else if (cameraID == 1)
	{
		camera[1] = Player->camera;
		Player->Update(dt);
	}

	//if (Application::GetKeyDown('E'))
	//{
	//	Pistol.SetPickUp(true);
	//	Player.currentObject = Pistol.weaponObject;

	//	//Changing it to a static object
	//	Pistol.weaponObject->SetStatic(true);
	//	Pistol.weaponObject->obb.upDatePos(Pistol.weaponObject->transform.position);
	//}

	fps = (float)(1.f / dt);

	for (std::vector<GameObject *>::iterator it = gl.m_goList.begin(); it != gl.m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->GetActive())
		{
			if (go->type == GameObject::GO_BULLET)
			{
				go->transform.position += go->m_v3dir * 10 * dt;
			}

			if (go->type == GameObject::GO_PLAYER)
			{
				go->transform.position = Player->camera.position;
			}
		}
	}
	static bool m_bRBDown = false;
	if (Application::GetMouseDown(2) && !m_bRBDown)
	{
		m_fFOV = 10.f;
		m_bRBDown = true;
	}
	if (!Application::GetMouseDown(2) && m_bRBDown)
	{
		m_fFOV = 45.f;
		m_bRBDown = false;
	}
}

void GameScene::RenderWorld()
{
	for (std::vector<GameObject *>::iterator it = gl.m_goList.begin(); it != gl.m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->GetActive())
		{
			RenderGO(go);
		}
	}
}

void GameScene::RenderWater()
{
	modelStack.PushMatrix();
	modelStack.Translate(-300, 70, -630);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-25, 0, 0, 1);
	modelStack.Scale(1200.f, 1200.f, 0.f);
	RenderMesh(meshList[GEO_WATER], false, true, true);
	modelStack.PopMatrix();
}

void GameScene::RenderPassGPass()
{
	m_renderPass = RENDER_PASS_PRE;
	m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_gPassShaderID);
	//These matrices should change when light position or direction changes

	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
		m_lightDepthProj.SetToOrtho(-100, 100, -100, 100, 0.1, 1000);
	else
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 200);

	if (lights[1].type == Light::LIGHT_DIRECTIONAL)
		m_lightDepthProj.SetToOrtho(-500, 500, -100, 300, 0, 1400);
	else
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 1000);

	m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);
	m_lightDepthView.SetToLookAt(lights[1].position.x, lights[1].position.y, lights[1].position.z, 0, 0, 0, 0, 1, 0);

	RenderWorld();
}

void GameScene::RenderPassMain()
{
	m_renderPass = RENDER_PASS_MAIN;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(), Application::GetWindowHeight());
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_programID);
	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

	//Mtx44 ortho;
	//ortho.SetToOrtho(-100, 100, -100, 100, 0.1, 1000);
	//projectionStack.LoadMatrix(ortho);

	Mtx44 perspective;
	perspective.SetToPerspective(m_fFOV, 4.0f / 3.0f, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(perspective);
	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(camera[cameraID].position.x, camera[cameraID].position.y, camera[cameraID].position.z,
		camera[cameraID].target.x, camera[cameraID].target.y, camera[cameraID].target.z,
		camera[cameraID].up.x, camera[cameraID].up.y, camera[cameraID].up.z);

	modelStack.LoadIdentity();
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if (lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	RenderMesh(meshList[GEO_AXES], false, false, false);

	//RenderWater();


	RenderWorld();


	//modelStack.PushMatrix();
	//modelStack.Translate(100, 200, 0);
	//modelStack.Scale(75, 75, 50);
	//RenderMesh(meshList[GEO_LIGHT_DEPTH_QUAD], false, false, false); // Red color quad for the shadow map
	//modelStack.PopMatrix();
}

void GameScene::InitGO(GameObject * go)
{
	switch (go->type)
	{
	case GameObject::GO_PLAYER:
		static_cast<PlayerGO*>(go)->Init();
	case GameObject::GO_ENEMY:
		static_cast<EnemyGO*>(go)->Init();
		break;
	case GameObject::GO_PISTOL:
		break;
	}
}

void GameScene::UpdateGO(GameObject * go, double dt)
{
	switch (go->type)
	{
	//case GameObject::GO_PLAYER:
	//	static_cast<PlayerGO*>(go)->Update(dt);
	case GameObject::GO_ENEMY:
		static_cast<EnemyGO*>(go)->Update(dt);
		break;
	case GameObject::GO_PISTOL:
		break;
	}
}

void GameScene::RenderGO(GameObject* go)
{
	switch (go->type)
	{
	case GameObject::GO_PISTOL:
		if (Pistol.GetPickUp())
		{
			Pistol.weaponObject->transform.position = Player->transform.position;
			//if (isRBDown)
			//{
			//	modelStack.PushMatrix();
			//	modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
			//	modelStack.Rotate(Math::RadianToDegree(atan2(player.camera.target.x - player.camera.position.x, player.camera.target.z - player.camera.position.z)), 0, 1, 0);
			//	modelStack.Translate(0, -2, 4);
			//	//modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
			//	RenderMesh(meshList[GEO_PISTOL], false, false, false);
			//	modelStack.PopMatrix();
			//}
			//else
			//{
			modelStack.PushMatrix();
			modelStack.Translate(Player->transform.position.x, Player->transform.position.y, Player->transform.position.z);
			//modelStack.PushMatrix();
			modelStack.Rotate(Math::RadianToDegree(atan2(Player->camera.target.x - Player->camera.position.x, Player->camera.target.z - Player->camera.position.z)), 0, 1, 0);
			//modelStack.Rotate(Math::RadianToDegree(-atan2(Player.camera.target.y - Player.camera.position.y, Player.camera.target.x - Player.camera.position.x)), 1, 0, 0);
			modelStack.Translate(-8, -10, 30);
			modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
			RenderMesh(meshList[GEO_PISTOL], false, false, false);
			//modelStack.PopMatrix();
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			modelStack.Translate(go->obb.pos.x, go->obb.pos.y, go->obb.pos.z);
			modelStack.Scale(go->obb.Half_size.x, go->obb.Half_size.y, go->obb.Half_size.z);
			RenderMesh(meshList[GEO_CUBE], false, false, false);
			modelStack.PopMatrix();
			//}
		}
		else
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
			modelStack.Rotate(go->transform.rotation, 0, 1, 0);
			modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
			RenderMesh(meshList[GEO_PISTOL], false, false, false);
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			modelStack.Translate(go->obb.pos.x, go->obb.pos.y, go->obb.pos.z);
			modelStack.Scale(go->obb.Half_size.x*2, go->obb.Half_size.y*2, go->obb.Half_size.z*2);
			RenderMesh(meshList[GEO_CUBE], false, false, false);
			modelStack.PopMatrix();
		}
		break;
	case GameObject::GO_BULLET:
		modelStack.PushMatrix();
		modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
		modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
		RenderMesh(meshList[GEO_LIGHTBALL], false, false, false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PLAYER:
		modelStack.PushMatrix();
		modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(Player->camera.target.x - Player->camera.position.x, Player->camera.target.z - Player->camera.position.z)), 0, 1, 0);
		modelStack.Scale(10, 10, 10);
		//RenderMesh(meshList[GEO_HANDS], false, false, false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ENEMY:
		if (static_cast<EnemyGO* >(go)->aiStatus->state == AIBehaviour::IDLE)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
			//modelStack.Rotate(Math::RadianToDegree(atan2(Player.camera.target.x - Player.camera.position.x, Player.camera.target.z - Player.camera.position.z)), 0, 1, 0);
			modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
			RenderMesh(meshList[GEO_ENEMY_STAND], false, false, false);
			modelStack.PopMatrix();
		}
		else if (static_cast<EnemyGO*>(go)->aiStatus->state == AIBehaviour::WALK)
		{
			if (static_cast<EnemyGO*>(go)->aiStatus->m_bstartWalk01)
			{
				modelStack.PushMatrix();
				modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
				modelStack.Rotate(Math::RadianToDegree(atan2(Player->camera.target.x - Player->camera.position.x, Player->camera.target.z - Player->camera.position.z)), 0, 1, 0);
				modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
				RenderMesh(meshList[GEO_ENEMY_WALK01], false, false, false);
				modelStack.PopMatrix();
			}
			if (static_cast<EnemyGO*>(go)->aiStatus->m_bstartWalk02)
			{
				modelStack.PushMatrix();
				modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
				modelStack.Rotate(Math::RadianToDegree(atan2(Player->camera.target.x - Player->camera.position.x, Player->camera.target.z - Player->camera.position.z)), 0, 1, 0);
				modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
				RenderMesh(meshList[GEO_ENEMY_WALK02], false, false, false);
				modelStack.PopMatrix();
			}
		}
		break;
	case GameObject::GO_FLOOR:
		modelStack.PushMatrix();
		modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
		RenderMesh(meshList[GEO_FLOOR], false, false, false);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(go->obb.pos.x, go->obb.pos.y, go->obb.pos.z);
		modelStack.Rotate(90, 1, 0, 0);
		modelStack.Scale(go->obb.Half_size.x*2, go->obb.Half_size.y*2, go->obb.Half_size.z*2);
		RenderMesh(meshList[GEO_CUBE], false, false, false);
		modelStack.PopMatrix();
		break;
	}
}

void GameScene::Render()
{
	RenderPassGPass();

	RenderPassMain();

	//
	//// Render the crosshair
	//RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 10.0f);

	//On screen text
	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);
}

void GameScene::Exit()
{
	SceneBase::Exit();
	gl.Exit();
}