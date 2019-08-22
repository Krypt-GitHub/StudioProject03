#ifndef COLLIDER_H
#define COLLIDER_H
#include "Vector3.h"
#include "Mtx44.h"
class Collider
{
private:
	bool isEnabled;


	bool GetSeparatingPlane(Vector3& RPos,Vector3& Plane,Collider& OtherBox);
public:
	// Members
	Collider();
	Collider(Vector3 Pos,Vector3 scale,Vector3 AxisX,Vector3 AxisZ);
	~Collider();

	Vector3 AxisX;
	Vector3 AxisY;
	Vector3 AxisZ;
	Vector3 pos;
	Vector3 Half_size;
	void Enable();
	bool GetCollision( Collider&box2);
	void UpdateAxis(Vector3 AxisX , Vector3 AxisZ);
	void UpdatePos(Vector3 pos);
	void RotateAxis(float degree, Vector3 rotateAxis);
	void SetScale(Vector3 scale);
	void Render();
};

#endif
