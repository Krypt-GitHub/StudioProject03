#ifndef SCENE_BASE_H
#define SCENE_BASE_H

#include "../Core/Scene.h"
#include "Mtx44.h"
#include "FirstPersonCamera.h"
#include "../Source/Mesh.h"
#include "MatrixStack.h"
#include "../Source/Light.h"
#include "../Core/GameObject.h"
#include <vector>
#include "../Source/DepthFBO.h"
#include "../Source/Particle.h"
#include "../Core/ParticleEngine.h"
#include "..//Core/SoundEngine.h"

class SceneBase : public Scene
{
public:

	SceneBase();
	~SceneBase();

	virtual void Init();
	virtual void Update(double dt);
	void RenderMesh(Mesh * mesh, bool enableLight, bool enableMT, bool enableReflect);
	void RenderText(Mesh * mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh * mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh * mesh, bool enableLight, float sizeX, float sizeY, float x, float y, float rotation, Vector3 rotateAxis);
	void RenderParticle();
	void RenderScreenText(int level, int counter);
	virtual void Render();
	virtual void Exit();

protected:
	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
	};
	enum GEOMETRY_TYPE
	{
		// Menu
		GEO_PLAY,
		// Menu
		GEO_LOGO,
		// Misc
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_FIST,
		GEO_LIGHTBALL,
		// Geometry
		GEO_QUAD,
		GEO_CUBE,
		GEO_RING,
		GEO_CONE,
		GEO_TEXT,
		// Environment
		GEO_FLOOR,
		GEO_WALL,
		GEO_PILLAR,
		// GameObjects
		GEO_PISTOL,
		GEO_BULLET,
		GEO_TRACER,
		// Player
		GEO_HANDS,
		GEO_ENEMY_STAND,
		GEO_ENEMY_WALK01,
		GEO_ENEMY_WALK02,
		GEO_ENEMY_SHOOT01,
		GEO_ENEMY_SHOOT02,
		GEO_ENEMY_BONUS,
		//TSL
		GEO_SKYDOME,
		GEO_TERRAIN,
		GEO_WATER,
		GEO_SPRITE_ANIMATION,
		// Shadow
		GEO_LIGHT_DEPTH_QUAD,
		GEO_OBB_GREEN,
		GEO_OBB_RED,
		GEO_OBB_BLUE,
		GEO_PA_GUNSHATTER,
		GEO_PA_GUNSMOKE,
		GEO_PA_ENEMYSHATTER1,
		GEO_PA_ENEMYSHATTER2,
		// Text
		GEO_TIME,
		GEO_KILL,
		GEO_THEM,
		GEO_ALL,
		GEO_GRAB,
		GEO_THE,
		GEO_GUN,
		GEO_SUPER,
		GEO_COOL,
		GEO_1,
		GEO_2,
		GEO_3,
		GEO_4,
		GEO_5,
		GEO_6,
		GEO_7,
		GEO_8,
		GEO_9,
		GEO_10,
		GEO_11,
		GEO_12,
		NUM_GEOMETRY,
	};
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE_ENABLED1,
		U_COLOR_TEXTURE_ENABLED2,
		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE1,
		U_COLOR_TEXTURE2,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLED,
		// Shadow
		U_LIGHT_DEPTH_MVP_GPASS,
		U_LIGHT_DEPTH_MVP,
		U_SHADOW_MAP,
		// MT
		U_MTENABLED,
		U_ELAPSEDTIME,
		// Reflection
		U_ENABLEREFLECT,
		U_CAMERAPOS,
		U_SKYDOME,
		// Shadow Alpha
		U_SHADOW_COLOR_TEXTURE,
		U_SHADOW_COLOR_TEXTURE1,
		U_SHADOW_COLOR_TEXTURE2,
		U_SHADOW_COLOR_TEXTURE_ENABLED,
		U_SHADOW_COLOR_TEXTURE_ENABLED1,
		U_SHADOW_COLOR_TEXTURE_ENABLED2,
		U_TOTAL,
	};

	// Shadow
	unsigned m_gPassShaderID;
	DepthFBO m_lightDepthFBO;

	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;

	RENDER_PASS m_renderPass;

	unsigned m_vertexArrayID;
	unsigned cubeVAO;
	Mesh* meshList[NUM_GEOMETRY];

	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	FirstPersonCamera camera[2] = {};
	int cameraID;

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[2];

	bool bLightEnabled;

	float fps;

	float m_fFOV;
	// Terrain
	std::vector<unsigned char> m_heightMap;

	float m_fMTElapsedTime;

	// Scene Objects
	float m_fchRotate;

	// Screen Text
	int m_iTextCounter;
	float m_fTextTimer;
	bool m_bRenderScreenText;
	int m_iSoundCounter;
};

#endif
