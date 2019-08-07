#include "Camera.h"
#include "Application.h"
#include "Mtx44.h"

Camera::Camera()
{
	Reset();
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = target;
	this->up = up;
}

void Camera::Reset()
{
	position.Set(1, 0, 0);
	target.Set(0, 0, 0);
	up.Set(0, 1, 0);
}

void Camera::Update(double dt)
{
	static const float CAMERA_SPEED = 20.f;
	if(Application::GetKeyDown(VK_LEFT) || Application::GetKeyDown('A'))
	{
		position.x -= (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::GetKeyDown(VK_RIGHT) || Application::GetKeyDown('D'))
	{
		position.x += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::GetKeyDown(VK_UP) || Application::GetKeyDown('W'))
	{
		position.y += (float)(CAMERA_SPEED * 0.2 * dt);
	}
	if(Application::GetKeyDown(VK_DOWN) || Application::GetKeyDown('S'))
	{
		position.y -= (float)(CAMERA_SPEED * 0.2 * dt);
	}
}