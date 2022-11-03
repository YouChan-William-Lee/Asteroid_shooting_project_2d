#include "Unifiedheader.h"  
#include "gameobject.h"   

constexpr int screenwidth = 1920;
constexpr int screenheight = 1080; 

float timePassed;
int score;

void init() noexcept {
	/* In this program these OpenGL calls only need to be done once,
	  but normally they would go elsewhere, e.g. display */

	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

void gameProcess() {
	g_Timemanger.GetTimefromClock();
	g_AsteroidLauncher.shootasteroid( g_gameobjectpool ); 
}

constexpr int font = (int)GLUT_BITMAP_9_BY_15;

void renderBitmapString(float x, float y, void *font,const char *string) noexcept {
    const char *c;
	c = nullptr;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
} 

void showGameScoreandTime() {
	const int gametime = g_Timemanger.GetGameTime() ;

	char titlestring[128], timestring[128], scorestring[128]; 

	sprintf_s (titlestring, "Asteroid Game");
	sprintf_s (timestring, "TIME PASSED : %.2f sec", static_cast<float>(gametime) / 1000); 
	sprintf_s (scorestring, "SCORE : %d", g_AsteroidLauncher.m_DestroyedAsteroid * 100); 

	timePassed = static_cast<float>(gametime) / 1000;
	score = g_AsteroidLauncher.m_DestroyedAsteroid * 100;

	renderBitmapString(-5, (yheightofspace * ArenaYPercentage * 0.5f) + 1, (void*)font, titlestring);
	renderBitmapString(-(xwidthofspace * ArenaXPercentage * 0.5f), (yheightofspace * ArenaYPercentage * 0.5f) + 1, (void *)font, timestring);
	renderBitmapString((xwidthofspace * ArenaXPercentage * 0.5f) - 10, (yheightofspace * ArenaYPercentage * 0.5f) + 1, (void *)font, scorestring);
}

void display(void) { 
	// Process   
	if (g_gameState == GAME_PLAYING)
		gameProcess();

	// Render
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   

	// if game over
	if (g_gameState == GAME_OVER) { 
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();  

		glOrtho(-xwidthofspace / 2.0f,xwidthofspace / 2.0f,
			-yheightofspace / 2.0f,yheightofspace / 2.0f, 1, -1);

		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity(); 
		 
		g_gameobjectpool.render() ;
		g_Arenamanager.Process();
		g_Arenamanager.Render();  

		char titlestring[128], timestring[128], scorestring[128];

		sprintf_s(titlestring, "Asteroid Game");
		sprintf_s(timestring, "TIME PASSED : %.2f sec", timePassed);
		sprintf_s(scorestring, "SCORE : %d", score);

		renderBitmapString(-5, (yheightofspace * ArenaYPercentage * 0.5f) + 1, (void*)font, titlestring);
		renderBitmapString(-(xwidthofspace * ArenaXPercentage * 0.5f), (yheightofspace * ArenaYPercentage * 0.5f) + 1, (void*)font, timestring);
		renderBitmapString((xwidthofspace * ArenaXPercentage * 0.5f) - 10, (yheightofspace * ArenaYPercentage * 0.5f) + 1, (void*)font, scorestring);

		renderBitmapString(0, 0, (void *)font, "Game Over. Press any key to play again"); 
	}
	// if game start
	else if (g_gameState == FIRST_START) {
		renderBitmapString(0, 0, (void *)font, "Press any key to start");
	}
	// if game is starting
	else if (g_gameState == GAME_PLAYING) { 
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();  

		glOrtho(-xwidthofspace / 2.0f, xwidthofspace / 2.0f,
			-yheightofspace / 2.0f, yheightofspace / 2.0f, 1, -1);

		glMatrixMode(GL_MODELVIEW); 
		glLoadIdentity(); 

		g_gameobjectpool.process();
		g_gameobjectpool.render() ;

		g_Arenamanager.Process();
		g_Arenamanager.Render();  
		 
		showGameScoreandTime();
	}    
	glutSwapBuffers(); 
}

// mouse button
void mouse(int button, int state, int x, int y) {
    if (g_gameState != GAME_PLAYING) {
		 g_gameState = GAME_PLAYING;
		 g_Timemanger.SetStartTime() ; 
		 RestartGame();
	}

	if (button == GLUT_LEFT_BUTTON) {
        //shoot bullet
		g_bulletmanager.firebullet(g_gameobjectpool,*g_gameobjectpool.m_Globalspaceship); 
    } 
}
 
// keyboard button
void keyboard(unsigned char key, int x, int y) {
	if (g_gameState != GAME_PLAYING) {
		 g_gameState = GAME_PLAYING;
		 g_Timemanger.SetStartTime() ; 
		 RestartGame();
	}
	else {
		switch (key) {
		case 'w':
		case 'W':
			g_gameobjectpool.m_Globalspaceship->speed += spaceshipAccel;
			if (g_gameobjectpool.m_Globalspaceship->speed >= spaceshipMAXSPEED)
				g_gameobjectpool.m_Globalspaceship->speed = spaceshipMAXSPEED;
			break;
		case 's':
		case 'S':
			g_gameobjectpool.m_Globalspaceship->speed -= spaceshipDeAccel;
			if (g_gameobjectpool.m_Globalspaceship->speed < 0)
				g_gameobjectpool.m_Globalspaceship->speed = 0;
			break;
		case 'a':
		case 'A':
			g_gameobjectpool.m_Globalspaceship->rotate += spaceshipRotateSpeed;
			break;
		case 'd':
		case 'D':
			g_gameobjectpool.m_Globalspaceship->rotate -= spaceshipRotateSpeed;
			break;
		default:
			break;
		}
	}
}


int main(int argc, char **argv) {   
	init();

	initAll();  

 	glutInitWindowSize(screenwidth,screenheight);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInit(&argc, argv);
	glutCreateWindow("ASTEROID GAME");

	//glutFullScreen();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse );
	glutIdleFunc(display);
 
	constexpr float clearcolor = 0.0; 

	glClearColor(clearcolor, clearcolor, clearcolor, 1.f);  

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE); 
	 
	glutGet(GLUT_ELAPSED_TIME);
	glutMainLoop();
	   
	return 0;
}