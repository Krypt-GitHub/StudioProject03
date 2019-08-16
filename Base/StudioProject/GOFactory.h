#ifndef GO_FACTORY_H
#define GO_FACTORY_H

#include "GOList.h"


class GOFactory
{
public:

	GOList &gl = GOList::GetInstance();

	GOFactory();
	~GOFactory();

	void CreateGO(std::string _name, GameObject::GO_TYPE _type, bool _static, float _mass, Vector3 _position, Vector3 _scale, float _rotation, Vector3 _obbScale);

	GameObject * GameObjectFactory(std::string _name, GameObject::GO_TYPE _type, bool _static, float _mass, Vector3 _position, Vector3 _scale, float _rotation, Vector3 _obbScale);

	GameObject * GameObjectFactory(std::string _name, GameObject::GO_TYPE _type, bool _static, float _mass, Vector3 _position, Vector3 _scale, float _rotation);
	
	//EnemyGO* SpawnEnemy(std::string _name, bool _static, float mass, Vector3 _position, Vector3 _scale, float _rotation);
};

#endif