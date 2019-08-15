#include "Collider.h"

Collider::Collider()
{
}

Collider::Collider(Vector3 Pos, Vector3 scale, Vector3 AxisX, Vector3 AxisZ)
{
	this->pos = Pos;
	this->Half_size = scale;
	this->AxisX = AxisX;
	this->AxisY = AxisX.Cross(AxisZ);
	this->AxisZ = AxisZ;
}


Collider::~Collider()
{
}

void Collider::Enable()
{
	isEnabled = true;
}

bool Collider::getSeparatingPlane(Vector3& RPos, Vector3& Plane,Collider& OtherBox)
{
	return (fabs(RPos.Dot(Plane)) >
		(fabs((this->AxisX*this->Half_size.x).Dot(Plane)) +
			fabs((this->AxisY*this->Half_size.y).Dot(Plane)) +
			fabs((this->AxisZ*this->Half_size.z).Dot(Plane)) +
			fabs((OtherBox.AxisX*OtherBox.Half_size.x).Dot(Plane)) +
			fabs((OtherBox.AxisY*OtherBox.Half_size.y).Dot(Plane)) +
			fabs((OtherBox.AxisZ*OtherBox.Half_size.z).Dot(Plane))));
}

bool Collider::getCollision(Collider & box2)
{
	{
		Vector3 RPos;
		RPos = box2.pos - this->pos;

		return !(getSeparatingPlane(RPos, this->AxisX, box2) ||
			getSeparatingPlane(RPos, this->AxisY, box2) ||
			getSeparatingPlane(RPos, this->AxisZ, box2) ||
			getSeparatingPlane(RPos, box2.AxisX, box2) ||
			getSeparatingPlane(RPos, box2.AxisY, box2) ||
			getSeparatingPlane(RPos, box2.AxisZ, box2) ||
			getSeparatingPlane(RPos, this->AxisX.Cross(box2.AxisX), box2) ||
			getSeparatingPlane(RPos, this->AxisX.Cross(box2.AxisY), box2) ||
			getSeparatingPlane(RPos, this->AxisX.Cross(box2.AxisZ), box2) ||
			getSeparatingPlane(RPos, this->AxisY.Cross(box2.AxisX), box2) ||
			getSeparatingPlane(RPos, this->AxisY.Cross(box2.AxisY), box2) ||
			getSeparatingPlane(RPos, this->AxisY.Cross(box2.AxisZ), box2) ||
			getSeparatingPlane(RPos, this->AxisZ.Cross(box2.AxisX), box2) ||
			getSeparatingPlane(RPos, this->AxisZ.Cross(box2.AxisY), box2) ||
			getSeparatingPlane(RPos, this->AxisZ.Cross(box2.AxisZ), box2));
	}
}

void Collider::upDateAxis(Vector3 AxisX, Vector3 AxisZ)
{
	this->AxisX = AxisX;
	this->AxisZ = AxisZ;
	this->AxisY = AxisZ.Cross(AxisX);
}

void Collider::upDatePos(Vector3 pos)
{
	this->pos = pos;
}

void Collider::rotateAxis(float degree, Vector3 rotateAxis)
{
	Mtx44 rotate;
	rotate.SetToRotation(degree, rotateAxis.x, rotateAxis.y, rotateAxis.z);
	AxisX = rotate * AxisX;
	AxisY = rotate * AxisY;
	AxisZ = rotate * AxisZ;
}

void Collider::setScale(Vector3 scale)
{
	Half_size = scale;
}

