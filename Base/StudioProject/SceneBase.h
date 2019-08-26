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
	virtual void Render();
	virtual void Exit();

protected:
	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
	};
public:
	enum GEOMETRY_TYPE
	{
		// Menu
		GEO_PLAY,
		// Window
		GLASS_01,
		GLASS_02,
		GLASS_03,
		GLASS_04,
		GLASS_05,
		GLASS_06,
		GLASS_07,
		GLASS_08,
		GLASS_09,
		GLASS_10,
		GLASS_11,
		GLASS_12,
		GLASS_13,
		GLASS_14,
		GLASS_15,
		GLASS_16,
		GLASS_17,
		GLASS_18,
		GLASS_19,
		GLASS_20,
		// Menu
		GEO_LOGO,
		// Misc
		GEO_AXES,
		GEO_CROSSHAIR,
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
		NUM_GEOMETRY,
	};
protected:
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
	public:
	Mesh* meshList[NUM_GEOMETRY];
	protected:
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
};

#endif