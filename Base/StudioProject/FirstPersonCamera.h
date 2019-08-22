#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H

#include "../Core/BaseCamera.h"

class FirstPersonCamera : public BaseCamera
{
public:
	Vector3 position;
	Vector3 target;
	Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	FirstPersonCamera();
	virtual ~FirstPersonCamera();

	virtual Mtx44 GetViewMatrix();

	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();

	//virtual Vector3 GetCameraPos() const;
	//virtual void SetCameraPos(Vector3 pos);
	//virtual Vector3 GetCameraTarget() const;
	//virtual void SetCameraTarget(Vector3 pos);
	//virtual Vector3 GetCameraUp() const;
	//virtual void SetCameraUp(Vector3 pos);
};

#endif
