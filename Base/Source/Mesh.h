#ifndef MESH_H
#define MESH_H
#define MAX_TEXTURES	8

#include <string>
#include <vector>
#include "Material.h"

#include "Vertex.h"

class Mesh
{
public:
	enum DRAW_MODE
	{
		DRAW_TRIANGLES, //default mode
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
	};
	Mesh(const std::string &meshName);
	~Mesh();
	virtual void Render();
	void Render(unsigned offset, unsigned count);
	void UpdateVertices();

	std::vector<Vertex> allVertices;
	Mesh& operator=(const Mesh& other);

	std::string name;
	DRAW_MODE mode;

	Material material;
	unsigned vertexBuffer;
	unsigned colorBuffer;
	unsigned indexBuffer;
	unsigned indexSize;
	unsigned textureArray[MAX_TEXTURES];
};

#endif