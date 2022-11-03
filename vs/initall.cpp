#include "Unifiedheader.h"
#include "gameobject.h"

void initAll() {
	g_TimeManger.SetStartTime();

	spaceship* s = new spaceship();
	g_gameObjectPool.m_gameObjectList.push_back(s);
	s->speed = 0;
	g_gameObjectPool.m_GlobalSpaceship = s;
}