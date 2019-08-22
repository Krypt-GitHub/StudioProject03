#include "Ray.h"





Ray::Ray(Vector3 dir, Vector3 pos)
{
	this->dir = dir;
	this->Pos = pos;
}

Ray::~Ray()
{
}

bool Ray::IntersectionOBB(Collider& box)
{
	float tMin = 0;
	float tMax = 1000000000000000000.f;
	Vector3 delta = box.pos - this->Pos;

	if (delta.Length() > 50)
	{
		return false;
	}

	{
		float e = box.AxisX.Dot(delta);
		float f = dir.Dot(box.AxisX);
		if (Math::FAbs(f) > 0.001f)
		{
			float interection1 = ((e + (-box.Half_size.x)) / f);
			float interection2 = ((e + (box.Half_size.x)) / f);
			
			if (interection1 > interection2)
			{
				float temp = interection1;
				interection1 = interection2;
				interection2 = temp;
			}
			if (interection2 < tMax)
				tMax = interection2;
			if (interection1 > tMin)
				tMin = interection1;

			if (tMax < tMin)
				return false;
		}
		else
		{
			if (-e + (-box.Half_size.x) > 0.f || -e+(box.Half_size.x) < 0.0f)
				return false;
		}
	}
	{
		float e = box.AxisY.Dot(delta);
		float f = dir.Dot(box.AxisY);
		if (Math::FAbs(f) > 0.001f)
		{
			float interection1 = ((e + (-box.Half_size.y)) / f);
			float interection2 = ((e + (box.Half_size.y)) / f);

			if (interection1 > interection2)
			{
				float temp = interection1;
				interection1 = interection2;
				interection2 = temp;
			}
			if (interection2 < tMax)
				tMax = interection2;
			if (interection1 > tMin)
				tMin = interection1;

			if (tMax < tMin)
				return false;
		}
		else
		{
			if (-e + (-box.Half_size.y) > 0.f || -e + (box.Half_size.y) < 0.0f)
				return false;
		}
	}
	{
		float e = box.AxisZ.Dot(delta);
		float f = dir.Dot(box.AxisZ);
		if (Math::FAbs(f) > 0.001f)
		{
			float interection1 = ((e + (-box.Half_size.z)) / f);
			float interection2 = ((e + (box.Half_size.z)) / f);

			if (interection1 > interection2)
			{
				float temp = interection1;
				interection1 = interection2;
				interection2 = temp;
			}
			if (interection2 < tMax)
				tMax = interection2;
			if (interection1 > tMin)
				tMin = interection1;

			if (tMax < tMin)
				return false;
		}
		else
		{
			if (-e + (-box.Half_size.z) > 0.f || -e + (box.Half_size.z) < 0.0f)
				return false;
		}
	}

	if (tMin > 35)
		return false;
	


	return true;
}

bool Ray::IntersectionOBB(Collider & box, Vector3 output)
{
	float tMin = 0;
	float tMax = 1000000000000000000.f;
	Vector3 delta = box.pos - this->Pos;


	{
		float e = box.AxisX.Dot(delta);
		float f = dir.Dot(box.AxisX);
		if (Math::FAbs(f) > 0.001f)
		{
			float interection1 = ((e + (-box.Half_size.x)) / f);
			float interection2 = ((e + (box.Half_size.x)) / f);

			if (interection1 > interection2)
			{
				float temp = interection1;
				interection1 = interection2;
				interection2 = temp;
			}
			if (interection2 < tMax)
				tMax = interection2;
			if (interection1 > tMin)
				tMin = interection1;

			if (tMax < tMin)
				return false;
		}
		else
		{
			if (-e + (-box.Half_size.x) > 0.f || -e + (box.Half_size.x) < 0.0f)
				return false;
		}
	}
	{
		float e = box.AxisY.Dot(delta);
		float f = dir.Dot(box.AxisY);
		if (Math::FAbs(f) > 0.001f)
		{
			float interection1 = ((e + (-box.Half_size.y)) / f);
			float interection2 = ((e + (box.Half_size.y)) / f);

			if (interection1 > interection2)
			{
				float temp = interection1;
				interection1 = interection2;
				interection2 = temp;
			}
			if (interection2 < tMax)
				tMax = interection2;
			if (interection1 > tMin)
				tMin = interection1;

			if (tMax < tMin)
				return false;
		}
		else
		{
			if (-e + (-box.Half_size.y) > 0.f || -e + (box.Half_size.y) < 0.0f)
				return false;
		}
	}
	{
		float e = box.AxisZ.Dot(delta);
		float f = dir.Dot(box.AxisZ);
		if (Math::FAbs(f) > 0.001f)
		{
			float interection1 = ((e + (-box.Half_size.z)) / f);
			float interection2 = ((e + (box.Half_size.z)) / f);

			if (interection1 > interection2)
			{
				float temp = interection1;
				interection1 = interection2;
				interection2 = temp;
			}
			if (interection2 < tMax)
				tMax = interection2;
			if (interection1 > tMin)
				tMin = interection1;

			if (tMax < tMin)
				return false;
		}
		else
		{
			if (-e + (-box.Half_size.z) > 0.f || -e + (box.Half_size.z) < 0.0f)
				return false;
		}
	}


	output = dir * tMin;

	return true;
}
