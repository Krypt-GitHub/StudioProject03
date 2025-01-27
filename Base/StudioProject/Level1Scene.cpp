﻿#include "Level1Scene.h"
#include "GL\glew.h"

#include "../Source/shader.hpp"
#include "../Source/MeshBuilder.h"
#include "../Source/Application.h"
#include "../Source/Utility.h"
#include "../Source/LoadTGA.h"
#include <sstream>
#include <time.h>
#include "../Physics/Collider.h"
#include "SceneManager.h"
#include "../Ray.h"


Level1Scene::Level1Scene()
{
	srand(time(NULL));
}

Level1Scene::~Level1Scene()
{
}

void Level1Scene::Init()
{
	SceneBase::Init();

	//Bullet initialisation
	for (int i = 0; i < 5; i++)
	{
		gl.m_goList.push_back(new GameObject(GameObject::GO_NONE));
	}

	// GAME OBJECT CREATION
	goFactory.CreateGO("Player", GameObject::GO_PLAYER, true, 70, Vector3(0, 19, 70), Vector3(1, 1, 1), 0, Vector3(6, 19, 2), 0, Vector3(0, 1, 0));
	goFactory.CreateGO("Pistol", GameObject::GO_PISTOL, true, 1.1, Vector3(0, 23, 60), Vector3(0.3, 0.3, 0.3), 90, Vector3(1.5 * 0.3, 6.5 * 0.3, 10 * 0.3), 90, Vector3(0, 1, 0));
	goFactory.CreateGO("Enemy", GameObject::GO_ENEMY, true, 70, Vector3(-15, 19, -60), Vector3(2, 2, 2), 0, Vector3(6, 19, 2), 0, Vector3(0, 1, 0));
	goFactory.CreateGO("Enemy", GameObject::GO_ENEMY, true, 70, Vector3(15, 19, -60), Vector3(2, 2, 2), 0, Vector3(6, 19, 2), 0, Vector3(0, 1, 0));
	goFactory.CreateGO("Floor", GameObject::GO_FLOOR, true, 0, Vector3(0, -2.5, 0), Vector3(100, 5, 300), 0, Vector3(50, 2.5, 150), 0, Vector3(0, 1, 0));
	goFactory.CreateGO("North Wall", GameObject::GO_WALL, true, 0, Vector3(0, 40, -165), Vector3(100, 80, 30), 0, Vector3(50, 40, 15), 0, Vector3(0, 1, 0));
	goFactory.CreateGO("South Wall", GameObject::GO_WALL, true, 0, Vector3(0, 40, 165), Vector3(100, 80, 30), 180, Vector3(50, 40, 15), 180, Vector3(0, 1, 0));
	goFactory.CreateGO("West Wall", GameObject::GO_WALL, true, 0, Vector3(65, 40, 0), Vector3(300, 80, 30), 90, Vector3(150, 40, 15), 90, Vector3(0, 1, 0));
	goFactory.CreateGO("East Wall", GameObject::GO_WALL, true, 0, Vector3(-65, 40, 0), Vector3(300, 80, 30), -90, Vector3(150, 40, 15), -90, Vector3(0, 1, 0));

	Player = static_cast<PlayerGO*>(gl.FetchGO(GameObject::GO_PLAYER));

	camera[1] = Player->camera;

	// INIT GAME OBJECT
	for (std::vector<GameObject *>::iterator it = gl.m_goList.begin(); it != gl.m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->GetActive())
		{
			InitGO(go);
		}
	}

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(13, 160, 90);
	lights[0].color.Set(0.217f, 0.094f, 0.028f);
	lights[0].power = 20.f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	camera[0].Init(Vector3(-420, 184, 178), Vector3(0, 185, 0), Vector3(0, 1, 0));

	Player = static_cast<PlayerGO*>(gl.FetchGO(GameObject::GO_PLAYER));
	Player->camera.target.Set(0, 0, 0);
	Player->camera.up.Set(0, 1, 0);
	camera[1] = Player->camera;

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.01f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);

	bLightEnabled = true;

	// Shadow
	m_lightDepthFBO.Init(4096, 4096);

	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f);
	meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();

	m_parameters[U_ELAPSEDTIME] = glGetUniformLocation(m_programID, "myTime");
	m_parameters[U_MTENABLED] = glGetUniformLocation(m_programID, "enableMT");
	//meshList[GEO_WATER] = MeshBuilder::GenerateQuad("water", Color(1, 1, 1), 1.f);
	//meshList[GEO_WATER]->textureArray[0] = LoadTGA("Image//sky.tga");
	//meshList[GEO_WATER]->textureArray[1] = LoadTGA("Image//water.tga");

	m_parameters[U_ENABLEREFLECT] = glGetUniformLocation(m_programID, "enableReflect");
	m_parameters[U_CAMERAPOS] = glGetUniformLocation(m_programID, "cameraPos");
}

void Level1Scene::Update(double dt)
{
	SceneBase::Update(dt);

	if (cameraID == 0)
	{
		camera[0].Update(dt);
	}
	else if (cameraID == 1)
	{
		camera[1] = Player->camera;
		Player->Update(dt);

	}

	//RayCast
	Ray* ray = new Ray((camera[1].target - camera[1].position).Normalized(), camera[1].position);
	for (auto go : gl.m_goList)
	{
		if (go->type != GameObject::GO_PISTOL)
			continue;

		if (
			(ray->IntersectionOBB(go->obb) && Application::GetKeyDown('E') && !static_cast<PistolGO*>(go)->GetPickUp() && Player->gun == nullptr)
			||
			(Player->obb.GetCollision(go->obb) && !static_cast<PistolGO*>(go)->GetPickUp() && Player->gun == nullptr)
			)
		{

			static_cast<PistolGO*>(go)->SetPickUp(true);
			Player->gun = static_cast<PistolGO*>(go);

			Player->gun->transform.position = Player->transform.position + Vector3(-8, -14, 40);

		}



	}

	// UPDATE GAME OBJECT
	for (std::vector<GameObject *>::iterator it = gl.m_goList.begin(); it != gl.m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->GetActive())
		{
			UpdateGO(go, dt);
		}
	}


	fps = (float)(1.f / dt);

	//for (std::vector<GameObject *>::iterator it = gl.m_goList.begin(); it != gl.m_goList.end(); ++it)
	//{
	//	GameObject *go = (GameObject *)*it;
	//	if (go->GetActive())
	//	{
	//		if (go->type == GameObject::GO_BULLET)
	//		{
	//			go->transform.position += go->m_v3dir * 10 * dt;
	//			go->obb.UpdateAxis(Vector3(1, 0, 0), Vector3(0, 0, 1));
	//			go->obb.UpdatePos(go->transform.position);
	//			go->obb.SetScale(go->transform.scale * (1 / 2));
	//			go->obb.RotateAxis(0, Vector3(0, 1, 0));
	//		}

	//		if (go->type == GameObject::GO_PLAYER)
	//		{
	//			go->transform.position = Player->camera.position;
	//		}
	//	}
	//}

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

	if (Application::GetShouldUpdate)
		m_fchRotate -= 10 * dt;

	if (Application::GetKeyDown('0'))
		SceneManager::SetSceneID(1);
}

void Level1Scene::RenderWorld()
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

void Level1Scene::RenderWater()
{
	modelStack.PushMatrix();
	modelStack.Translate(-300, 70, -630);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-25, 0, 0, 1);
	modelStack.Scale(1200.f, 1200.f, 0.f);
	RenderMesh(meshList[GEO_WATER], false, true, true);
	modelStack.PopMatrix();
}

void Level1Scene::RenderPassGPass()
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

	m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);

	RenderWorld();
}

void Level1Scene::RenderPassMain()
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
	perspective.SetToPerspective(m_fFOV, 4.0f / 3.0f, 0.01f, 10000.0f);
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

	RenderWorld();

	modelStack.PushMatrix();
	RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 4, 5, 0, 0, m_fchRotate, Vector3(0, 0, 1));
	modelStack.PopMatrix();
	//modelStack.PushMatrix();
	//modelStack.Translate(100, 200, 0);
	//modelStack.Scale(75, 75, 50);
	//RenderMesh(meshList[GEO_LIGHT_DEPTH_QUAD], false, false, false); // Red color quad for the shadow map
	//modelStack.PopMatrix();
}

void Level1Scene::InitGO(GameObject * go)
{
	switch (go->type)
	{
	case GameObject::GO_PLAYER:
		static_cast<PlayerGO*>(go)->Init();
		break;
	case GameObject::GO_ENEMY:
		static_cast<EnemyGO*>(go)->Init();
		break;
	case GameObject::GO_PISTOL:
		static_cast<PistolGO*>(go)->Init();
		break;
	}
}

void Level1Scene::UpdateGO(GameObject * go, double dt)
{
	switch (go->type)
	{
	case GameObject::GO_PLAYER:
		if (cameraID == 1)
			static_cast<PlayerGO*>(go)->Update(dt);
		break;
	case GameObject::GO_ENEMY:
		static_cast<EnemyGO*>(go)->Update(dt);
		break;
	case GameObject::GO_PISTOL:
		static_cast<PistolGO*>(go)->Update(dt);
		break;
	}
}

void Level1Scene::RenderGO(GameObject* go)
{
	switch (go->type)
	{
	case GameObject::GO_PISTOL:
		if (static_cast<PistolGO*>(go)->GetPickUp())
		{
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

			Vector3 check = Vector3(Player->camera.target.z - Player->camera.position.z);
			if (Application::GetKeyDown('Z'))
				int i = 0;
			modelStack.Rotate(Math::RadianToDegree(atan2(Player->camera.target.x - Player->camera.position.x, Player->camera.target.z - Player->camera.position.z)), 0, 1, 0);


			//if(check.x >0 )
			modelStack.Rotate(Math::RadianToDegree(-atan2(Player->camera.target.y - Player->camera.position.y, Vector3(Player->camera.target.x - Player->camera.position.x, 0, Player->camera.target.z - Player->camera.position.z).Length())), 1, 0, 0);

			//else
			//modelStack.Rotate(Math::RadianToDegree(-atan2(Player->camera.target.y - Player->camera.position.y, Vector3(-Player->camera.target.z + Player->camera.position.z, 0, Player->camera.target.x - Player->camera.position.x).Length())), 1, 0, 0);

			modelStack.Translate(-4, -5, 20);
			Player->gun->transform.position = Vector3(modelStack.Top().a[12], modelStack.Top().a[13], modelStack.Top().a[14]);
			modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
			RenderMesh(meshList[GEO_PISTOL], false, false, false);
			//modelStack.PopMatrix();
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			modelStack.Translate(go->obb.pos.x, go->obb.pos.y, go->obb.pos.z);
			modelStack.Rotate(Math::RadianToDegree(acosf((go->obb.AxisZ.Dot(Vector3(0, 0, 1)) / go->obb.AxisZ.Length() * Vector3(0, 0, 1).Length()))), 0, 1, 0);
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
			//modelStack.PushMatrix();
			//modelStack.Translate(go->obb.pos.x, go->obb.pos.y, go->obb.pos.z);
			//modelStack.Rotate(Math::RadianToDegree(acosf((go->obb.AxisZ.Dot(Vector3(0, 0, 1)) / go->obb.AxisZ.Length() * Vector3(0, 0, 1).Length()))), 0, 1, 0);
			//modelStack.Scale(go->obb.Half_size.x*2, go->obb.Half_size.y*2, go->obb.Half_size.z*2);
			//RenderMesh(meshList[GEO_CUBE], false, false, false);
			//modelStack.PopMatrix();
		}
		break;
	case GameObject::GO_BULLET:
		modelStack.PushMatrix();
		modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
		modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
		RenderMesh(meshList[GEO_LIGHTBALL], false, false, false);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(go->obb.pos.x, go->obb.pos.y, go->obb.pos.z);
		modelStack.Scale(go->obb.Half_size.x * 2, go->obb.Half_size.y * 2, go->obb.Half_size.z * 2);
		RenderMesh(meshList[GEO_CUBE], false, false, false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PLAYER:
		modelStack.PushMatrix();
		modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(Player->camera.target.x - Player->camera.position.x, Player->camera.target.z - Player->camera.position.z)), 0, 1, 0);
		modelStack.Scale(10, 10, 10);
		//RenderMesh(meshList[GEO_HANDS], false, false, false);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(go->obb.pos.x, go->obb.pos.y, go->obb.pos.z);
		modelStack.Scale(go->obb.Half_size.x * 2, go->obb.Half_size.y * 2, go->obb.Half_size.z * 2);
		RenderMesh(meshList[GEO_CUBE], false, false, false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_ENEMY:
		if (static_cast<EnemyGO*>(go)->aiStatus->state == AIBehaviour::IDLE)
		{
			modelStack.PushMatrix();
			modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
			//modelStack.Rotate(Math::RadianToDegree(atan2(Player.camera.target.x - Player.camera.position.x, Player.camera.target.z - Player.camera.position.z)), 0, 1, 0);
			modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
			RenderMesh(meshList[GEO_ENEMY_STAND], false, false, false);
			modelStack.PopMatrix();
			modelStack.PushMatrix();
			modelStack.Translate(go->obb.pos.x, go->obb.pos.y, go->obb.pos.z);
			modelStack.Scale(go->obb.Half_size.x * 2, go->obb.Half_size.y * 2, go->obb.Half_size.z * 2);
			RenderMesh(meshList[GEO_CUBE], false, false, false);
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
		modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
		RenderMesh(meshList[GEO_FLOOR], false, false, false);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(go->obb.pos.x, go->obb.pos.y, go->obb.pos.z);
		modelStack.Scale(go->obb.Half_size.x * 2, go->obb.Half_size.y * 2, go->obb.Half_size.z * 2);
		RenderMesh(meshList[GEO_CUBE], false, false, false);
		modelStack.PopMatrix();
		//modelStack.PushMatrix();
		//modelStack.Translate(go->obb.pos.x, go->obb.pos.y, go->obb.pos.z);
		//modelStack.Rotate(90, 1, 0, 0);
		//modelStack.Scale(go->obb.Half_size.x*2, go->obb.Half_size.y*2, go->obb.Half_size.z*2);
		//RenderMesh(meshList[GEO_CUBE], false, false, false);
		//modelStack.PopMatrix();
		break;
	case GameObject::GO_WALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
		modelStack.Rotate(go->transform.rotation, 0, 1, 0);
		modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
		RenderMesh(meshList[GEO_WALL], false, false, false);
		modelStack.PopMatrix();
		break;
	}
}

void Level1Scene::Render()
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

void Level1Scene::Exit()
{
	SceneBase::Exit();
	gl.Exit();
}
