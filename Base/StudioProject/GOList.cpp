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

GameObject* GOList::FetchGO(GameObject::GO_TYPE _type)
{
	for (std::vector<GameObject *>::iterator it = m_goList.begin(); it != m_goList.end(); ++it)
	{
		GameObject *go = (GameObject *)*it;
		if (go->GetActive())
		{
			if (go->type == _type)
			{
				return go;
			}
		}
	}
}

void GOList::Exit()
{

}
