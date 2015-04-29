#pragma once

#include "defines.h"
#include <vector>

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>


#define GRAV_DOWN 0
#define GRAV_RIGHT 1
#define GRAV_UP 2
#define GRAV_LEFT 3

using namespace std;

class Entity {
public:
	Entity(float x, float y, float xr, float yr, float colorR = 1.0f, float colorG = 0.0f, float colorB = 0.0f, bool statc = false, bool destrct = false, bool coin = false);
	void Update(float elapsed);
	void Render();
	bool collidesWith(Entity *entity);
	void FixedUpdate(unsigned char **level, int mapHeight, int mapWidth, Entity* player = NULL);
	void jump();
	void ResetDynamic();
	void playerInput();
	void rotateGravR();
	void rotateGravL();
	void setGrav(int direction);



	void destroy(bool start);
	float eventTime;
	bool eventSequence;


	const float initXpos;   
	const float initYpos;
	float rIn, gIn, bIn;
	float r, g, b;

	float xPos;
	float yPos;
	float xRad;
	float yRad;
	float xVel;
	float yVel;
	float xAccel;
	float yAccel;

	int gravFlag;
	float yGrav;
	float xGrav;
	float xFric;
	float yFric;
	

	const bool isStatic;
	const bool destructable;
	const bool isCoin;
	bool isVisable;
	
	//bool enableCollisions;

	bool collidedTop;
	bool collidedBottom;
	bool collidedLeft;
	bool collidedRight;

	int score;

	Mix_Chunk *jumpSound;
	Mix_Chunk *hitSound;
	GLuint sheet;
};

float lerp(float v0, float v1, float t);
void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY);