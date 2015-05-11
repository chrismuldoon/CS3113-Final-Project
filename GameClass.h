#pragma once

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "defines.h"
#include "common.h"
#include "entity.h"

#include <SDL_mixer.h>


#include <fstream>
#include <string>
#include <iostream>
#include <sstream>


//define for level


using namespace std;

class GameClass {
public:
	GameClass();
	~GameClass();
	void Init();
	bool UpdateAndRender();
	void Render();
	void renderLevel();
	void Update(float elapsed);
	void FixedUpdate();

	void RenderGO();
	void RenderMenu();
	void UpdateGO();
	void UpdateMenu();

	void ReadTileMapFile();
	bool ReadTileMapHeaderData(std::ifstream &stream);
	bool ReadTileMapLayerData(std::ifstream &stream);
	bool ReadTileMapEntityData(std::ifstream &stream);


private:
	int state;
	int levelNum;

	bool done;
	float lastFrameTicks;
	float timeLeftOver;
	SDL_Window* displayWindow;
	SDL_Event event;
	GLuint textImg;
	GLuint spriteImg;
	//unsigned char levelData[LEVEL_HEIGHT][LEVEL_WIDTH];
	unsigned char **levelData;
	int mapWidth;
	int mapHeight;

	Entity* player;
	Entity* box;
	Entity* exit;
	//Entity* box;
	vector<Entity*> enemies;

	Mix_Music *music;

	SDL_Joystick * playerOneController;

	void enemyCollision(Entity* player, Entity* enemy);
	void InitLevel();
};