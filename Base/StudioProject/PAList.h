#ifndef PAList_H
#define PAList_H

#include "../Core/GameObject.h"
#include <vector>
#include "../Source/Particle.h"

class PAList
{
public:
	static PAList& GetInstance()
	{
		static PAList PAList;
		return PAList;
	}

	std::vector <Particle* > m_PAList;

	PAList();
	~PAList();

	Particle* FetchPA();
	void Exit();
};

#endif
