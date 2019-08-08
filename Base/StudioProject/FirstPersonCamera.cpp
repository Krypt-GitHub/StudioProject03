#include "FirstPersonCamera.h"
#include "../Source/Application.h"


FirstPersonCamera::FirstPersonCamera()
{
}


FirstPersonCamera::~FirstPersonCamera()
{
}

Mtx44 FirstPersonCamera::GetViewMatrix()
{
	Mtx44 result;
	result.SetToLookAt(position.x, position.y, position.z,
		target.x, target.y, target.z,
		up.x, up.y, up.z);
	return result;
}

void FirstPersonCamera::Init(const Vector3 & pos, const Vector3 & target, const Vector3 & up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();
}

void FirstPersonCamera::Update(double dt)
{
	static const float CAMERA_SPEED = 200.f;

	if (Application::GetKeyDown('A'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position -= right * CAMERA_SPEED * (float)dt;
		target -= right * CAMERA_SPEED * (float)dt;
	}
	if (Application::GetKeyDown('D'))
	{
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		position += right * CAMERA_SPEED * (float)dt;
		target += right * CAMERA_SPEED * (float)dt;
	}
	if (Application::GetKeyDown('W'))
	{
		Vector3 view = (target - position).Normalized();
		position += view * CAMERA_SPEED * (float)dt;
		target += view * CAMERA_SPEED * (float)dt;
	}
	if (Application::GetKeyDown('S'))
	{
		Vector3 view = (target - position).Normalized();
		position -= view * CAMERA_SPEED * (float)dt;
		target -= view * CAMERA_SPEED * (float)dt;
	}
	//Update the camera direction based on mouse move
	// left-right rotate
	{
		Vector3 view = (target - position).Normalized();
		float yaw = (float)(-100.f * Application::camera_yaw * (float)dt);
		Mtx44 rotation;
		rotation.SetToRotation(yaw, 0, 1, 0);
		view = rotation * view;
		target = position + view;
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
	}
	{
		float pitch = (float)(-100.f * Application::camera_pitch * (float)dt);
		Vector3 view = (target - position).Normalized();
		Vector3 right = view.Cross(up);
		right.y = 0;
		right.Normalize();
		up = right.Cross(view).Normalized();
		Mtx44 rotation;
		rotation.SetToRotation(pitch, right.x, right.y, right.z);
		view = rotation * view;
		target = position + view;
	}
	if (Application::GetKeyDown('R'))
	{
		Reset();
	}
}

void FirstPersonCamera::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}

