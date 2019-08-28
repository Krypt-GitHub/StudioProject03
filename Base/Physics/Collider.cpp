#include "Collider.h"

Collider::Collider()
{
	isEnabled = true;
}

Collider::Collider(Vector3 Pos, Vector3 scale, Vector3 AxisX, Vector3 AxisZ)
{
	this->pos = Pos;
	this->Half_size = scale;
	this->AxisX = AxisX;
	this->AxisY = AxisX.Cross(AxisZ);
	this->AxisZ = AxisZ;
	isEnabled = true;
}


Collider::~Collider()
{
}

bool Collider::GetSeparatingPlane(Vector3& RPos, Vector3& Plane,Collider& OtherBox)
{
	return (fabs(RPos.Dot(Plane)) >
		(fabs((this->AxisX*this->Half_size.x).Dot(Plane)) +
			fabs((this->AxisY*this->Half_size.y).Dot(Plane)) +
			fabs((this->AxisZ*this->Half_size.z).Dot(Plane)) +
			fabs((OtherBox.AxisX*OtherBox.Half_size.x).Dot(Plane)) +
			fabs((OtherBox.AxisY*OtherBox.Half_size.y).Dot(Plane)) +
			fabs((OtherBox.AxisZ*OtherBox.Half_size.z).Dot(Plane))));
}

bool Collider::GetCollision(Collider& box2)
{
	{
		Vector3 RPos;
		RPos = box2.pos - this->pos;

		return !(GetSeparatingPlane(RPos, this->AxisX, box2) ||
			GetSeparatingPlane(RPos, this->AxisY, box2) ||
			GetSeparatingPlane(RPos, this->AxisZ, box2) ||
			GetSeparatingPlane(RPos, box2.AxisX, box2) ||
			GetSeparatingPlane(RPos, box2.AxisY, box2) ||
			GetSeparatingPlane(RPos, box2.AxisZ, box2) ||
			GetSeparatingPlane(RPos, this->AxisX.Cross(box2.AxisX), box2) ||
			GetSeparatingPlane(RPos, this->AxisX.Cross(box2.AxisY), box2) ||
			GetSeparatingPlane(RPos, this->AxisX.Cross(box2.AxisZ), box2) ||
			GetSeparatingPlane(RPos, this->AxisY.Cross(box2.AxisX), box2) ||
			GetSeparatingPlane(RPos, this->AxisY.Cross(box2.AxisY), box2) ||
			GetSeparatingPlane(RPos, this->AxisY.Cross(box2.AxisZ), box2) ||
			GetSeparatingPlane(RPos, this->AxisZ.Cross(box2.AxisX), box2) ||
			GetSeparatingPlane(RPos, this->AxisZ.Cross(box2.AxisY), box2) ||
			GetSeparatingPlane(RPos, this->AxisZ.Cross(box2.AxisZ), box2));
	}
}

void Collider::UpdateAxis(Vector3 AxisX, Vector3 AxisZ)
{
	this->AxisX = AxisX;
	this->AxisZ = AxisZ;
	this->AxisY = AxisZ.Cross(AxisX);
}

void Collider::UpdatePos(Vector3 pos)
{
	this->pos = pos;
}

void Collider::RotateAxis(float degree, Vector3 rotateAxis)
{
	rotation = degree;
	Mtx44 rotate;
	rotate.SetToRotation(degree, rotateAxis.x, rotateAxis.y, rotateAxis.z);
	AxisX = rotate * AxisX;
	AxisY = rotate * AxisY;
	AxisZ = rotate * AxisZ;
}

void Collider::SetScale(Vector3 scale)
{
	Half_size = scale;
}

