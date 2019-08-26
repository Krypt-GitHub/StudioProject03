#include "MenuScene.h"
#include "GL\glew.h"
#include <GLFW/glfw3.h>

#include "../Source/shader.hpp"
#include "../Source/MeshBuilder.h"
#include "../Source/Application.h"
#include "../Source/Utility.h"
#include "../Source/LoadTGA.h"
#include <sstream>
#include <time.h>
#include "../Physics/Collider.h"
#include "SceneManager.h"

MenuScene::MenuScene()
{
	srand(time(NULL));
}

MenuScene::~MenuScene()
{
}

void MenuScene::Init()
{
	SceneBase::Init();

	// GAME OBJECT CREATION
	goFactory.CreateGO("Logo", GameObject::GO_LOGO, true, 0, Vector3(328, 400, -350), Vector3(800, 230, 1), 20, Vector3(1, 0, 0), 0, Vector3(0, 1, 0));
	goFactory.CreateGO("Play Button", GameObject::GO_PLAY, true, 0, Vector3(328, 200, -250), Vector3(7, 7, 7), 0, Vector3(20, 40, 10), 0, Vector3(0, 1, 0));
	goFactory.CreateGO("Floor01", GameObject::GO_FLOOR, true, 0, Vector3(-100, 0, -500), Vector3(600, 250, 1000), 0, Vector3(300, 125, 500), 0, Vector3(0, 1, 0));
	goFactory.CreateGO("Floor02", GameObject::GO_FLOOR, true, 0, Vector3(760, 0, -500), Vector3(600, 250, 1000), 0, Vector3(300, 125, 500), 0, Vector3(0, 1, 0));

	PlayButton = gl.FetchGO(GameObject::GO_PLAY);

	for (int i = 0; i < 5; i++)
	{
		gl.m_goList.push_back(new GameObject(GameObject::GO_NONE));
	}

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
	lights[0].position.Set(328, 0, 200);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 500.f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0, 0, 1);

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

	meshList[GEO_SKYDOME] = MeshBuilder::GenerateSkyPlane("skydome", Color(0.f, 0.f, 1.f), 100, 600, 2000, 2, 2);
	meshList[GEO_SKYDOME]->textureArray[0] = LoadTGA("Image//sky.tga");

	m_parameters[U_ELAPSEDTIME] = glGetUniformLocation(m_programID, "myTime");
	m_parameters[U_MTENABLED] = glGetUniformLocation(m_programID, "enableMT");
	meshList[GEO_WATER] = MeshBuilder::GenerateQuad("water", Color(1, 1, 1), 1.f);
	meshList[GEO_WATER]->textureArray[0] = LoadTGA("Image//sky.tga");
	meshList[GEO_WATER]->textureArray[1] = LoadTGA("Image//water.tga");

	m_parameters[U_ENABLEREFLECT] = glGetUniformLocation(m_programID, "enableReflect");
	m_parameters[U_CAMERAPOS] = glGetUniformLocation(m_programID, "cameraPos");

	camera[0].Init(Vector3(-420, 184, 178), Vector3(0, 185, 0), Vector3(0, 1, 0));
	camera[1].Init(Vector3(331, 249, 600), Vector3(331, 249, 0), Vector3(0, 1, 0));

	cursorRay = new MouseRayCast(camera[1], projectionStack.Top());
}

void MenuScene::Update(double dt)
{
	SceneBase::Update(dt);

	if (cameraID == 0)
	{
		camera[0].Update(dt);
		Application::GetMouseUpdate();
	}

	cursorRay->Update(dt);

	fps = (float)(1.f / dt);

	if (Application::GetKeyDown('0'))
		SceneManager::SetSceneID(10);
	
	//m_v3mouseRayCast = Vector3
	
	if (Application::GetMouseDown(0))
	{
		PlayButton->m_bGravity = true;
		//Vector3 tempPos = PlayButton->transform.position;
		//Vector3 tempMouse = Vector3(Application::GetMousePosX(), Application::GetMousePosY(), 0);
		//Vector3 newMousePos ()

		PlayButton->SetStatic(false);
		PlayButton->transform.position.x = Application::GetMousePosX();
		PlayButton->transform.position.y = Application::GetMousePosY();
	}

	for (std::vector<GameObject *>::iterator it = gl.m_goList.begin(); it != gl.m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->GetActive())
		{
			go->obb.UpdatePos(go->transform.position);

			if (go->type == GameObject::GO_PLAY)
			{
				if (go->m_bGravity && !go->GetStatic())
					go->transform.position += Vector3(0, -47.5, 0) * 10 * dt;
				if (go->transform.position.y <= 60)
				{
					go->m_bGravity = false;
					go->transform.position.z -= 100 * dt;
				}
			}
		}

		for (std::vector<GameObject *>::iterator it2 = it + 1; it2 != gl.m_goList.end(); ++it2)
		{
			GameObject *go2 = (GameObject *)*it2;
			if (go->obb.GetCollision(go2->obb))
			{
				if (go->type == GameObject::GO_PLAY && go2->type == GameObject::GO_FLOOR)
					go->m_bGravity = false;
			}
		}
	}
}

void MenuScene::RenderWorld()
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

void MenuScene::RenderPassGPass()
{
	m_renderPass = RENDER_PASS_PRE;
	m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(m_gPassShaderID);
	//These matrices should change when light position or direction changes

	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
		m_lightDepthProj.SetToOrtho(-1000, 1000, -1000, 1000, 0.1, 1000);
	else
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 200);

	m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);

	RenderWorld();
}

void MenuScene::RenderPassMain()
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

	RenderMesh(meshList[GEO_AXES], false, false, false);

	RenderWorld();

	modelStack.PushMatrix();
	modelStack.Translate(330, 800, 0);
	RenderMesh(meshList[GEO_SKYDOME], false, false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(500, -200, 0);
	//modelStack.Rotate(45, 0, 1, 0);
	modelStack.Scale(400, 250, 1000);
	RenderMesh(meshList[GEO_CUBE], false, false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(330, 70, -100);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-135, 0, 0, 1);
	modelStack.Scale(1000.f, 1000.f, 0.f);
	RenderMesh(meshList[GEO_WATER], false, true, true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 200, 0);
	modelStack.Scale(75, 75, 50);
	RenderMesh(meshList[GEO_LIGHT_DEPTH_QUAD], false, false, false); // Red color quad for the shadow map
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_CUBE], false, false, false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(cursorRay->getCurrentRay().x, 0, cursorRay->getCurrentRay().z);
	RenderMesh(meshList[GEO_CUBE], false, false, false);
	modelStack.PopMatrix();

	std::ostringstream ss;
	ss.precision(5);
	ss << "CursorRayX: " << cursorRay->getCurrentRay().x;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 3, 0, 6);

	std::ostringstream ss1;
	ss1.precision(5);
	ss1 << "CursorRayZ: " << cursorRay->getCurrentRay().y;
	RenderTextOnScreen(meshList[GEO_TEXT], ss1.str(), Color(0, 1, 0), 3, 0, 3);
}

void MenuScene::InitGO(GameObject * go)
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

void MenuScene::RenderGO(GameObject* go)
{
	switch (go->type)
	{
	case GameObject::GO_PISTOL:
		modelStack.PushMatrix();
		modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
		modelStack.Rotate(go->transform.rotation, 0, 1, 0);
		modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
		RenderMesh(meshList[GEO_PISTOL], false, false, false);
		modelStack.PopMatrix();
		modelStack.PushMatrix();
		modelStack.Translate(go->obb.pos.x, go->obb.pos.y, go->obb.pos.z);
		modelStack.Rotate(Math::RadianToDegree(acosf((go->obb.AxisZ.Dot(Vector3(0, 0, 1)) / go->obb.AxisZ.Length() * Vector3(0, 0, 1).Length()))), 0, 1, 0);
		modelStack.Scale(go->obb.Half_size.x * 2, go->obb.Half_size.y * 2, go->obb.Half_size.z * 2);
		RenderMesh(meshList[GEO_CUBE], false, false, false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_FLOOR:
		modelStack.PushMatrix();
		modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
		modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
		RenderMesh(meshList[GEO_FLOOR], true, false, false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_WALL:
		modelStack.PushMatrix();
		modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
		modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
		RenderMesh(meshList[GEO_WALL], false, false, false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_PLAY:
		modelStack.PushMatrix();
		modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
		modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
		RenderMesh(meshList[GEO_PLAY], false, false, false);
		modelStack.PopMatrix();
		break;
	case GameObject::GO_LOGO:
		modelStack.PushMatrix();
		modelStack.Translate(go->transform.position.x, go->transform.position.y, go->transform.position.z);
		modelStack.Rotate(go->transform.rotation, 1, 0, 0);
		modelStack.Scale(go->transform.scale.x, go->transform.scale.y, go->transform.scale.z);
		RenderMesh(meshList[GEO_LOGO], false, false, false);
		modelStack.PopMatrix();
		break;
	}
}

void MenuScene::Render()
{
	RenderPassGPass();

	RenderPassMain();
}

void MenuScene::Exit()
{
	SceneBase::Exit();
	gl.Exit();
}