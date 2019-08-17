#include "GOFactory.h"
#include "PlayerGO.h"
#include "EnemyGO.h"
#include "PistolGO.h"

GOFactory::GOFactory()
{
}

GOFactory::~GOFactory()
{
}

void GOFactory::CreateGO(std::string _name, GameObject::GO_TYPE _type, bool _static, float _mass, Vector3 _position, Vector3 _scale, float _rotation, Vector3 _obbScale, float _obbRotation, Vector3 _obbRotateAxis)
{
	gl.m_goList.push_back(GameObjectFactory(_name, _type, _static, _mass, _position, _scale, _rotation, _obbScale, _obbRotation,_obbRotateAxis));
}

GameObject* GOFactory::GameObjectFactory(std::string _name, GameObject::GO_TYPE _type, bool _static, float _mass, Vector3 _position, Vector3 _scale, float _rotation, Vector3 _obbScale, float _obbRotation, Vector3 _obbRotateAxis)
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
		goPlayer->obb.UpdateAxis(Vector3(1, 0, 0), Vector3(0, 0, 1));
		goPlayer->obb.UpdatePos(Vector3(_position.x, _position.y + 18, _position.z));
		goPlayer->obb.SetScale(_obbScale);
		goPlayer->obb.RotateAxis(0, Vector3(0, 1, 0));
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
		goEnemy->obb.UpdateAxis(Vector3(1, 0, 0), Vector3(0, 0, 1));
		goEnemy->obb.UpdatePos(Vector3(_position.x, _position.y + 18, _position.z));
		goEnemy->obb.SetScale(_obbScale);
		goEnemy->obb.RotateAxis(0, Vector3(0, 1, 0));
		return goEnemy;
	}
	else if (_type == GameObject::GO_PISTOL)
	{
		PistolGO* goPistol = new PistolGO;
		goPistol->name = _name;
		goPistol->type = _type;
		goPistol->SetActive(true);
		goPistol->SetStatic(_static);
		goPistol->SetMass(_mass);
		goPistol->transform.SetTransform(_position, _scale, _rotation);
		goPistol->obb.UpdateAxis(Vector3(1, 0, 0), Vector3(0, 0, 1));
		goPistol->obb.UpdatePos(Vector3(_position.x, _position.y + 18, _position.z));
		goPistol->obb.SetScale(_obbScale);
		goPistol->obb.RotateAxis(0, Vector3(0, 1, 0));
		return goPistol;
	}
	else
	{
		GameObject* gameObject = new GameObject;
		gameObject->name = _name;
		gameObject->type = _type;
		gameObject->SetActive(true);
		gameObject->SetStatic(_static);
		gameObject->SetMass(_mass);
		gameObject->transform.SetTransform(_position, _scale, _rotation);
		gameObject->obb.UpdateAxis(Vector3(1, 0, 0), Vector3(0, 0, 1));
		gameObject->obb.UpdatePos(Vector3(_position.x, _position.y, _position.z));
		gameObject->obb.SetScale(_obbScale);
		gameObject->obb.RotateAxis(0, Vector3(0, 1, 0));
		return gameObject;
	}
}