#include "GameObject.h"

GameObject::GameObject(GO_TYPE typeValue)
	: type(typeValue)
	, m_bisActive(false)
	, isStatic(false)
	, mass(0)
	, dir(Vector3(0, 0, 0))
{
}

GameObject::~GameObject()
{
}

void GameObject::SetActive(bool _active)
{
	m_bisActive = _active;
}

bool GameObject::GetActive()
{
	return m_bisActive;
}