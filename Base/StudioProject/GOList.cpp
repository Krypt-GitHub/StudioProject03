#include "GOList.h"

GOList::GOList()
{
	for (int i = 0; i < 100; i++)
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

GameObject* GOList::CreateGO(std::string _name, GameObject::GO_TYPE _type, bool _static, float mass, Vector3 _position, Vector3 _scale, float rotation)
{
	GameObject *go = FetchGO();
	go->name = _name;
	go->type = _type;
	go->SetActive(true);
	go->SetStatic(_static);
	go->SetMass(mass);
	go->transform.SetTransform(_position, _scale, rotation);
	return go;
}

void GOList::Exit()
{
	while (m_goList.size() > 0)
	{
		GameObject *go = m_goList.back();
		delete go;
		m_goList.pop_back();
	}
}
