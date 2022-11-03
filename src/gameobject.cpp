#include "gameobject.h"
#include <typeinfo> 

timemanager g_Timemanger; 
ArenaManager g_Arenamanager;
asteroidlauncher g_AsteroidLauncher;
objectpool g_gameobjectpool;
bulletmanager g_bulletmanager;
bombmanager g_bombmanager;
GAMESTATE g_gameState = FIRST_START;

bool gameojbect::checkcollision(const gameojbect& other) {
	vec2 distanceV =  other.pos - pos;
	float distance = length(distanceV) ;
	if (distance <= radius + other.radius)
		return true; // collided 
	else
		return false; 
 }

Asteroid::Asteroid(void) noexcept {  
	type = ASTEROID; 
	duration = 700.0f; 
	inArena = false;
}


Asteroid::~Asteroid(void) {
}
 
void Asteroid::generateAsteroidOutline(float max, float min) {
	// add vertex every 15 degree
	for (float i = 0; i < 16.0f; i++) {
		constexpr float apie = 3.145f * 2.0f / 16.0f;

		const float x = cosf(apie * i);
		const float y = sinf(apie * i);
		vec2 unit(x,y);

		float randnum = GetRandom(max - min);
		unit = unit * min + randnum;
		m_listofoutlinepoints.push_back(unit);
	}

	radius = (max + min) / 2.0f;
}

void Asteroid::render() noexcept {
	glPushMatrix();
	glTranslatef(pos.x,pos.y, 0);

	float arotate = g_Timemanger.Getsec() - createdtime;; 
	glRotatef(arotate * 5.0f, 0, 0, -1);

	// draw outlines
	glLineWidth(3.0f);
	glBegin(GL_LINES);
	glColor3f(rColour, gColour, bColour);

	for (int i = 1; i < m_listofoutlinepoints.size( ); i += 1) { 
		glVertex2f(m_listofoutlinepoints[i].x, m_listofoutlinepoints[i].y);
		glVertex2f(m_listofoutlinepoints[i - 1].x, m_listofoutlinepoints[i - 1].y);
 	}
	glVertex2f(m_listofoutlinepoints.back( ).x, m_listofoutlinepoints.back( ).y);
	glVertex2f(m_listofoutlinepoints[0].x, m_listofoutlinepoints[0].y);

	glEnd();

	glLineWidth(1.0f);
	glPopMatrix();
} 

vec2 body[] = {vec2(-0.75f, -1.5), vec2(-0.75f, 0.75f), vec2(0.75f, 0.75f), vec2(0.75f, -1.5)};
vec2 gun[] = {vec2(-0.15f, 0.75f), vec2(-0.15f, 1.5f), vec2(0.15f, 1.5f), vec2(0.15f, 0.75f)};
vec2 wing1[] = {vec2(-0.75f, -1.5f), vec2(-0.75f, 0.75f), vec2(-1.5f, -1.5f)};
vec2 wing2[] = {vec2(0.75f, -1.5f), vec2(0.75f, 0.75f), vec2(1.5f, -1.5f)};

void spaceship::render() noexcept {   
	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	glRotatef(rotate * 180.0f / PI, 0, 0, 1); 

	// draw body
	glLineWidth(1.0f);
	glBegin(GL_QUADS);
	glColor3f(0, 0.5, 1);
	for (int i = 0; i < sizeof(body) / sizeof(vec2); i++) {
		glVertex2f(body[i].x, body[i].y);
	}
	glEnd();

	// draw body outlines
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	for (int i = 0; i < sizeof(body) / sizeof(vec2); i++) {
		glVertex2f(body[i].x, body[i].y);
	}
	glEnd();

	// draw gun body
	glLineWidth(1.0f);
	glBegin(GL_QUADS);
	glColor3f(0.5f, 0.8f, 0.8f);
	for (int i = 0; i < sizeof(gun) / sizeof(vec2); i++) {
		glVertex2f(gun[i].x, gun[i].y);
	}
	glEnd();

	// draw gun outlines
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	for (int i = 0; i < sizeof(gun) / sizeof(vec2); i++){
		glVertex2f(gun[i].x, gun[i].y);
	}
	glEnd();

	// draw left wing body
	glLineWidth(1.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(0.2f, 0.2f, 1);
	for (int i = 0; i < sizeof(wing1) / sizeof(vec2); i++) {
		glVertex2f(wing1[i].x, wing1[i].y);
	}
	glEnd();

	// draw left wing outlines
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	for (int i = 0; i < sizeof(wing1) / sizeof(vec2); i++) {
		glVertex2f(wing1[i].x, wing1[i].y);
	}
	glEnd();

	// draw right wing
	glLineWidth(1.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(0.2f, 0.2f, 1);
	for (int i = 0; i < sizeof(wing2) / sizeof(vec2); i++) {
		glVertex2f(wing2[i].x, wing2[i].y);
	}
	glEnd();

	// draw right wing outlines
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(1, 1, 1);
	for (int i = 0; i < sizeof(wing2) / sizeof(vec2); i++) {
		glVertex2f(wing2[i].x, wing2[i].y);
	}
	glEnd(); 

	glColor3f(1, 1, 1);  
	glLineWidth(1.0f); 
	glPopMatrix();
}

void spaceship::process() { 
	dir.x = cosf(rotate + PI / 2.0f);
	dir.y = sinf(rotate + PI / 2.0f);
	pos = pos + dir * speed;  

	speed -= spaceshipNaturalDeAccel * 0.03f;
	if (speed < 0)
		speed = 0;
}

void objectpool::render() noexcept {
	for (std::list<gameojbect*>::iterator it = m_gameobjectlist.begin(); it != m_gameobjectlist.end(); it++)
		(*it)->render(); 
}

void objectpool::process() { 
	for (std::list<gameojbect*>::iterator it = m_gameobjectlist.begin(); it != m_gameobjectlist.end(); it++) { 
		(*it)->process();
		float halfArenaX = xwidthofspace * ArenaXPercentage * 0.5f;
		float halfArenaY = yheightofspace * ArenaYPercentage * 0.5f;

		for (std::list<gameojbect*>::iterator it2 = m_gameobjectlist.begin(); it2 != m_gameobjectlist.end(); it2++) {  
			// collision between Asteroid and Arena
			if ((*it2)->type == ASTEROID) {
				// check if Asteroid get into the Arena
				if ((*it2)->inArena == false 
					&& (*it2)->pos.x > -halfArenaX + (*it2)->radius * 2.1 && (*it2)->pos.x < halfArenaX - (*it2)->radius * 2.1
					&& (*it2)->pos.y > -halfArenaY + (*it2)->radius * 2.1 && (*it2)->pos.y < halfArenaY - (*it2)->radius * 2.1) {
					(*it2)->inArena = true;
				}
				// collision with wall after Asteroid get into the Arena
				else if ((*it2)->inArena == true
					&& ((abs(-halfArenaX - ((*it2)->pos.x + (*it2)->radius)) <= ArenaGameOverdistance)
					|| (abs(halfArenaX - ((*it2)->pos.x + (*it2)->radius)) <= ArenaGameOverdistance)))
					(*it2)->dir.x = -(*it2)->dir.x;
				else if ((*it2)->inArena == true
					&& ((abs(-halfArenaY - ((*it2)->pos.y + (*it2)->radius)) <= ArenaGameOverdistance)
					|| (abs(halfArenaY - ((*it2)->pos.y + (*it2)->radius)) <= ArenaGameOverdistance)))
					(*it2)->dir.y = -(*it2)->dir.y;
			}
			// collision between Bullet and Arena
			if ((*it2)->type == BULLET) {
				// collision with wall after Bullet is launched
				if ((abs(-halfArenaX - ((*it2)->pos.x + (*it2)->radius)) <= ArenaGameOverdistance)
					|| (abs(halfArenaX - ((*it2)->pos.x + (*it2)->radius)) <= ArenaGameOverdistance))
					(*it2)->duration = -1;
				else if (((abs(-halfArenaY - ((*it2)->pos.y + (*it2)->radius)) <= ArenaGameOverdistance)
					|| (abs(halfArenaY - ((*it2)->pos.y + (*it2)->radius)) <= ArenaGameOverdistance))
					&& (*it2)->inArena == true)
					(*it2)->duration = -1;
			}

			if ((*it)->checkcollision(**it2)) {
				// collision between Asteroid and Spaceship 
				if ((*it)->type == ASTEROID && (*it2)->type == SPACESHIP || (*it)->type == SPACESHIP && (*it2)->type == ASTEROID) {
					if ((*it)->type == SPACESHIP)
						g_bombmanager.showBombSpaceship(g_gameobjectpool, (*it)->pos, (*it)->radius);
					else
						g_bombmanager.showBombAsteroid(g_gameobjectpool, (*it2)->pos, (*it2)->radius, (*it2)->rColour, (*it2)->gColour, (*it2)->bColour);

					// To WISH TO MAKE SHIP HAVE HEAL POINT. WE HAVE TO MODIFY THIS
					if ((*it)->type == ASTEROID)
						(*it)->duration = -1;
					else if ((*it2)->type == ASTEROID)
						(*it2)->duration = -1;

					// SHIP COLLIDE WITH ASTEROID . WE CAN MAKE HEALTH POINT HERE OR JUST GO TO GAME OVER
					g_gameState = GAME_OVER;
				}
				// collision between Asteroid and Bullet 
				else if ((*it)->type == ASTEROID && (*it2)->type == BULLET || (*it)->type == BULLET && (*it2)->type == ASTEROID) {
					if ((*it)->type == ASTEROID)
						g_bombmanager.showBombAsteroid(g_gameobjectpool, (*it)->pos, (*it)->radius, (*it)->rColour, (*it)->gColour, (*it)->bColour);
					else
						g_bombmanager.showBombAsteroid(g_gameobjectpool, (*it2)->pos, (*it2)->radius, (*it2)->rColour, (*it2)->gColour, (*it2)->bColour);
					g_AsteroidLauncher.m_DestroyedAsteroid++;
					(*it)->duration = -1;
					(*it2)->duration = -1;
				}
				// collision between Asteroid and Asteroid 
				else if ((*it)->type == ASTEROID && (*it)->type == (*it2)->type) {
					(*it)->dir.x = -(*it)->dir.x;
					(*it)->dir.y = -(*it)->dir.y;
					(*it2)->dir.x = -(*it2)->dir.x;
					(*it2)->dir.y = -(*it2)->dir.y;
				}
			}	
		}

		if ((*it)->NeedtoKillthis()) { 
			delete *it;			
			it = m_gameobjectlist.erase(it);
			--it;
		}
	} 
}

void objectpool::SplitAsteroid( Asteroid * asteroid )
{
	// size speed as 2/3 vector as side

}
	 
void bullet::render() noexcept { 
	glPointSize(5);
	glBegin(GL_POINTS);
	glColor3f(1, 0, 0);
	glVertex2f(pos.x, pos.y);
	glEnd(); 
}  

void asteroidlauncher::shootasteroid(objectpool & basepool) {
	int asteroidCount = 0;
	for (std::list<gameojbect*>::iterator it = basepool.m_gameobjectlist.begin(); it != basepool.m_gameobjectlist.end(); it ++) {
		if ((*it)->type == ASTEROID) { 
			if ((*it)->radius > Asteroidminradius) //dont count splited Asteroid
				asteroidCount++;
		} 
	}

	const float timeaftershoot = g_Timemanger.Getsec() - m_lastshoottime;

	if (timeaftershoot > AsteroidInterval) {
		if (asteroidCount < MAXAteroidCount) {
			m_lastshoottime = g_Timemanger.Getsec();

			// pick Random direction and shoot
			float Angle = GetRandom(2.0f * PI);
			float x = cosf(Angle);
			float y = sinf(Angle);

			// GetAsteroid Distance by Arena MAx size 
			float halfArenaX = xwidthofspace * ArenaXPercentage * 0.5f;
			float halfArenaY = yheightofspace * ArenaYPercentage * 0.5f;
			float RadiusofArena = sqrtf(halfArenaX * halfArenaX + halfArenaY * halfArenaY);

			RadiusofArena *= 1.5;

			vec2 startpos(x * RadiusofArena, y * RadiusofArena);
			vec2 dirtoship = g_gameobjectpool.m_Globalspaceship->pos - startpos;
			dirtoship = normalize(dirtoship);
			Asteroid* anewasteroid = new Asteroid;

			anewasteroid->speed = AsteroidSpeed;
			anewasteroid->pos = startpos;
			anewasteroid->dir = dirtoship;
			anewasteroid->radius = GetRandom(AsteroidRandomRadius) + AsteroidBaseRadius;
			anewasteroid->rColour = GetRandom(1);
			anewasteroid->gColour = GetRandom(1);
			anewasteroid->bColour = GetRandom(1);
			// not to make too dark
			if (anewasteroid->rColour < 0.2 && anewasteroid->gColour < 0.2 && anewasteroid->bColour < 0.2) {
				anewasteroid->rColour = anewasteroid->rColour * 3;
				anewasteroid->gColour = anewasteroid->gColour * 3;
				anewasteroid->bColour = anewasteroid->gColour * 3;
			}
			anewasteroid->generateAsteroidOutline(Asteroidmaxradius + anewasteroid->radius, Asteroidminradius + anewasteroid->radius);
			basepool.m_gameobjectlist.push_back(anewasteroid);
		}
	}
}

void ArenaManager::Process() {
	const float halfX =  xwidthofspace * ArenaXPercentage * 0.5f;
	const float halfY =  yheightofspace * ArenaYPercentage * 0.5f;
	//check distance with Left 
	const vec2& shipos = g_gameobjectpool.m_Globalspaceship->pos;
	const float shipradius = g_gameobjectpool.m_Globalspaceship->radius;

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
		g_bombmanager.showBombSpaceship(g_gameobjectpool, g_gameobjectpool.m_Globalspaceship->pos, g_gameobjectpool.m_Globalspaceship->radius);
		g_gameState = GAME_OVER;
	}
} 

void ArenaManager::Render() {
	const float halfX = xwidthofspace * ArenaXPercentage * 0.5f;
	const float halfY = yheightofspace * ArenaYPercentage * 0.5f;
	
	glLineWidth(3.0f);
	glBegin(GL_LINES);

	const vec2 & shipos = g_gameobjectpool.m_Globalspaceship->pos;
	const float shipradius = g_gameobjectpool.m_Globalspaceship->radius;

	// left
	if (abs(-halfX - (shipos.x + shipradius)) <= ArenaTooclosedistance)
		glColor3f(1, 0, 0);
	else 
		glColor3f(1, 1, 1); 
	glVertex2f(-halfX, halfY);
	glVertex2f(-halfX, -halfY);

	// right
	if (abs(halfX - (shipos.x + shipradius)) <= ArenaTooclosedistance)
		glColor3f(1, 0, 0);
	else 
		glColor3f(1, 1, 1); 
	glVertex2f(halfX, halfY);
	glVertex2f(halfX, -halfY);

	// bottom 
	if (abs(-halfY - (shipos.y +shipradius)) <= ArenaTooclosedistance )
		glColor3f(1, 0, 0);
	else 
		glColor3f(1, 1, 1); 
	glVertex2f(-halfX, -halfY);
	glVertex2f(halfX, -halfY);

	// top
	if (abs(halfY - (shipos.y +shipradius)) <= ArenaTooclosedistance)
		glColor3f(1, 0, 0);
	else 
		glColor3f(1, 1, 1); 
	glVertex2f(-halfX, halfY);
	glVertex2f(halfX, halfY);
	
	glColor3f(1, 1, 1);
	glEnd();

	glLineWidth(1.0f);
}

void RestartGame() {
	for (std::list<gameojbect*>::iterator it = g_gameobjectpool.m_gameobjectlist.begin(); it != g_gameobjectpool.m_gameobjectlist.end(); it++) { 
		delete *it;
	}

	g_gameobjectpool.m_gameobjectlist.clear();

	spaceship *s = new spaceship();
	g_gameobjectpool.m_gameobjectlist.push_back(s); 
	s->speed = 0;
	g_gameobjectpool.m_Globalspaceship = s; 
	g_AsteroidLauncher.m_DestroyedAsteroid = 0;
	g_Timemanger.SetStartTime();
}

void bomb::render() noexcept {
	constexpr float count = 16.0f;

	glBegin(GL_LINES);
	glColor3f(rColour, gColour, bColour);
	for (float i = 0; i < count; i++) {
		constexpr float apie = 3.145f * 2.0f / count;
		const float x = cosf(apie * i) * radius;
		const float y = sinf(apie * i) * radius;
		glVertex2f(pos.x, pos.y);
		glVertex2f(pos.x + x, pos.y + y);
	}

	glColor3f(1, 1, 1);
	glEnd();
}