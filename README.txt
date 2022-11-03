Student Name : You Chan Lee
Student ID: s3850825

I've implemented all the feature including bonus features. Basically, I've used ASTEROID, BULLET, SPACESHIP,	EXPLOSION, DRONE, MISSILE objects in gameobject.
Also, I've used manager of each game object so that the manager can controll each game object. Some math functions are made for vector calculation in vectormath.cpp.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

LEVEL 1 FEATURES
1.1 - Screen Mode
	glutFullScreen() is used.
	Please check line 176 in main.cpp.
1.2 - The Arena
	I've set the size of Arena to 120 x 90 and used grey colour for backgrournd with 4 white outlines.
	Please check lines 26-27 in Unifiedheader.h, lines 57-110 in main.cpp, lines 684-776 in gameobject.cpp, lines 407-411 in gameobject.h.
1.3 - Ready Player 1
	I've used dark orange colour for 2 wings, and light orange colour for body with white outlines using GL_QUADS, GL_LINE_LOOP.
	Please check lines 108-234 in gameobject.cpp.
1.4 - Starting Location
	I've set the default position vector to (0, 0) and direction vector to (0, 1), and also spaceship size is much smaller than Arena.
	Please check lines 75-129 in gameobject.h.
1.5 - Player Movement
	When game is not playing, if a player presses any key, then it restars the game.
	w and W key for moving forward, a and A key for rotating anti-clockwise, d and D key for rotating colockwise, x and X key for spawning drone, and z and Z key for toggling gun and missile.
	Please check lines 129-164 in main.cpp.
1.6 - Hit the Wall
	If spaceship gets close to the any wall of Arena, the line(wall) colour is changed to red. If it is not close anymore, then change the line colour back to white.
	If spaceship hits the wall, then game state is changed to "game over".
	Please check lines 684-776 in gameobject.cpp.
1.7 - Shooting
	If player press mouse left button, bullet launches to the same direction of spaceship.
	Please check lines 496-502 in gameobject.cpp, 264-282 in gameobject.h, 129-164 in main.cpp.
1.8 - Math Structs/Classes and Functions
	I've made vectormath.cpp and vectormath.h for calculating.
	Please check vectormath.cpp and vectormath.h files.
1.9 - Code Quality
	I've tried to use consistent naming like this, longNameParameter.
	Also, I've tried to devide the long function into small functions. 

LEVEL 2 FEATURES
2.1 - Launch Position
	I've set the start position of Asteroid to 1.1 times of Arena's radius. 
	Please check lines 623-682 in gameobject.cpp.
2.2 - Asteroid Launch
	I've set the speed, rotation, colour of Asteroid to random with maximum and minimum value.  
	Please check lines 26-86, 623-682 in gameobject.cpp.
2.3 - Gun Rendering & Shooting
	I've drawn gun with grey colour body and white outline. It is located on the spaceship with bit of overlapping to make it real. 
	Please check lines 108-161 in gameobject.cpp, and lines 264-282 in gameobject.h.
2.4 - Asteroid / Ship / Bullet Collision
	I've checked collisions between objects and killed after collision with several functions.
	Please check lines 16-22, 88-104, 254-439 in gameobject.cpp.
2.5 - Multiple Asteroids
	I've implemented waves with an array. Firstly, shoot one Asteroid, and then shoot two Asteroids, etc.
	Please check lines 623-682 in gameobject.cpp.

LEVEL 3 FEATURES
3.1 - Procedurally Generated Asteroids
	I've implemented this by adding random vertex every 30 degree.
	Please check lines 26-41 in gameobject.cpp.
3.2 - Rotating Asteroids
	I've rotated Asteroid with random number with maximum and minimum value.
	Please check lines 43-86 in gameobject.cpp.
3.3 - Gun Switching
	I've drawn a missile launcher with black colour body and white outline. It is located on the spaceship with bit of overlapping to make it real. 
	Please check lines 108-161, 556-618 in gameobject.cpp, and lines 374-400 in gameobject.h, 156-159 in main.cpp.
3.4 - Hit Points
	I've used objectHasCrack parameter to distinguish whether a Asteroid has full energy or not.
	Please check lines 72-86, 373-439 in gameobject.cpp, and lines 75-90 in gameobject.h
3.5 - Time and Score
	I've used timePassed and score parameter in main.cpp.
	Please check lines 7-8, 33-55 in main.cpp.
3.6 - Game Over, Man
	I've used renderBitmapString() to show game over and game start messages.
	Please check lines 57-110 in main.cpp.

LEVEL 4 FEATURES
4.1 - Bouncies
	I've let asteroids get into Arena first, and then make it bounce when they hit the wall using their position and direction vector.
	Please check lines 261-309 in gameobject.cpp.
4.2 - Bouncies 2
	I've let asteroids bounce each other using their position and direction vectors.
	Please check lines 339-351 in gameobject.cpp
4.3 - Do The Splits
	I've set asteroid's energy to 2 when its radius is over 3.0, so after cracking when it's energy is 0, then it splits to half size of original Asteroid.
	Please check lines 387-415, 441-481 in gameobject.cpp.
4.4 - Earth Shattering Kaboom
	I've made an explosion particle effect with random direction, speed and angle.
	Please check lines 373-439, 496-502 in gameobject.cpp.

BONUS FEATURES
B.1 - Better Ship Movement
	I've set spaceshipNaturalDeAccel so that spaceship naturally slow down.
	Please check lines 236-247 in gameobject.cpp.
B.2 - Spawn Drone
	I've made drone feature which launches 6 bullets on each edge two times before it gets killed.
	Player only can use two drones at a time for a limited time, but as long as enough score, it can be used many times.
	Please check lines 506-554 in gameobject.cpp, 331-368 in gameobject.h, 151-155 in main.cpp.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PS.
	I've tried to run my program on many different desktops and laptops, and I realized that the game speed is all different on every single machine.
	I've set up the best speed and all the setting to my laptop, so please let me know if you have an issue with speed on your machine.
	Or you can adjust some const parameters like asteroidInterval, asteroidMinSpeed, asteroidMaxSpeed (lines 244-247 in gameobject.h) and bulletInterval, bulletSpeed (lines 260-262 in gameobject.h).
	I can do demo through google meet or microsoft Team if it is necessary.
	Thanks for the lectorial and tutorial, I've really enjoyed and now I'm really intereted in 2D, 3D programming. 