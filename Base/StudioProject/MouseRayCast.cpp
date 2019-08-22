#include "MouseRayCast.h"
#include "../Source/Application.h"


MouseRayCast::MouseRayCast()
{
}

MouseRayCast::MouseRayCast(FirstPersonCamera _camera, Mtx44 _projection)
{
	camera = _camera;
	projectionMatrix = _projection;
	viewMatrix = _camera.GetViewMatrix();
}


MouseRayCast::~MouseRayCast()
{
}

Vector3 MouseRayCast::getCurrentRay()
{
	return currentRay;
}

void MouseRayCast::Update(double dt)
{
	viewMatrix = camera.GetViewMatrix();
	currentRay = calculateMouseRay();
}

Vector3 MouseRayCast::calculateMouseRay()
{
	float mouseX = Application::GetMousePosX();
	float mouseY = Application::GetMousePosY();
	std::cout << mouseX << "::" << mouseY << std::endl;
	Vector2 normalisedCoords(getNormalisedDeviceCoords(mouseX, mouseY));
	Vector3 clipCoords = Vector3(normalisedCoords.x, normalisedCoords.y, -1.f);
	Vector3 eyeCoords = toEyeCoords(clipCoords);
	Vector3 worldRay = toWorldCoords(eyeCoords);
	return worldRay;
}

Vector3 MouseRayCast::toWorldCoords(Vector3 _eyeCoords)
{
	Mtx44 invertedView = viewMatrix.GetInverse();
	Vector3 rayWorld = invertedView.Multi(_eyeCoords, 0.f);
	Vector3 mouseRay = Vector3(rayWorld.x, rayWorld.y, rayWorld.z);
	mouseRay.Normalize();
	return mouseRay;
}

Vector3 MouseRayCast::toEyeCoords(Vector3 _clipCoords)
{
	Mtx44 invertedProjection = projectionMatrix.GetInverse();
	Vector3 eyeCoords = invertedProjection.Multi(_clipCoords, 1.f);
	return Vector3(eyeCoords.x, eyeCoords.y, -1);
}

Vector2 MouseRayCast::getNormalisedDeviceCoords(float _mouseX, float _mouseY)
{
	float x = (2.f * _mouseX) / 1920 - 1;
	float y = (2.f * _mouseY) / 1080 - 1;
	return Vector2(x, -y);;
}
