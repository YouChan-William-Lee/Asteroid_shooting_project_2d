#pragma once
#include "Unifiedheader.h"
#include "time.h"
#include "vectormath.h"

//extern int g_gameScore;
class timeManager;
class arenaManager;
class asteroidLauncher;
class objectPool;
class bulletManager;
class explosionManager;
class droneManager;
class missileManager;

// extern bool g_needtoSetgameOver;
extern timeManager g_TimeManger;
extern arenaManager g_ArenaManager;
extern asteroidLauncher g_AsteroidLauncher;
extern objectPool g_gameObjectPool;
extern bulletManager g_bulletManager;
extern explosionManager g_explosionManager;
extern droneManager g_droneManager;
extern missileManager g_missileManager;

// game over -1, game first start 0, game playing 1
enum GAMESTATE {
	GAME_OVER = -1,
	FIRST_START = 0,
	GAME_PLAYING = 1
};

extern GAMESTATE g_gameState;

enum GAMEOBJECTTYPE {
	ERROR_DEFAULTOBJ = 0,
	ASTEROID = 1,
	BULLET = 2,
	SPACESHIP = 3,
	EXPLOSION = 4,
	DRONE = 5,
	MISSILE = 6
};

class timeManager {
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
		return m_Currenttime * 0.01f;
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

class gameObject {
public:
	GAMEOBJECTTYPE type;
	vec2f pos;
	vec2f dir;
	float speed;
	float radius;
	float rotate;
	float createdtime;
	float duration;
	float rColour;
	float gColour;
	float bColour;
	int energy;
	bool objectInArena;
	bool objectHasCrack;

	gameObject() noexcept {
		type = ERROR_DEFAULTOBJ;
		speed = 0;
		radius = 2;
		pos.x = 0;
		pos.y = 0;
		dir.x = 0;
		dir.y = 1;
		rotate = 0;
		duration = 20.0f;
		rColour = 0;
		gColour = 0;
		bColour = 0;
		energy = 1;
		createdtime = g_TimeManger.Getsec();
		objectInArena = true;
		objectHasCrack = false;
	}

	virtual bool NeedtoKillthis(droneManager& droneManager) noexcept {
		if (type == SPACESHIP) 
			return false;

		if (energy <= 0)
			return true;

		if (g_TimeManger.Getsec() - createdtime >= duration) 
			return true;
		else
			return false;
	}

	virtual	void process() {
		pos = pos + (dir * speed);
	}
	virtual	void render() noexcept {};
	virtual bool checkcollision(const gameObject& other);
};

constexpr float Asteroidmaxradius = 0.8f;
constexpr float Asteroidminradius = -0.3f;
constexpr float AsteroidBaseRadius = 2.0f;
constexpr float AsteroidRandomRadius = 1.5f;
constexpr float AsteroidDuration = 1000.0f;

class asteroid : public gameObject {
public:
	asteroid() noexcept {
		type = ASTEROID;
		rotate = GetRandom(1.0);
		duration = AsteroidDuration;
		objectInArena = false;
	};
	~asteroid();
	void generateAsteroidOutline(const float max, const float min);
	vector<vec2f> m_listOfOutlinePoints;

	virtual void render() noexcept;
};

constexpr float spaceshipMaxSpeed = 0.15f;
constexpr float spaceshipRotateSpeed = 0.3f;
constexpr float spaceshipAccel = 0.02f;
constexpr float spaceshipNaturalDeAccel = 0.01f;
constexpr float spaceshipDuration = -1.0f;

class spaceship : public gameObject {
public:
	spaceship() noexcept {
		type = SPACESHIP;
		duration = spaceshipDuration;
	}
	~spaceship();
	virtual void render() noexcept;
	virtual void process();

	vector<vec2f> m_listofoutlinepoints;
};

constexpr float bulletDuration = 30.0f;
constexpr float bulletRadius = 0.25;

class bullet : public gameObject {
public:
	bullet() {
		type = BULLET;
		duration = bulletDuration;
		radius = bulletRadius;
	};
	~bullet();
	virtual void render() noexcept;
};

constexpr float explosionDuration = 5.0f;
constexpr float explosionRadius = 3.0f;
constexpr float explosionSpeed = 0.01f;

class explosion : public gameObject {
public:
	explosion() {
		type = EXPLOSION;
		radius = explosionRadius;
		duration = explosionDuration;
		speed = explosionSpeed;
	}
	~explosion();
	virtual void render() noexcept;
};

constexpr float droneDuration = 20.0f;
constexpr float droneRadius = 3.0f;

class drone : public gameObject {
public:
	drone() {
		type = DRONE;
		duration = droneDuration;
		dir = vec2f(0, 0);
		radius = droneRadius;
	}
	~drone();
	virtual void render() noexcept;
};

constexpr float missileDuration = 30.0f;
constexpr float missileRadius = 0.45;

class missile : public gameObject {
public:
	missile() {
		type = MISSILE;
		duration = missileDuration;
		radius = missileRadius;
	}
	virtual void render() noexcept;
	void refreshMissileDirection() noexcept;
};

class objectPool {
public:
	objectPool() noexcept {
		m_GlobalSpaceship = 0;
	}
	void process();
	void render() noexcept;
	void SplitAsteroid(gameObject* gameObject);
	bool asteroidInArena(asteroid* asteroid) noexcept;
	void killAfterCollision(gameObject* gameObject1, gameObject* gameObject2) noexcept;
	std::list<gameObject*> m_gameObjectList;
	spaceship* m_GlobalSpaceship;
};

constexpr int ateroidMaxCount = 25;
constexpr float asteroidInterval = 30.0f;
constexpr float asteroidMinSpeed = 0.01f;
constexpr float asteroidMaxSpeed = 0.025f;

class asteroidLauncher {
public:
	asteroidLauncher() noexcept {
		m_lastShootTime = 0;
		m_DestroyedAsteroid = 0;
	}
	float m_lastShootTime;
	int  m_DestroyedAsteroid;
	void shootAsteroid(objectPool& basepool);
};

constexpr int bulletManagerMaxBulletCount = 10;
constexpr float bulletInterval = 1.5f;
constexpr float bulletSpeed = 0.15f;

class bulletManager {
public:
	bulletManager() noexcept {
		lastBulletTime = 0;
	}
	void spaceshipFireBullet(objectPool& basepool, const spaceship& ship) {
		if (g_TimeManger.Getsec() - lastBulletTime < bulletInterval)
			return;
		lastBulletTime = g_TimeManger.Getsec();
		bullet* abullet = new bullet();
		vec2f shipDir = ship.dir;
		// shot from the tip of the gun
		abullet->pos = shipDir.normalize() * 2.0f + ship.pos;
		abullet->dir = ship.dir;
		abullet->speed = bulletSpeed;
		basepool.m_gameObjectList.push_back(abullet);
	}
	float lastBulletTime;
};

class explosionManager {
public:
	explosionManager() noexcept {}
	void showExplosionAsteroid(objectPool& basepool, gameObject* gameObject) {
		float randNum = GetRandom(10) + 10;
		for (float i = 0; i < randNum; i++) {
			constexpr float count = 12.0f;
			float apie = PI * 2.0f / GetRandom(count);
			const float x = cosf(apie);
			const float y = sinf(apie);

			explosion* aexplosion = new explosion();
			aexplosion->pos = gameObject->pos;
			aexplosion->dir = vec2f(x, y);
			aexplosion->speed = aexplosion->speed + GetRandom(explosionSpeed);
			aexplosion->radius = gameObject->radius;
			aexplosion->rColour = gameObject->rColour;
			aexplosion->gColour = gameObject->gColour;
			aexplosion->bColour = gameObject->bColour;
			basepool.m_gameObjectList.push_back(aexplosion);
		}
	}
	void showExplosionSpaceship(objectPool& basepool, vec2f pos, float radius) {
		float randNum = GetRandom(10) + 10;
		for (float i = 0; i < randNum; i++) {
			constexpr float count = 12.0f;
			float apie = PI * 2.0f / GetRandom(count);
			const float x = cosf(apie);
			const float y = sinf(apie);

			explosion* aexplosion = new explosion();
			aexplosion->pos = pos;
			aexplosion->dir = vec2f(x, y);
			aexplosion->speed = aexplosion->speed + GetRandom(explosionSpeed);
			aexplosion->radius = radius;
			// Make a red bomb
			aexplosion->rColour = 1;
			aexplosion->gColour = 0;
			aexplosion->bColour = 0;
			basepool.m_gameObjectList.push_back(aexplosion);
		}
	}
};

constexpr float droneTimeInterval = 2.0f;
constexpr int maxDroneNumberAtOnce = 2;

class droneManager {
public:
	droneManager() noexcept {
		lastDroneTime = 0;
		droneInArena = 0;
		howManyDronesSpawned = 0;
	}
	void spawnDrone(objectPool& basepool, const spaceship& ship) {
		if (g_TimeManger.Getsec() - lastDroneTime < droneTimeInterval)
			return;
		lastDroneTime = g_TimeManger.Getsec();
		drone* adrone = new drone();
		adrone->pos = ship.pos;
		basepool.m_gameObjectList.push_back(adrone);

		g_droneManager.droneInArena += 1;
		g_droneManager.howManyDronesSpawned += 1;

		droneFireBullet(basepool, ship.pos, ship.radius);
	}
	void droneFireBullet(objectPool& basepool, const vec2f pos, float radius) {
		constexpr float count = 8.0f;
		// fire bullet from all edges of the drone
		for (float j = 0; j < count; j++) {
			constexpr float apie = PI * 2.0f / count;
			const float x = cosf(apie * j) * radius * 2;
			const float y = sinf(apie * j) * radius * 2;
			bullet* abullet = new bullet();
			abullet->pos = vec2f(pos.x + x, pos.y + y);
			abullet->dir = vec2f(x, y).normalize();
			abullet->speed = bulletSpeed;
			basepool.m_gameObjectList.push_back(abullet);
		}
	}
	float lastDroneTime;
	int droneInArena;
	int howManyDronesSpawned;
};

constexpr int MissileManagerMaxMissileCount = 10;
constexpr float MissileInterval = 1.5f;
constexpr float MissileSpeed = 0.15f;

class missileManager {
public:
	missileManager() noexcept {
		missileInUse = false;
		lastMissileTime = 0;
	}
	void toggleMissile(objectPool& basepool, spaceship& ship) {
		if (missileInUse == false)
			missileInUse = true;
		else
			missileInUse = false;
	}
	void spaceshipFireMissile(objectPool& basepool, const spaceship& ship) {
		if (g_TimeManger.Getsec() - lastMissileTime < bulletInterval)
			return;
		lastMissileTime = g_TimeManger.Getsec();
		missile* amissile = new missile();
		vec2f shipDir = ship.dir;
		// shot from the tip of the missile gun
		amissile->pos = shipDir.normalize() * 2.0f + ship.pos;
		amissile->dir = ship.dir;
		amissile->speed = MissileSpeed;
		basepool.m_gameObjectList.push_back(amissile);
	}
	bool missileInUse;
	float lastMissileTime;
};

constexpr float ArenaXPercentage = 0.7f;
constexpr float ArenaYPercentage = 0.9f;
constexpr float ArenaTooclosedistance = 10.2f;
constexpr float ArenaGameOverdistance = 1.0f;

class arenaManager {
public:
	void Process();
	void Render();
};

// restart entire game, reset all array and values
void RestartGame();