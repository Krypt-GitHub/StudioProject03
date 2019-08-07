#ifndef BASE_CAMERA_H
#define BASE_CAMERA_H

#include "Mtx44.h"

class BaseCamera
{
public:
	BaseCamera();
	~BaseCamera();

	virtual Mtx44 GetViewMatrix() = 0;
};

#endif