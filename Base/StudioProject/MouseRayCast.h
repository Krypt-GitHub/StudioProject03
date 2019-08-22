#pragma once

#include "Vector3.h"
#include "Vector2.h"
#include "Mtx44.h"
#include "FirstPersonCamera.h"

class MouseRayCast
{
private:
	Vector3 currentRay;
	FirstPersonCamera camera;
public:
	Mtx44 projectionMatrix;
	Mtx44 viewMatrix;
	MouseRayCast();
	MouseRayCast(FirstPersonCamera _camera, Mtx44 _projection);
	~MouseRayCast();

	Vector3 getCurrentRay();
	void Update(double dt);
	Vector3 calculateMouseRay();
	Vector3 toWorldCoords(Vector3 _eyeCoords);
	Vector3 toEyeCoords(Vector3 _clipCoords);
	Vector2 getNormalisedDeviceCoords(float _mouseX, float _mouseY);
};

