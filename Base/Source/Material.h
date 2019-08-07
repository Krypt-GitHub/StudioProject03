#ifndef MATERIAL_H
#define MATERIAL_H

struct Components
{
	float r, g, b;
	Components(float r = 0.1f, float g = 0.1f, float b = 0.1f) {Set(r, g, b);}
	void Set(float r, float g, float b) {this->r = r; this->g = g; this->b = b;}
};
struct Material
{
	Components kAmbient;
	Components kDiffuse;
	Components kSpecular;
	float kShininess;

	Material()
	{
		//some default values
		kAmbient.Set(0.1f, 0.1f, 0.1f);
		kDiffuse.Set(0.6f, 0.6f, 0.6f);
		kSpecular.Set(0.3f, 0.3f, 0.3f);
		kShininess = 5.f;
	}
};

#endif