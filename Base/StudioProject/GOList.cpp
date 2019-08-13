#include "GOList.h"

GOList::GOList()
{
	for (int i = 0; i < 50; i++)
	{
		m_goList.push_back(new GameObject(GameObject::GO_NONE));
	}
}


GOList::~GOList()
{
}

GameObject* GOList::FetchGO()
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (!go->GetActive())
		{
			return go;
		}
	}
}

void GOList::Exit()
{

}
