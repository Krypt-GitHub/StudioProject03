#include "OurScene.h"
#include "GL\glew.h"

#include "../Source/shader.hpp"
#include "../Source/MeshBuilder.h"
#include "../Source/Application.h"
#include "../Source/Utility.h"
#include "../Source/LoadTGA.h"
#include <sstream>
#include <time.h>

OurScene::OurScene()
{
	srand(time(NULL));
}

OurScene::~OurScene()
{
}

void OurScene::Init()
{
	SceneBase::Init();

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	m_programID = LoadShaders("Shader//Shadow.vert", "Shader//Shadow.frag");

	// Shadow
	m_gPassShaderID = LoadShaders("Shader//GPass.vert", "Shader//GPass.frag");
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");
	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");
	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	m_parameters[U_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_programID, "colorTextureEnabled[2]");
	m_parameters[U_COLOR_TEXTURE2] = glGetUniformLocation(m_programID, "colorTexture[2]");
	m_parameters[U_SKYDOME] = glGetUniformLocation(m_programID, "skybox");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	// FOG
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.enabled");

	// Shadow Alpha
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE] = glGetUniformLocation(m_gPassShaderID, "colorTexture[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE] = glGetUniformLocation(m_gPassShaderID, "colorTexture[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[2]");
	m_parameters[U_SHADOW_COLOR_TEXTURE] = glGetUniformLocation(m_gPassShaderID, "colorTexture[2]");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_POINT;
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

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(465, 255, 375);
	lights[1].color.Set(0.24f, 0.19f, 0.06f);
	lights[1].power = 10.f;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	// Init FOG
	Color fogColor(0.24f, 0.19f, 0.06f);

	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
	glUniform1f(m_parameters[U_FOG_START], 10);
	glUniform1f(m_parameters[U_FOG_END], 1000);
	glUniform1f(m_parameters[U_FOG_DENSITY], 0.005f);
	glUniform1i(m_parameters[U_FOG_TYPE], 0);
	glUniform1i(m_parameters[U_FOG_ENABLED], 1);


	camera.Init(Vector3(-420, 184, 178), Vector3(0, 185, 0), Vector3(0, 1, 0));

	//camera.Init(Vector3(lights[1].position.x, lights[1].position.y, lights[1].position.z), Vector3(0, 0, 0), Vector3(0, 1, 0));


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
	meshList[GEO_TERRAIN] = MeshBuilder::GenerateTerrain("GEO_TERRAIN", "Image//heightmap.raw", m_heightMap);
	meshList[GEO_TERRAIN]->textureArray[0] = LoadTGA("Image//grass.tga");


	// Sprites
	//meshList[GEO_SPRITE_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("flame", 8, 8);
	//meshList[GEO_SPRITE_ANIMATION]->textureArray[0] = LoadTGA("Image//flame.tga");

	//SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
	//if (sa)
	//{
	//	sa->m_anim = new Animation();
	//	sa->m_anim->Set(0, 8, 0, 1.f, true);
	//}

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

void OurScene::Update(double dt)
{
	SceneBase::Update(dt);

	if (Application::GetKeyDown('8'))
	{
		bLightEnabled = true;
	}
	else if (Application::GetKeyDown('9'))
	{
		bLightEnabled = false;
	}

	camera.Update(dt);
	fps = (float)(1.f / dt);
}

void OurScene::RenderWorld()
{
	//modelStack.PushMatrix();
	//modelStack.Translate(13, 140, 90);
	//modelStack.Rotate(180, 1, 0, 0);
	//modelStack.Rotate(220, 0, 1, 0);
	//modelStack.Scale(120.f, 120.f, 0.f);
	//RenderMesh(meshList[GEO_SPRITE_ANIMATION], false, false, false);
	//modelStack.PopMatrix();

	//for (std::vector<Particle *>::iterator it = m_paList.begin(); it != m_paList.end(); ++it)
	//{
	//	Particle *pa = (Particle *)*it;
	//	if (pa->active)
	//	{
	//		RenderPA(pa);
	//	}
	//}
}

void OurScene::RenderWater()
{
	modelStack.PushMatrix();
	modelStack.Translate(-300, 70, -630);
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Rotate(-25, 0, 0, 1);
	modelStack.Scale(1200.f, 1200.f, 0.f);
	RenderMesh(meshList[GEO_WATER], false, true, true);
	modelStack.PopMatrix();
}

void OurScene::RenderPassGPass()
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

void OurScene::RenderPassMain()
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
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	projectionStack.LoadMatrix(perspective);
	// Camera matrix        
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);

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

	RenderTerrain();

	RenderWater();

	modelStack.PushMatrix();
	modelStack.Translate(0, 800, 0);
	RenderMesh(meshList[GEO_SKYDOME], false, false, false);
	modelStack.PopMatrix();

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

	// Particle
	//for (std::vector<Particle *>::iterator it = m_paList.begin(); it != m_paList.end(); ++it)
	//{
	//	Particle *pa = (Particle *)*it;
	//	if (pa->active)
	//	{
	//		RenderPA(pa);
	//	}
	//}
}

void OurScene::RenderTerrain()
{
	modelStack.PushMatrix();
	modelStack.Scale(4000.0f, 350.f, 4000.0f); // values varies. 
	RenderMesh(meshList[GEO_TERRAIN], true, false, false);
	modelStack.PopMatrix();
}

void OurScene::Render()
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

void OurScene::Exit()
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
