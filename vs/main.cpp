#include "Unifiedheader.h"  
#include "gameobject.h"   

constexpr int screenwidth = 1920;
constexpr int screenheight = 1080;

float timePassed;
int score = 0;
constexpr int droneCost = 500;

void init() noexcept {
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void gameProcess() {
	g_TimeManger.GetTimefromClock();
	g_AsteroidLauncher.shootAsteroid(g_gameObjectPool);
}

constexpr int font = (int)GLUT_BITMAP_9_BY_15;

void renderBitmapString(float x, float y, void* font, const char* string) noexcept {
	const char* c;
	c = nullptr;
	glRasterPos2f(x, y);
	for (c = string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}

void showGameScoreAndTime() {
	const int gametime = g_TimeManger.GetGameTime();
	score = g_AsteroidLauncher.m_DestroyedAsteroid * 100 - (droneCost * g_droneManager.howManyDronesSpawned);

	char titlestring[128], timestring[128], scorestring[128], drone[128];
	glColor3f(1, 1, 1);

	sprintf_s(titlestring, "Asteroid Game");
	if (g_gameState != GAME_OVER) {
		sprintf_s(timestring, "TIME PASSED : %.2f sec", static_cast<float>(gametime) / 1000.0f);
		timePassed = static_cast<float>(gametime) / 1000;
	}
	else {
		sprintf_s(timestring, "TIME PASSED : %.2f sec", timePassed);
	}
	sprintf_s(scorestring, "SCORE : %d", score);
	sprintf_s(drone, "< 1 DRONE COST : 500 >");

	renderBitmapString(-5, (yheightofspace * ArenaYPercentage * 0.5f) + 1, (void*)font, titlestring);
	renderBitmapString(-(xwidthofspace * ArenaXPercentage * 0.5f), (yheightofspace * ArenaYPercentage * 0.5f) + 1, (void*)font, timestring);
	renderBitmapString((xwidthofspace * ArenaXPercentage * 0.5f) - 10, (yheightofspace * ArenaYPercentage * 0.5f) + 1, (void*)font, scorestring);
	renderBitmapString((xwidthofspace * ArenaXPercentage * 0.5f), (yheightofspace * ArenaYPercentage * 0.5f) + 1, (void*)font, drone);
}

void display(void) {
	// Process   
	if (g_gameState == GAME_PLAYING)
		gameProcess();

	// Render
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// if game over
	if (g_gameState == GAME_OVER) {
		gameProcess();
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(-xwidthofspace / 2.0f, xwidthofspace / 2.0f,
			-yheightofspace / 2.0f, yheightofspace / 2.0f, 1, -1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		g_ArenaManager.Render();

		g_gameObjectPool.process();
		g_gameObjectPool.render();
		
		showGameScoreAndTime();

		renderBitmapString(0, 0, (void*)font, "Game Over. Press any key to play again...");
	}
	// if game start
	else if (g_gameState == FIRST_START) {
		renderBitmapString(0, 0, (void*)font, "Press any key to start...");
	}
	// if game is starting
	else if (g_gameState == GAME_PLAYING) {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(-xwidthofspace / 2.0f, xwidthofspace / 2.0f,
			-yheightofspace / 2.0f, yheightofspace / 2.0f, 1, -1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		g_ArenaManager.Process();
		g_ArenaManager.Render();

		g_gameObjectPool.process();
		g_gameObjectPool.render();

		showGameScoreAndTime();
	}
	glutSwapBuffers();
}

// mouse button
void mouse(int button, int state, int x, int y) {
	if (g_gameState != GAME_PLAYING) {
		g_gameState = GAME_PLAYING;
		g_TimeManger.SetStartTime();
		RestartGame();
	}

	if (button == GLUT_LEFT_BUTTON) {
		if (g_missileManager.missileInUse == false)
			g_bulletManager.spaceshipFireBullet(g_gameObjectPool, *g_gameObjectPool.m_GlobalSpaceship);
		else
			g_missileManager.spaceshipFireMissile(g_gameObjectPool, *g_gameObjectPool.m_GlobalSpaceship);
	}
}

// keyboard button
void keyboard(unsigned char key, int x, int y) {
	if (g_gameState != GAME_PLAYING) {
		g_gameState = GAME_PLAYING;
		g_TimeManger.SetStartTime();
		RestartGame();
	}
	else {
		switch (key) {
		case 'w':
		case 'W':
			g_gameObjectPool.m_GlobalSpaceship->speed += spaceshipAccel;
			if (g_gameObjectPool.m_GlobalSpaceship->speed >= spaceshipMaxSpeed)
				g_gameObjectPool.m_GlobalSpaceship->speed = spaceshipMaxSpeed;
			break;
		case 'a':
		case 'A':
			g_gameObjectPool.m_GlobalSpaceship->rotate += spaceshipRotateSpeed;
			break;
		case 'd':
		case 'D':
			g_gameObjectPool.m_GlobalSpaceship->rotate -= spaceshipRotateSpeed;
			break;
		case 'x':
		case 'X':
			if ((score >= 500) && (g_droneManager.droneInArena < maxDroneNumberAtOnce))
				g_droneManager.spawnDrone(g_gameObjectPool, *g_gameObjectPool.m_GlobalSpaceship);
			break;
		case 'z':
		case 'Z':
			g_missileManager.toggleMissile(g_gameObjectPool, *g_gameObjectPool.m_GlobalSpaceship);
			break;
		default:
			break;
		}
	}
}

int main(int argc, char** argv) {
	init();

	initAll();

	glutInitWindowSize(screenwidth, screenheight);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);
	glutCreateWindow("ASTEROID GAME");

	//glutFullScreen();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutIdleFunc(display);

	constexpr float clearcolor = 0.0;

	glClearColor(clearcolor, clearcolor, clearcolor, 1.f);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);

	glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();

	return 0;
}