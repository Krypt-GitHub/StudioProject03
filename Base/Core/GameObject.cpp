#include "GameObject.h"

GameObject::GameObject(GO_TYPE typeValue)
	: type(typeValue),
	isActive(false),
	isStatic(false),
	mass()
{
}

GameObject::~GameObject()
{
}

void GameObject::SetActive(bool _active)
{
	isActive = _active;
}

void GameObject::AddComponent(Component _type)
{
}
