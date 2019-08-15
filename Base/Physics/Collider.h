#ifndef COLLIDER_H
#define COLLIDER_H
#include "Vector3.h"
#include "Mtx44.h"
class Collider
{
private:
	bool isEnabled;
	bool getSeparatingPlane(Vector3& RPos,Vector3& Plane,Collider& OtherBox);
public:
	// Members
	Collider();
	Collider(Vector3 Pos,Vector3 scale,Vector3 AxisX,Vector3 AxisZ);
	~Collider();

	Vector3 pos;
	Vector3 Half_size;
	Vector3 AxisX;
	Vector3 AxisY;
	Vector3 AxisZ;
	void Enable();
	bool getCollision( Collider&box2);
	void upDateAxis(Vector3 AxisX , Vector3 AxisZ);
	void upDatePos(Vector3 pos);
	void rotateAxis(float degree, Vector3 rotateAxis);
	void setScale(Vector3 scale);
	void Render();
};

#endif
