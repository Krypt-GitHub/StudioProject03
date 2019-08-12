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
	GameObject *FetchGO();
};

#endif