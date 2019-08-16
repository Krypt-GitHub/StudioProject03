#include "GOFactory.h"
#include "PlayerGO.h"
#include "EnemyGO.h"

GOFactory::GOFactory()
{
}

GOFactory::~GOFactory()
{
}

void GOFactory::CreateGO(std::string _name, GameObject::GO_TYPE _type, bool _static, float _mass, Vector3 _position, Vector3 _scale, float _rotation)
{
	gl.m_goList.push_back(GameObjectFactory(_name, _type, _static, _mass, _position, _scale, _rotation));
}

GameObject* GOFactory::GameObjectFactory(std::string _name, GameObject::GO_TYPE _type, bool _static, float _mass, Vector3 _position, Vector3 _scale, float _rotation)
{
	if (_type == GameObject::GO_PLAYER)
	{
		PlayerGO* goPlayer = new PlayerGO;
		goPlayer->name = _name;
		goPlayer->type = _type;
		goPlayer->SetActive(true);
		goPlayer->SetStatic(_static);
		goPlayer->SetMass(_mass);
		goPlayer->transform.SetTransform(_position, _scale, _rotation);
		return goPlayer;
	}
	else if (_type == GameObject::GO_ENEMY)
	{
		EnemyGO* goEnemy = new EnemyGO;
		goEnemy->name = _name;
		goEnemy->type = _type;
		goEnemy->SetActive(true);
		goEnemy->SetStatic(_static);
		goEnemy->SetMass(_mass);
		goEnemy->transform.SetTransform(_position, _scale, _rotation);
		return goEnemy;
	}
	else if (_type == GameObject::GO_PISTOL)
	{

	}
}