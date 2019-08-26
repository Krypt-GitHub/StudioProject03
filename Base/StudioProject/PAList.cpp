#include "PAList.h"

PAList::PAList()
{
}


PAList::~PAList()
{
}



Particle* PAList::FetchPA()
{
	for (std::vector<Particle *>::iterator it = m_PAList.begin(); it != m_PAList.end(); ++it)
	{
		Particle *pa = (Particle *)*it;
		if (!pa->GetActive())
		{
			return pa;
		}
	}
	//GameObject *pa = new GameObject;
	//m_PAList.push_back(pa);
	//return pa;
}

void PAList::Exit()
{
	while (m_PAList.size() > 0)
	{
		Particle *pa = m_PAList.back();
		delete pa;
		m_PAList.pop_back();
	}
}
