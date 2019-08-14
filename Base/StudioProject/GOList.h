#ifndef GOLIST_H
#define GOLIST_H

#include "../Core/GameObject.h"
#include <vector>

class GOList
{
public:
	static GOList& GetInstance()
	{
		static GOList golist;
		return golist;
	}

	std::vector <GameObject* > m_goList;

	GOList();
	~GOList();
	GameObject* FetchGO();
	GameObject* FetchGO(GameObject::GO_TYPE _type);
	GameObject* CreateGO(std::string _name, GameObject::GO_TYPE _type, bool _static, float mass, Vector3 _position, Vector3 _scale, float rotation);


	void Exit();
};

#endif
