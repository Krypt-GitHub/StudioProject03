#include "GameScene.h"
#include "GL\glew.h"

#include "../Source/shader.hpp"
#include "../Source/MeshBuilder.h"
#include "../Source/Application.h"
#include "../Source/Utility.h"
#include "../Source/LoadTGA.h"
#include <sstream>
#include <time.h>

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

	// Player Init
	Player.Init();
	camera[1] = Player.camera;

	// Weapon Init
	Pistol.Init("Pistol", GameObject::GO_PISTOL, 10, 15.f, 12);

	// Init FOG
	//Color fogColor(0.24f, 0.19f, 0.06f);

	//glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
	//glUniform1f(m_parameters[U_FOG_START], 10);
	//glUniform1f(m_parameters[U_FOG_END], 1000);
	//glUniform1f(m_parameters[U_FOG_DENSITY], 0.005f);
	//glUniform1i(m_parameters[U_FOG_TYPE], 0);
	//glUniform1i(m_parameters[U_FOG_ENABLED], 1);

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(0, 0, 1), 1.f);

	// UI
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureArray[0] = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);

	// Environment
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_SKYDOME] = MeshBuilder::GenerateSkyPlane("skydome", Color(0.f, 0.f, 1.f), 100, 600, 2000, 2, 2);
	meshList[GEO_SKYDOME]->textureArray[0] = LoadTGA("Image//sky.tga");

	meshList[GEO_FLOOR] = MeshBuilder::GenerateQuad("floor", Color(1, 1, 1), 1.f);
	meshList[GEO_FLOOR]->textureArray[0] = LoadTGA("Image//grass.tga");

	meshList[GEO_PISTOL] = MeshBuilder::GenerateOBJ("pistol", "OBJ//pistol.obj");
	meshList[GEO_PISTOL]->textureArray[0] = LoadTGA("Image//pistol.tga");

	meshList[GEO_HANDS] = MeshBuilder::GenerateOBJ("hands", "OBJ//hands.obj");

	//meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerrain("GEO_TERRAIN", "Image//heightmap.raw", m_heightMap);



	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
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

	Player.Update(dt);
	Pistol.Update(dt, (Player.camera.target - Player.camera.position).Normalize());

	if (cameraID == 0)
	{
		camera[0].Update(dt);
	}
	else if (cameraID == 1)
	{
		camera[1] = Player.camera;
		Player.Update(dt);
	}

	if (Application::GetKeyDown('E'))
	{
		Pistol.SetPickUp(true);
	}

	fps = (float)(1.f / dt);

	std::cout << Pistol.weaponObject->transform.position << std::endl;

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
				go->transform.position = Player.camera.position;
			}
		}
	}
	static bool m_bRBDown = false;
	if (Application::GetMouseDown(1) && !m_bRBDown)
	{
		m_fFOV = 10.f;
		m_bRBDown = true;
	}
	if (!Application::GetMouseDown(1) && m_bRBDown)
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

	modelStack.PushMatrix();
	modelStack.Translate(0, -10, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Scale(1000, 1000, 0);
	RenderMesh(meshList[GEO_FLOOR], false, false, false);
	modelStack.PopMatrix();

	//RenderWater();

	//modelStack.PushMatrix();
	//modelStack.Translate(0, 800, 0);
	//RenderMesh(meshList[GEO_SKYDOME], false, false, false);
	//modelStack.PopMatrix();

	

	RenderWorld();

	//modelStack.PushMatrix();
	//modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	//RenderMesh(meshList[GEO_LIGHTBALL], false, false, false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(lights[1].position.x, lights[1].position.y, lights[1].position.z);
	//RenderMesh(meshList[GEO_LIGHTBALL], false, false, false);
	//modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(100, 200, 0);
	//modelStack.Scale(75, 75, 50);
	//RenderMesh(meshList[GEO_LIGHT_DEPTH_QUAD], false, false, false); // Red color quad for the shadow map       
	//modelStack.PopMatrix();
}

//void GameScene::RenderTerrain()
//{
//	modelStack.PushMatrix();
//	modelStack.Scale(4000.0f, 350.f, 4000.0f); // values varies. 
//	RenderMesh(meshList[GEO_TERRAIN], true, false, false);
//	modelStack.PopMatrix();
//}

void GameScene::RenderGO(GameObject* go)
{
	switch (go->type)
	{
	case GameObject::GO_PISTOL:
		if (Pistol.GetPickUp())
		{
			Pistol.weaponObject->transform.position = Player.player->transform.position;
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
			modelStack.Translate(Player.player->transform.position.x, Player.player->transform.position.y, Player.player->transform.position.z);
			modelStack.Rotate(Math::RadianToDegree(atan2(Player.camera.target.x - Player.camera.position.x, Player.camera.target.z - Player.camera.position.z)), 0, 1, 0);
			modelStack.Translate(-1, -1.5, 3);
			modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
			RenderMesh(meshList[GEO_PISTOL], false, false, false);
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
		modelStack.Rotate(Math::RadianToDegree(atan2(Player.camera.target.x - Player.camera.position.x, Player.camera.target.z - Player.camera.position.z)), 0, 1, 0);
		modelStack.Scale(10, 10, 10);
		RenderMesh(meshList[GEO_HANDS], false, false, false);
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
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteProgram(m_gPassShaderID);
	glDeleteVertexArrays(1, &m_vertexArrayID);
}
