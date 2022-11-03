#pragma once

#include "Unifiedheader.h"
#include "time.h"
using namespace glm;  

//extern int g_gameScore;
class timemanager;
class ArenaManager;
class asteroidlauncher;
class objectpool;
class bulletmanager;
class bombmanager;

// extern bool g_needtoSetgameOver;
extern timemanager g_Timemanger;
extern ArenaManager g_Arenamanager;
extern asteroidlauncher g_AsteroidLauncher;
extern objectpool g_gameobjectpool; 
extern bulletmanager g_bulletmanager;
extern bombmanager g_bombmanager;

// game over -1, game first start 0, game playing 1
enum GAMESTATE {
	GAME_OVER  = -1,
	FIRST_START = 0,
	GAME_PLAYING = 1 
};
 
extern GAMESTATE g_gameState;

enum GAMEOBJECTTYPE {
	ERROR_DEFAULTOBJ = 0,
	ASTEROID = 1,
	BULLET = 2,
	SPACESHIP = 3,
	BOMB = 4
};
 
class timemanager {
public: 
	float GetTimefromClock() noexcept {  
		m_Lasttime = m_Currenttime = clock(); 
		return m_Currenttime;
	};

	float SetStartTime() { 
		GetTimefromClock(); //twice because of ellaps check
		m_GamestartTime = GetTimefromClock();
		return m_GamestartTime;
	}
	float Getsec() noexcept {
		return m_Currenttime *0.01f;
	}
	float GetDeltasec() noexcept {
		float delta = m_Currenttime - m_Lasttime;
		delta /= 0.01f;
		return delta;
	}

	float GetGameTime() noexcept {
		return m_Currenttime - m_GamestartTime;
	}
	float m_Lasttime;
	float m_Currenttime;
	float Maxsec;
	float m_GamestartTime;
};

class gameojbect {
public: 
	GAMEOBJECTTYPE type;
	vec2 pos;
	vec2 dir;  
	float speed;
	float radius;
	float rotate;
	float createdtime;
	float duration;
	float rColour;
	float gColour;
	float bColour;
	bool needtoremovefromlist;
	bool inArena;

	gameojbect() noexcept {
		type = ERROR_DEFAULTOBJ;
		speed = 0.001f;
		radius = 2;
		dir = vec2(0, 1);
		pos = vec2(0, 0);
		rotate = 0;
		duration = 20.0f;
		createdtime = g_Timemanger.Getsec();
		needtoremovefromlist = false;
		inArena = true;
	}

	virtual bool NeedtoKillthis() noexcept {
		if(type == SPACESHIP)
			return false;

		if(g_Timemanger.Getsec() - createdtime  >= duration)
			return true;
		else
			return false;
	}

	virtual	void process() {
		pos = pos + speed * dir ;
	}
	virtual	void render() noexcept {};
	virtual bool checkcollision(const vec2 & pos, float radiuofother) noexcept {
		return false;
	};
	virtual bool checkcollision(const gameojbect & other); 
};

constexpr float Asteroidmaxradius = 0.8f;
constexpr float Asteroidminradius = -0.3f; 
constexpr float AsteroidBaseRadius = 2.0f; 
constexpr float AsteroidRandomRadius = 1.5f; 

class Asteroid : public gameojbect {
public:
	Asteroid(void) noexcept;
	~Asteroid(void);  
	void generateAsteroidOutline(float max, float min);
	
	float drawingrotation; 
	////bigger stronger ,if shoot it divided by two 2/3 until too small
	vector<vec2> m_listofoutlinepoints;

	virtual void render() noexcept;
};

constexpr float spaceshipMAXSPEED = 0.20f; 
constexpr float spaceshipRotateSpeed = 0.4f;
constexpr float spaceshipAccel = 0.025f;
constexpr float spaceshipDeAccel = 0.018f;
constexpr float spaceshipNaturalDeAccel = 0.015f;

class spaceship : public gameojbect {
public:    
	spaceship() noexcept {
		type = SPACESHIP;
		duration = -1;
	}
	virtual void render() noexcept; 
	virtual void process(); 
	 
	vector<vec2> m_listofoutlinepoints;
};

class bullet : public gameojbect {    
public:
	bullet() {
		radius = 0.02f;
		type = BULLET;
	};
	virtual void render() noexcept; 
};

class bomb : public gameojbect {
public:
	bomb() {
		type = BOMB;
		radius = 3.0f;
	}
	virtual void render() noexcept;
};


class objectpool {
public:
	objectpool() noexcept {
		m_Globalspaceship = 0;
	}
	void process();
	void render() noexcept;  
	void SplitAsteroid(Asteroid * asteroid); //splitself and destroy self
	void killobject(); 
	std::list<gameojbect*> m_gameobjectlist;
	spaceship* m_Globalspaceship;
};

constexpr int MAXAteroidCount = 20;
constexpr float AsteroidInterval = 9.3f;
constexpr float AsteroidSpeed = 0.02f;
 

class asteroidlauncher {
public:
	asteroidlauncher() noexcept {
		m_lastshoottime = 0;
		m_DestroyedAsteroid = 0;
	}
	float m_lastshoottime;
	int  m_DestroyedAsteroid;
	void shootasteroid(objectpool & basepool);
};

constexpr int bulletmanagermaxbulletcount = 10;
constexpr float bulletinterval = 2.5f;
constexpr float bulletspeed = 0.5f;

class bulletmanager {	
public:
	bulletmanager() noexcept {
		lastbullettime = 0;
	}
	void firebullet(objectpool& basepool, const spaceship& ship) { 
		if(g_Timemanger.Getsec() - lastbullettime < bulletinterval)
			return;
		lastbullettime = g_Timemanger.Getsec();
		bullet *abullet = new bullet(); 
		abullet->pos = ship.pos;
		abullet->dir = ship.dir; 
		abullet->speed = bulletspeed; 
		basepool.m_gameobjectlist.push_back(abullet);
	}
	
	float lastbullettime;
};

class bombmanager {
public:
	bombmanager() noexcept {}
	void showBombAsteroid(objectpool& basepool, vec2 pos, float radius, float rColour, float gColour, float bColour) {
		bomb *abomb = new bomb();
		abomb->pos = pos;
		abomb->radius = radius;
		abomb->rColour = rColour;
		abomb->gColour = gColour;
		abomb->bColour = bColour;
		basepool.m_gameobjectlist.push_back(abomb);
	}
	void showBombSpaceship(objectpool& basepool, vec2 pos, float radius) {
		bomb* abomb = new bomb();
		abomb->pos = pos;
		abomb->radius = radius;
		abomb->rColour = 1;
		abomb->gColour = 0;
		abomb->bColour = 0;
		basepool.m_gameobjectlist.push_back(abomb);
	}
};

constexpr float ArenaXPercentage = 0.7f;
constexpr float ArenaYPercentage = 0.9f;
constexpr float ArenaTooclosedistance = 10.2f;
constexpr float ArenaGameOverdistance = 1.0f;

class ArenaManager {
public:
	void Process();
	void Render();
};

// restart entire game, reset all array and values
void RestartGame(); 