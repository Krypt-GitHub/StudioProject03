#include "GameObject.h"

GameObject::GameObject(GO_TYPE typeValue)
	: type(typeValue)
	, m_bisActive(false)
	, m_bisStatic(false)
	, m_fmass(0)
	, m_v3dir(Vector3(0, 0, 0))
	, m_v3vel(Vector3(0, 0, 0))
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

void GameObject::SetStatic(bool _static)
{
	m_bisStatic = _static;
}

bool GameObject::GetStatic()
{
	return m_bisStatic;
}

void GameObject::SetMass(float _mass)
{
	m_fmass = _mass;
}

float GameObject::GetMass()
{
	return m_fmass;
}