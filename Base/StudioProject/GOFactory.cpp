#include "GOFactory.h"



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
	if (_type == GameObject::GO_ENEMY)
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
}