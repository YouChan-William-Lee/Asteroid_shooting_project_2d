#include "Unifiedheader.h"
#include "gameobject.h"


void initAll() { 
	g_Timemanger.SetStartTime() ; 

	// change 0 to number if you want to start with many  asteroid
	for (float i = 1 ; i < 0; i += 1) { 
		Asteroid *a = new Asteroid;
		
		a->generateAsteroidOutline(Asteroidmaxradius, Asteroidminradius);
		a->pos.x = i * 3 - 7 * 3;
		a->pos.y = -15; 
		a->rotate = 3.14f / 2.0f;
		a->speed = -0.0005f * GetRandom(5);
		a->rColour = GetRandom(1);
		a->gColour = GetRandom(1);
		a->bColour = GetRandom(1);
		g_gameobjectpool.m_gameobjectlist.push_back((gameojbect*) a);
 	}

	spaceship *s = new spaceship();
	g_gameobjectpool.m_gameobjectlist.push_back(s); 
	s->speed = 0;
	g_gameobjectpool.m_Globalspaceship = s; 
}