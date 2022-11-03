#include "gameobject.h"

timeManager g_TimeManger;
arenaManager g_ArenaManager;
asteroidLauncher g_AsteroidLauncher;
objectPool g_gameObjectPool;
bulletManager g_bulletManager;
explosionManager g_explosionManager;
droneManager g_droneManager;
missileManager g_missileManager;
GAMESTATE g_gameState = FIRST_START;

// Arena's leftTop and rightBottom position
vec2f g_arenaPosition[2];

bool gameObject::checkcollision(const gameObject& other) {
	float distance = pos.distance(other.pos);
	if (distance <= radius + other.radius)
		return true; // collided 
	else
		return false;
}

asteroid::~asteroid() {}

void asteroid::generateAsteroidOutline(float max, float min) {
	// add vertex every 30 degree
	float count = 12.0f;
	for (float i = 0; i < count; i++) {
		float apie = PI * 2.0f / count;
		const float x = cosf(apie * i);
		const float y = sinf(apie * i);
		vec2f outLineVector(x, y);
		outLineVector = outLineVector.normalize();

		float randnum = GetRandom(max - min);
		outLineVector = ((outLineVector * min) + randnum);
		m_listOfOutlinePoints.push_back(outLineVector);
	}
	radius = (max + min) / 2.0f;
}

void asteroid::render() noexcept {
	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	float randNum = GetRandom(0.1f);
	float rotation;

	// decide clockwise or anticlockwise
	if (rotate < 0.5)
		rotation = (g_TimeManger.Getsec() * -1.0f) - (createdtime + randNum);
	else
		rotation = g_TimeManger.Getsec() - (createdtime + randNum);

	glRotatef(rotation * 5.0f, 0, 0, -1);

	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glColor3f(rColour, gColour, bColour);
	for (int i = 1; i < m_listOfOutlinePoints.size(); i += 1) {
		glVertex2f(m_listOfOutlinePoints[i].x, m_listOfOutlinePoints[i].y);
		glVertex2f(m_listOfOutlinePoints[i - 1].x, m_listOfOutlinePoints[i - 1].y);
	}
	glVertex2f(m_listOfOutlinePoints.back().x, m_listOfOutlinePoints.back().y);
	glVertex2f(m_listOfOutlinePoints[0].x, m_listOfOutlinePoints[0].y);
	glEnd();

	glLineWidth(1.0f);
	glPopMatrix();

	// show cracks which means still have energy
	if (objectHasCrack) {
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glColor3f(rColour, gColour, bColour);
		float numOfCracks = GetRandom(5) + 5;
		for (float i = 0; i < numOfCracks; i++) {
			float apie = PI * 2.0f / numOfCracks;
			const float x = cosf(apie * i) * GetRandom(radius) * 0.6f;
			const float y = sinf(apie * i) * GetRandom(radius) * 0.6f;
			glVertex2f(pos.x, pos.y);
			glVertex2f(pos.x + x, pos.y + y);
		}
		glEnd();
	}
}

bool objectPool::asteroidInArena(asteroid* asteroid) noexcept {
	float halfArenaX = xwidthofspace * ArenaXPercentage * 0.5f;
	float halfArenaY = yheightofspace * ArenaYPercentage * 0.5f;
	float distanceOffset;

	if (asteroid->objectInArena == false)
		distanceOffset = 1.0f;
	else
		distanceOffset = -1.0f;

	if ((asteroid->pos.x > (-halfArenaX + asteroid->radius + distanceOffset))
		&& (asteroid->pos.x < (halfArenaX - asteroid->radius - distanceOffset))
		&& (asteroid->pos.y > (-halfArenaY + asteroid->radius + distanceOffset))
		&& (asteroid->pos.y < (halfArenaY - asteroid->radius - distanceOffset)))
		return true;
	return false;
}

spaceship::~spaceship() {}

void spaceship::render() noexcept {
	vec2f body1(-0.75f, -1.5f), body2(-0.75f, 0.75f), body3(0.75f, 0.75f), body4(0.75f, -1.5f);
	vec2f gun1(-0.15f, 0.5f), gun2(-0.15f, 1.5f), gun3(0.15f, 1.5f), gun4(0.15f, 0.5f);
	vec2f missile1(-0.5f, 0.5f), missile2(-0.25f, 1.5f), missile3(0.25f, 1.5f), missile4(0.5f, 0.5f);
	vec2f wing1_1(-0.75f, -1.5f), wing1_2(-0.75f, 0.75f), wing1_3(-1.5f, -1.75f);
	vec2f wing2_1(0.75f, -1.5f), wing2_2(0.75f, 0.75f), wing2_3(1.5f, -1.75f);
	vec2f fire_1(-0.75f, -1.5f), fire_2(-0.375f, -2.5f), fire_3(0, -2.0f), fire_4(0.375f, -2.5f), fire_5(0.75f, -1.5f);

	vec2f body[] = {body1, body2, body3, body4};
	vec2f gun[] = {gun1, gun2, gun3, gun4};
	vec2f missile[] = {missile1, missile2, missile3, missile4};
	vec2f wing1[] = {wing1_1, wing1_2, wing1_3};
	vec2f wing2[] = {wing2_1, wing2_2, wing2_3};
	vec2f fire[] = {fire_1, fire_2, fire_3, fire_4, fire_5};

	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	glRotatef(rotate * 180.0f / PI, 0, 0, 1);
	glLineWidth(1.0f);

	// draw body
	glBegin(GL_QUADS);
	glColor3f(0.976f, 0.325f, 0.203f);
	for (int i = 0; i < sizeof(body) / sizeof(vec2f); i++) {
		glVertex2f(body[i].x, body[i].y);
	}
	glEnd();

	// draw body outlines
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 1, 1);
	for (int i = 0; i < sizeof(body) / sizeof(vec2f); i++) {
		glVertex2f(body[i].x, body[i].y);
	}
	glEnd();

	// decide bullet or missile
	if (g_missileManager.missileInUse == false) {
		// draw gun body
		glBegin(GL_QUADS);
		glColor3f(0.588f, 0.6f, 0.580f);
		for (int i = 0; i < sizeof(gun) / sizeof(vec2f); i++) {
			glVertex2f(gun[i].x, gun[i].y);
		}
		glEnd();

		// draw gun outlines
		glBegin(GL_LINE_LOOP);
		glColor3f(1, 1, 1);
		for (int i = 0; i < sizeof(gun) / sizeof(vec2f); i++) {
			glVertex2f(gun[i].x, gun[i].y);
		}
		glEnd();
	}
	else {
		// draw missile body
		glBegin(GL_QUADS);
		glColor3f(0, 0, 0);
		for (int i = 0; i < sizeof(missile) / sizeof(vec2f); i++) {
			glVertex2f(missile[i].x, missile[i].y);
		}
		glEnd();

		// draw missile outlines
		glBegin(GL_LINE_LOOP);
		glColor3f(1, 1, 1);
		for (int i = 0; i < sizeof(missile) / sizeof(vec2f); i++) {
			glVertex2f(missile[i].x, missile[i].y);
		}
		glEnd();
	}

	// draw left wing body
	glBegin(GL_TRIANGLES);
	glColor3f(0.811f, 0.133f, 0.007f);
	for (int i = 0; i < sizeof(wing1) / sizeof(vec2f); i++) {
		glVertex2f(wing1[i].x, wing1[i].y);
	}
	glEnd();

	// draw left wing outlines
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 1, 1);
	for (int i = 0; i < sizeof(wing1) / sizeof(vec2f); i++) {
		glVertex2f(wing1[i].x, wing1[i].y);
	}
	glEnd();

	// draw right wing
	glBegin(GL_TRIANGLES);
	glColor3f(0.811f, 0.133f, 0.007f);
	for (int i = 0; i < sizeof(wing2) / sizeof(vec2f); i++) {
		glVertex2f(wing2[i].x, wing2[i].y);
	}
	glEnd();

	// draw right wing outlines
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 1, 1);
	for (int i = 0; i < sizeof(wing2) / sizeof(vec2f); i++) {
		glVertex2f(wing2[i].x, wing2[i].y);
	}
	glEnd();

	// draw fire when it moves forward
	if (speed > 0) {
		glBegin(GL_POLYGON);
		glColor3f(0.960f, 0.956f, 0);
		for (int i = 0; i < sizeof(fire) / sizeof(vec2f); i++) {
			glVertex2f(fire[i].x, fire[i].y);
		}
		glEnd();

		glLineWidth(2.0f);
		// draw fire outlines
		glBegin(GL_LINE_LOOP);
		glColor3f(1, 0, 0);
		for (int i = 0; i < sizeof(fire) / sizeof(vec2f); i++) {
			glVertex2f(fire[i].x, fire[i].y);
		}
		glEnd();
	}

	glColor3f(1, 1, 1);
	glLineWidth(1.0f);
	glPopMatrix();
}

void spaceship::process() {
	// if gameover, move spaceship to out of Arena to prevent more collisions
	if (g_gameState == GAME_OVER)
		g_gameObjectPool.m_GlobalSpaceship->pos = vec2f(RAND_MAX, RAND_MAX);
	dir.x = cosf(rotate + PI / 2.0f);
	dir.y = sinf(rotate + PI / 2.0f);
	pos = (pos + (dir * speed));

	speed -= spaceshipNaturalDeAccel * 0.03f;
	if (speed < 0)
		speed = 0;
}

void objectPool::render() noexcept {
	for (std::list<gameObject*>::iterator it = m_gameObjectList.begin(); it != m_gameObjectList.end(); it++) 
		(*it)->render();
}

void objectPool::process() {
	for (std::list<gameObject*>::iterator it = m_gameObjectList.begin(); it != m_gameObjectList.end(); it++) {
		(*it)->process();
		float halfArenaX = xwidthofspace * ArenaXPercentage * 0.5f;
		float halfArenaY = yheightofspace * ArenaYPercentage * 0.5f;

		for (std::list<gameObject*>::iterator it2 = m_gameObjectList.begin(); it2 != m_gameObjectList.end(); it2++) {
			// collision between Asteroid and Arena
			if ((*it2)->type == ASTEROID) {
				// check if Asteroid get into the Arena
				if ((*it2)->objectInArena == false) {
					if (asteroidInArena((asteroid*)(*it2)))
						(*it2)->objectInArena = true;
				}
				// collision with one of the walls after Asteroid get into the Arena
				else {
					// once asteroid get into Arena, then it cannot go out of Arena, so consider it as a bug 
					if (asteroidInArena((asteroid*)(*it2)) == false)
						(*it2)->duration = -1;

					float distanceOffset = 1.3f;
					vec2f posCurrent = (*it2)->pos;
					float radius = (*it2)->radius;

					// left line of Arena
					if (abs(posCurrent.x - g_arenaPosition[0].x) <= radius) {
						vec2f reverseVec = vec2f((*it2)->dir.x * -1.0f, (*it2)->dir.y);
						reverseVec = reverseVec.normalize();
						(*it2)->pos = posCurrent + reverseVec * distanceOffset;
						(*it2)->dir = reverseVec;
					}

					// top line of Arena
					if (abs(posCurrent.y - g_arenaPosition[0].y) <= radius) {
						vec2f reverseVec = vec2f((*it2)->dir.x, (*it2)->dir.y * -1.0f);
						reverseVec = reverseVec.normalize();
						(*it2)->pos = posCurrent + reverseVec * distanceOffset;
						(*it2)->dir = reverseVec;
					}

					// right line of Arena
					if (abs(posCurrent.x - g_arenaPosition[1].x) <= radius) {
						vec2f reverseVec = vec2f((*it2)->dir.x * -1.0f, (*it2)->dir.y);
						reverseVec = reverseVec.normalize();
						(*it2)->pos = posCurrent + reverseVec * distanceOffset;
						(*it2)->dir = reverseVec;
					}

					// bottom line of Arena
					if (abs(posCurrent.y - g_arenaPosition[1].y) <= radius) {
						vec2f reverseVec = vec2f((*it2)->dir.x, (*it2)->dir.y * -1.0);
						reverseVec = reverseVec.normalize();
						(*it2)->pos = posCurrent + reverseVec * distanceOffset;
						(*it2)->dir = reverseVec;
					}
				}
			}
			// collision between Bullet and Arena
			if ((*it2)->type == BULLET || (*it2)->type == MISSILE) {
				// collision with wall after Bullet is launched
				if ((abs(-halfArenaX - ((*it2)->pos.x + (*it2)->radius)) <= ArenaGameOverdistance)
					|| (abs(halfArenaX - ((*it2)->pos.x + (*it2)->radius)) <= ArenaGameOverdistance))
					(*it2)->duration = -1;
				else if (((abs(-halfArenaY - ((*it2)->pos.y + (*it2)->radius)) <= ArenaGameOverdistance)
					|| (abs(halfArenaY - ((*it2)->pos.y + (*it2)->radius)) <= ArenaGameOverdistance)))
					(*it2)->duration = -1;
			}

			if ((*it)->checkcollision(**it2)) {
				// collision between Asteroid and Spaceship 
				if (((*it)->type == ASTEROID && (*it2)->type == SPACESHIP) || ((*it)->type == SPACESHIP && (*it2)->type == ASTEROID)) {
					killAfterCollision(*it, *it2);

					// SHIP COLLIDE WITH ASTEROID . WE CAN MAKE HEALTH POINT HERE OR JUST GO TO GAME OVER
					g_gameState = GAME_OVER;
				}
				// collision between Asteroid and Bullet or Missile
				else if (((*it)->type == ASTEROID && (*it2)->type == BULLET)
					|| ((*it)->type == BULLET && (*it2)->type == ASTEROID)
					|| ((*it)->type == ASTEROID && (*it2)->type == MISSILE)
					|| ((*it)->type == MISSILE && (*it2)->type == ASTEROID)) {
					killAfterCollision(*it, *it2);

					g_AsteroidLauncher.m_DestroyedAsteroid++;
				}
				// collision between 2 Asteroids in Arena
				else if ((*it)->type == ASTEROID && ((*it)->type == (*it2)->type)
					&& ((*it)->objectInArena == true) && ((*it2)->objectInArena == true)) {
					vec2f firstAsteroidDir = (*it)->dir;
					vec2f firstAsteroidPos = (*it)->pos;
					vec2f secondAsteroidDir = (*it2)->dir;
					vec2f secondAsteroidPos = (*it2)->pos;

					// calculate first Asteroid's dir
					(*it)->dir = ((firstAsteroidPos - secondAsteroidPos).normalize() + firstAsteroidDir).normalize();
					// Calculate second Asteroid's dir
					(*it2)->dir = ((secondAsteroidPos - firstAsteroidPos).normalize() + secondAsteroidDir).normalize();
				}
				// collision between Asteroid and Drone in Arena
				else if (((*it)->type == ASTEROID && (*it2)->type == DRONE)
					|| ((*it)->type == DRONE && (*it2)->type == ASTEROID)) 
					killAfterCollision(*it, *it2);
			}
		}

		// kill objects which have 0 or less than 0 energy, or 0 or less than 0 duration except spaceship
		if ((*it)->NeedtoKillthis(g_droneManager)) {
			if ((*it)->type == DRONE) {
				// launch one more set of bullets before get killed
				g_droneManager.droneFireBullet(g_gameObjectPool, (*it)->pos, g_gameObjectPool.m_GlobalSpaceship->radius);
				g_droneManager.droneInArena -= 1;
			}
			delete* it;
			it = m_gameObjectList.erase(it);
			--it;
		}
	}
}

void objectPool::killAfterCollision(gameObject* gameObject1, gameObject* gameObject2) noexcept {
	// ASTEROID with SPACESHIP -> just kill Asteroid
	if (gameObject1->type == ASTEROID && gameObject2->type == SPACESHIP) {
		g_explosionManager.showExplosionSpaceship(g_gameObjectPool, gameObject2->pos, gameObject2->radius);
		g_explosionManager.showExplosionAsteroid(g_gameObjectPool, gameObject1);
		gameObject1->duration = -1;
	}
	// ASTEROID with SPACESHIP -> just kill Asteroid
	else if (gameObject1->type == SPACESHIP && gameObject2->type == ASTEROID) {
		g_explosionManager.showExplosionSpaceship(g_gameObjectPool, gameObject1->pos, gameObject1->radius);
		g_explosionManager.showExplosionAsteroid(g_gameObjectPool, gameObject2);
		gameObject2->duration = -1;
	}
	// ASTEROID with BULLET -> kill both but still calculate energy of Asteroid
	else if (gameObject1->type == ASTEROID && gameObject2->type == BULLET) {
		gameObject1->energy -= 1;
		gameObject2->duration = -1;
		if (gameObject1->energy <= 0 && gameObject1->objectHasCrack == false)
			g_explosionManager.showExplosionAsteroid(g_gameObjectPool, gameObject1);
		// Asteroid has crack and energy is exhausted, then split it
		if (gameObject1->energy <= 0 && gameObject1->objectHasCrack == true) {
			SplitAsteroid(gameObject1);
			gameObject1->duration = -1;
		}
		if (gameObject1->energy > 0) {
			gameObject1->objectHasCrack = true;
		}
	}
	// ASTEROID with BULLET -> kill both but still calculate energy of Asteroid
	else if (gameObject1->type == BULLET && gameObject2->type == ASTEROID) {
		gameObject2->energy -= 1;
		gameObject1->duration = -1;
		if (gameObject2->energy <= 0 && gameObject2->objectHasCrack == false)
			g_explosionManager.showExplosionAsteroid(g_gameObjectPool, gameObject2);
		// Asteroid has crack and energy is exhausted, then split it
		if (gameObject2->energy <= 0 && gameObject2->objectHasCrack == true) {
			SplitAsteroid(gameObject2);
			gameObject2->duration = -1;
		}
		if (gameObject2->energy > 0) {
			gameObject2->objectHasCrack = true;
		}
	}
	// ASTEROID with MISSILE -> kill both
	else if (gameObject1->type == ASTEROID && gameObject2->type == MISSILE) {
		g_explosionManager.showExplosionAsteroid(g_gameObjectPool, gameObject1);
		// missile do not split the Asteroid but just kill
		gameObject1->energy -= 2;
		gameObject2->duration = -1;
	}
	// ASTEROID with MISSILE -> kill both
	else if (gameObject1->type == MISSILE && gameObject2->type == ASTEROID) {
		g_explosionManager.showExplosionAsteroid(g_gameObjectPool, gameObject2);
		gameObject2->energy -= 2;
		gameObject1->duration = -1;
	}
	// ASTEROID with DRONE -> kill Asteroid
	else if (gameObject1->type == ASTEROID && gameObject2->type == DRONE) {
		g_explosionManager.showExplosionAsteroid(g_gameObjectPool, gameObject1);
		gameObject1->duration -= 1;
	}
	// ASTEROID with DRONE -> kill Asteroid
	else if (gameObject1->type == DRONE && gameObject2->type == ASTEROID) {
		g_explosionManager.showExplosionAsteroid(g_gameObjectPool, gameObject2);
		gameObject2->duration = -1;
	}
}

void objectPool::SplitAsteroid(gameObject* gameObject) {
	asteroid* aNewHalfAsteroid1 = new asteroid;
	asteroid* aNewHalfAsteroid2 = new asteroid;

	// --- calculate first new Asteroid's pos and dir ---
	// position is original position (sin(@), -cos(@))
	vec2f originalAsteroidDir = gameObject->dir;
	vec2f newAsteroid1Pos = vec2f (gameObject->pos.y, gameObject->pos.x * -1.0f);
	// vector of 45 degree
	vec2f newAsteroid1Dir = (originalAsteroidDir.normalize() + newAsteroid1Pos.normalize()).normalize();

	aNewHalfAsteroid1->speed = gameObject->speed;
	aNewHalfAsteroid1->pos = gameObject->pos + (newAsteroid1Pos.normalize() * gameObject->radius * 2.0f);
	aNewHalfAsteroid1->dir = newAsteroid1Dir;
	aNewHalfAsteroid1->energy = 1;
	aNewHalfAsteroid1->rColour = gameObject->rColour;
	aNewHalfAsteroid1->gColour = gameObject->gColour;
	aNewHalfAsteroid1->bColour = gameObject->bColour;
	aNewHalfAsteroid1->objectInArena = true;
	aNewHalfAsteroid1->radius = gameObject->radius / 2.0f;
	aNewHalfAsteroid1->generateAsteroidOutline(aNewHalfAsteroid1->radius + Asteroidmaxradius, aNewHalfAsteroid1->radius + Asteroidminradius);
	m_gameObjectList.push_back(aNewHalfAsteroid1);

	// --- calculate second new Asteroid's pos and dir --- 
	// position is original position + (-sin(@), cos(@))
	vec2f newAsteroid2Pos = vec2f (gameObject->pos.y * -1.0f, gameObject->pos.x);
	// vector of 45 degree
	vec2f newAsteroid2Dir = (originalAsteroidDir.normalize() + newAsteroid2Pos.normalize()).normalize();

	aNewHalfAsteroid2->speed = gameObject->speed;
	aNewHalfAsteroid2->pos = gameObject->pos + (newAsteroid2Pos.normalize() * gameObject->radius * 2.0f);
	aNewHalfAsteroid2->dir = newAsteroid2Dir;
	aNewHalfAsteroid2->energy = 1;
	aNewHalfAsteroid2->rColour = gameObject->rColour;
	aNewHalfAsteroid2->gColour = gameObject->gColour;
	aNewHalfAsteroid2->bColour = gameObject->bColour;
	aNewHalfAsteroid2->objectInArena = true;
	aNewHalfAsteroid2->radius = gameObject->radius / 2.0f;
	aNewHalfAsteroid2->generateAsteroidOutline(aNewHalfAsteroid2->radius + Asteroidmaxradius, aNewHalfAsteroid2->radius + Asteroidminradius);
	m_gameObjectList.push_back(aNewHalfAsteroid2);
}

bullet::~bullet() {}

void bullet::render() noexcept {
	float bulletSize = radius * 20.0f;
	glPointSize(bulletSize);
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	glVertex2f(pos.x, pos.y);
	glEnd();
}

explosion::~explosion() {}

void explosion::render() noexcept {
	glBegin(GL_POINTS);
	glColor3f(rColour, gColour, bColour);
	glVertex2f(pos.x, pos.y);
	glColor3f(1, 1, 1);
	glEnd();
}

drone::~drone() {}

void drone::render() noexcept {
	constexpr float count = 6.0f;
	constexpr float propellerRadius = 1.2f;
	constexpr float propellerAngle = 0.001f;
	glLineWidth(5.0f);

	// draw drone body
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 1, 1);
	for (float i = 0; i < count; i++) {
		constexpr float apie = PI * 2.0f / count;
		const float x = cosf(apie * i) * radius;
		const float y = sinf(apie * i) * radius;
		glVertex2f(pos.x + x, pos.y + y);
	}
	glEnd();

	glLineWidth(2.5f);
	// draw drone wings
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	for (float i = 0; i < count; i++) {
		constexpr float apie = PI * 2.0f / count;
		const float x = cosf(apie * i) * radius;
		const float y = sinf(apie * i) * radius;
		glVertex2f(pos.x + x, pos.y + y);
		glVertex2f(pos.x + x * 1.3f, pos.y + y * 1.3f);
	}
	glEnd();

	// draw drone propellers
	glBegin(GL_LINES);
	glColor3f(GetRandom(1.0f), GetRandom(1.0f), GetRandom(1.0f));
	float randomPropellerAngle = GetRandom(propellerAngle);
	for (float i = 0; i < count; i++) {
		constexpr float aBodyPie = PI * 2.0f / count;
		const float x = cosf(aBodyPie * i) * radius;
		const float y = sinf(aBodyPie * i) * radius;

		float aPropellerPie = PI * 2.0f / randomPropellerAngle;
		const float wingX = cosf(aPropellerPie);
		const float wingY = sinf(aPropellerPie);
		glVertex2f(pos.x + x * 1.3f, pos.y + y * 1.3f);
		glVertex2f(pos.x + x * 1.3f + (propellerRadius * wingX), pos.y + y * 1.3f + (propellerRadius * wingY));
	}
	glColor3f(1, 1, 1);
	glEnd();
	glLineWidth(1.0f);
}

void missile::refreshMissileDirection() noexcept {
	for (std::list<gameObject*>::iterator it = g_gameObjectPool.m_gameObjectList.begin(); it != g_gameObjectPool.m_gameObjectList.end(); it++) {
		if (((*it)->type == MISSILE)) {
			vec2f nearestPos;
			float minDistance = FLT_MAX;
			int asteroidInArena = 0;
			vec2f missilePos = (*it)->pos;

			for (std::list<gameObject*>::iterator it2 = g_gameObjectPool.m_gameObjectList.begin(); it2 != g_gameObjectPool.m_gameObjectList.end(); it2++) {
				// check if there exists any Asteroid in Arena
				if (((*it2)->type == ASTEROID) && ((*it2)->objectInArena == true)) {
					vec2f asteroidPos = (*it2)->pos;
					float distance = missilePos.distance(asteroidPos);

					if (minDistance > distance) {
						minDistance = distance;
						nearestPos = asteroidPos;
						asteroidInArena++;
					}
				}
			}
			// find new vector from nearest asteroid's position
			vec2f missileNewDir = nearestPos - missilePos;
			// change the direction if only there is at least one Asteroid in Arena
			if (asteroidInArena > 0)
				(*it)->dir = missileNewDir.normalize();
			else
				(*it)->dir = g_gameObjectPool.m_GlobalSpaceship->dir;
		}
	}
}

void missile::render() noexcept {
	// refresh direction first
	refreshMissileDirection();

	// draw radioactivity sign
	glLineWidth(1.5f);
	glBegin(GL_POLYGON);
	glColor3f(0, 0, 0);
	constexpr float radioactivityCount = 4.0f;
	for (float i = 0; i < radioactivityCount; i++) {
		constexpr float apie = PI * 2.0f / radioactivityCount;
		const float x = cosf(apie * i) * radius;
		const float y = sinf(apie * i) * radius;
		glVertex2f(pos.x + x, pos.y + y);
	}
	glEnd();

	// draw missile outline
	glBegin(GL_LINE_LOOP);
	glColor3f(1, 1, 0);
	constexpr float count = 4.0f;
	for (float i = 0; i < count; i++) {
		constexpr float apie = PI * 2.0f / count;
		const float x = cosf(apie * i) * radius;
		const float y = sinf(apie * i) * radius;
		glVertex2f(pos.x + x, pos.y + y);
	}
	glEnd();

	glLineWidth(1.0f);
}

// There are 4 wave levels
int wave = 0;

void asteroidLauncher::shootAsteroid(objectPool& basepool) {
	int asteroidCount = 0;
	for (std::list<gameObject*>::iterator it = basepool.m_gameObjectList.begin(); it != basepool.m_gameObjectList.end(); it++) {
		if ((*it)->type == ASTEROID)
			if ((*it)->radius > Asteroidminradius)
				asteroidCount++;
	}

	const float timeAfterShoot = g_TimeManger.Getsec() - m_lastShootTime;
	// first wave launches 1 Asteroid, second wave launches 2 Asteroids, etc.
	const int waves[] = {1, 2, 3, 4};

	if (timeAfterShoot > asteroidInterval) {
		for (int i = 0; i < waves[wave]; i++) {
			if (asteroidCount < ateroidMaxCount) {
				// pick Random direction and shoot
				float Angle = GetRandom(2.0f * PI);
				float x = cosf(Angle);
				float y = sinf(Angle);

				// GetAsteroid Distance by Arena MAx size 
				float halfArenaX = xwidthofspace * ArenaXPercentage * 0.5f;
				float halfArenaY = yheightofspace * ArenaYPercentage * 0.5f;
				float RadiusofArena = sqrtf(halfArenaX * halfArenaX + halfArenaY * halfArenaY);

				RadiusofArena *= 1.1;

				vec2f startPos(x * RadiusofArena, y * RadiusofArena);
				vec2f dirToShip(g_gameObjectPool.m_GlobalSpaceship->pos.x - startPos.x, g_gameObjectPool.m_GlobalSpaceship->pos.y - startPos.y);
				dirToShip = dirToShip.normalize();

				asteroid* anewasteroid = new asteroid;

				anewasteroid->speed = asteroidMinSpeed + GetRandom(asteroidMaxSpeed - asteroidMinSpeed);
				anewasteroid->pos = startPos;
				anewasteroid->dir = dirToShip;
				anewasteroid->radius = GetRandom(AsteroidRandomRadius) + AsteroidBaseRadius;
				anewasteroid->rColour = GetRandom(1);
				anewasteroid->gColour = GetRandom(1);
				anewasteroid->bColour = GetRandom(1);
				// not to make too dark
				if (anewasteroid->rColour < 0.3)
					anewasteroid->rColour = 1 - anewasteroid->rColour;
				if (anewasteroid->gColour < 0.3)
					anewasteroid->gColour = 1 - anewasteroid->gColour;
				if (anewasteroid->bColour < 0.3)
					anewasteroid->bColour = 1 - anewasteroid->bColour;
				// if asteroid's radius is larger or eqault to 2.5f, then make its energy 2
				if (anewasteroid->radius > 2.5f)
					anewasteroid->energy = 2;
				anewasteroid->generateAsteroidOutline(Asteroidmaxradius + anewasteroid->radius, Asteroidminradius + anewasteroid->radius);
				basepool.m_gameObjectList.push_back(anewasteroid);
			}
		}
		m_lastShootTime = g_TimeManger.Getsec();
		wave += 1;
		if (wave == 4)
			wave = 3;
	}
}

void arenaManager::Process() {
	const float halfX = xwidthofspace * ArenaXPercentage * 0.5f;
	const float halfY = yheightofspace * ArenaYPercentage * 0.5f;
	//check distance with Left 
	const vec2f& shipos = g_gameObjectPool.m_GlobalSpaceship->pos;
	const float shipradius = g_gameObjectPool.m_GlobalSpaceship->radius;

	bool needtoRestart = false;

	if (abs(-halfX - (shipos.x + shipradius)) <= ArenaGameOverdistance)
		needtoRestart = true;

	if (abs(halfX - (shipos.x + shipradius)) <= ArenaGameOverdistance)
		needtoRestart = true;

	if (abs(-halfY - (shipos.y + shipradius)) <= ArenaGameOverdistance)
		needtoRestart = true;

	if (abs(halfY - (shipos.y + shipradius)) <= ArenaGameOverdistance)
		needtoRestart = true;

	if (needtoRestart || g_gameState == GAME_OVER) {
		g_explosionManager.showExplosionSpaceship(g_gameObjectPool, g_gameObjectPool.m_GlobalSpaceship->pos, g_gameObjectPool.m_GlobalSpaceship->radius);
		g_gameState = GAME_OVER;
	}
}

void arenaManager::Render() {
	const float screenX = xwidthofspace * 0.5f;
	const float screenY = yheightofspace * 0.5f;
	const float halfX = xwidthofspace * ArenaXPercentage * 0.5f;
	const float halfY = yheightofspace * ArenaYPercentage * 0.5f;

	// draw Arena body
	glBegin(GL_POLYGON);
	glColor3f(0.015f, 0.203f, 0.345f);
	glVertex2f(halfX, halfY);
	glVertex2f(halfX, -halfY);
	glVertex2f(-halfX, -halfY);
	glVertex2f(-halfX, halfY);
	glEnd();

	glLineWidth(3.0f);
	glBegin(GL_LINES);

	const vec2f& shipos = g_gameObjectPool.m_GlobalSpaceship->pos;
	const float shipradius = g_gameObjectPool.m_GlobalSpaceship->radius;

	// left
	if (abs(-halfX - (shipos.x + shipradius)) <= ArenaTooclosedistance)
		glColor3f(1, 0, 0);
	else
		glColor3f(1, 1, 1);
	glVertex2f(-halfX, halfY);
	glVertex2f(-halfX, -halfY);

	g_arenaPosition[0].x = -halfX;

	// right
	if (abs(halfX - (shipos.x + shipradius)) <= ArenaTooclosedistance)
		glColor3f(1, 0, 0);
	else
		glColor3f(1, 1, 1);
	glVertex2f(halfX, halfY);
	glVertex2f(halfX, -halfY);

	g_arenaPosition[1].x = halfX;

	// bottom 
	if (abs(-halfY - (shipos.y + shipradius)) <= ArenaTooclosedistance)
		glColor3f(1, 0, 0);
	else
		glColor3f(1, 1, 1);
	glVertex2f(-halfX, -halfY);
	glVertex2f(halfX, -halfY);

	g_arenaPosition[1].y = -halfY;

	// top
	if (abs(halfY - (shipos.y + shipradius)) <= ArenaTooclosedistance)
		glColor3f(1, 0, 0);
	else
		glColor3f(1, 1, 1);
	glVertex2f(-halfX, halfY);
	glVertex2f(halfX, halfY);

	g_arenaPosition[0].y = halfY;

	glColor3f(1, 1, 1);
	glEnd();

	glLineWidth(1.0f);
}

void RestartGame() {
	// delete all the objects
	for (std::list<gameObject*>::iterator it = g_gameObjectPool.m_gameObjectList.begin(); it != g_gameObjectPool.m_gameObjectList.end(); it++)
		delete* it;

	g_gameObjectPool.m_gameObjectList.clear();

	// make a new spaceship
	spaceship* s = new spaceship();
	g_gameObjectPool.m_gameObjectList.push_back(s);
	s->speed = 0;
	g_gameObjectPool.m_GlobalSpaceship = s;
	g_AsteroidLauncher.m_DestroyedAsteroid = 0;
	g_TimeManger.SetStartTime();

	// reset drones
	g_droneManager.howManyDronesSpawned = 0;
	g_droneManager.droneInArena = 0;

	// reset wave
	wave = 0;
}