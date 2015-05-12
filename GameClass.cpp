#pragma once

#include "GameClass.h"
#include "defines.h"


// 60 FPS (1.0f/60.0f)
//#define FIXED_TIMESTEP 0.0166666f
//#define MAX_TIMESTEPS 6


enum GameState { STATE_MENU, STATE_GAME, STATE_GAMEOVER };
int deaths = 0;

GameClass::GameClass() {
	Init();
	done = false;
	lastFrameTicks = 0.0f;
	timeLeftOver = 0.0f;
	logo = LoadTexture("logo.png");
	logoPos = .9f;
	//deaths = 0;
}
void GameClass::Init() {
	//video stuff
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	displayWindow = SDL_CreateWindow("THERE HAS TO BE A BETTER WAY", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);


	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	//other stuff
	textImg = LoadTexture("font1.png");
	spriteImg = LoadTexture("arne_sprites.png");

	state = STATE_MENU;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

	finishSound = Mix_LoadWAV("finish.wav");
	deathSound = Mix_LoadWAV("ouch.wav");


	music = Mix_LoadMUS("capn.mp3");
	Mix_PlayMusic(music, -1);
	Mix_VolumeMusic(37);


	//InitLevel();

}

void GameClass::InitLevel()
{
	//delete old stuff
	if (player) delete player;
	if (exit) delete exit;

	for (size_t i = 0; i < enemies.size(); i++){
		if (enemies[i]) delete enemies[i];
	}
	enemies.clear();


	//make new stuff
	//make variable called levelnum
	//modify read map file to aaccount for it
	//change the variable on level completion or wherever this function is called
	
	ReadTileMapFile();

	


	playerOneController = SDL_JoystickOpen(0);
	if (playerOneController) printf("\n\n\n\n\n\n\n\n\##################################\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	//player = new Entity(.33f, -1.0f, 0.06f, 0.06f);
	//box = new Entity(0.7f, -0.9f, 0.06f, 0.06f);

	player->sheet = spriteImg;
	player->jumpSound = Mix_LoadWAV("smw_jump.wav");
	player->changeSound = Mix_LoadWAV("change.wav");
	
	player->killSound = Mix_LoadWAV("kill.wav");




	//Mix_VolumeChunk((player->hitSound), 127);
	Mix_VolumeChunk((player->jumpSound), 90);

	exit->sheet = spriteImg;
	
	for (size_t i = 0; i < enemies.size(); i++){
		enemies[i]->sheet = spriteImg;

	}

	
	//someSound = Mix_LoadWAV("hangout.wav");

	//Mix_PlayChannel(-1, someSound, 0);


}
//defines for sprite sheet
#define SPRITE_COUNT_X 16
#define SPRITE_COUNT_Y 8

void GameClass::renderLevel(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, spriteImg);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::vector<float> vertexData;
	std::vector<float> texCoordData;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			//if goes here
			if ((int)levelData[y][x] != 0) {
				float u = (float)(((int)levelData[y][x]) % SPRITE_COUNT_X) / (float)SPRITE_COUNT_X;
				float v = (float)(((int)levelData[y][x]) / SPRITE_COUNT_X) / (float)SPRITE_COUNT_Y;
				float spriteWidth = 1.0f / (float)SPRITE_COUNT_X;
				float spriteHeight = 1.0f / (float)SPRITE_COUNT_Y;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
				});
				texCoordData.insert(texCoordData.end(), { u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight),
					u + spriteWidth, v
				});
			}
		}
	}

	//glLoadIdentity();
	//glTranslatef(-TILE_SIZE * LEVEL_WIDTH / 2, TILE_SIZE * LEVEL_HEIGHT / 2, 0.0f);


	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, mapHeight*mapWidth * 4);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);


}

GameClass::~GameClass() {
	//SDL_Quit();
	//Mix_FreeChunk(someSound);
	SDL_JoystickClose(playerOneController);
	Mix_FreeMusic(music);
	SDL_Quit();
}
void GameClass::RenderMenu(){
	// render stuff
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//size, spacing, x, y, rgba

	//DrawText(textImg, "INVADERS", 0.40, -0.16, -0.82, 0.5, 1.0, 1.0, 1.0, 1.0);
	//DrawText(textImg, "I just met her!", 0.20, -0.11, -1.1, -0.3, 1.0, 1.0, 1.0, 1.0);
	DrawSprite(logo, 0.0, logoPos, 0.0);
	
	
	DrawText(textImg, "(Press Enter to start)", 0.16, -0.11, -0.5, -0.85, 1.0, 1.0, 1.0, 1.0);

	//DrawRectanglee(0.0, 0.50, .50, .190);
	SDL_GL_SwapWindow(displayWindow);



}

void GameClass::RenderGO(){
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//DrawRectanglee(0.0, 0.0, 1.5, 2.0);

	//if (!winner) DrawText(textImg, "YOU DIED", 0.40, -0.16, -0.82, 0.5, 1.0, 0.0, 0.0, 1.0);
	DrawText(textImg, "YOU WIN", 0.40, -0.16, -0.72, 0.5, 1.0, 1.0, 0.0, 1.0);


	//DrawText(textImg, "Score: ", 0.16, -0.09, -0.48, -0.001, 1.0, 1.0, 1.0, 1.0);
	//DrawText(textImg, std::to_string(score * 10), 0.16, -0.09, 0.12, -0.001, 1.0, 1.0, 1.0, 1.0);
	DrawText(textImg, "(Press esc to continue)", 0.16, -0.09, -0.72, -0.3, 0.2, 0.2, 0.2, 0.8);


	SDL_GL_SwapWindow(displayWindow);

}


void GameClass::Render() {
	//glClearColor(0.04f, 0.60f, 0.67f, 1.0f);
	glClearColor(0.04f, 0.20f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// render stuff

	//DrawText(textImg, "Hello World", 0.2f, -0.1f, -0.9f, 0.5f, 1.0, 1.0, 1.0, 1.0);
	//DrawRectangle(0.0f, -0.2f, 0.2f, 0.2f);

	//DrawSpriteSheetSprite(spriteImg, 80, 16, 8);
	



	//scrolling
	glLoadIdentity();
	//DrawSpriteSheetSprite(spriteImg, 80, 16, 8, 0.4f, 0.4f);


	float transAmt = player->xPos*-1.0f; //follow the player
	//stay within bounds of level
	if (transAmt > -1.33f) transAmt = -1.33f;
	else if (transAmt < -mapWidth*TILE_SIZE + 1.33f) transAmt = -mapWidth*TILE_SIZE + 1.33f;
	//wiggle room
	
	float transAmtY = player->yPos*-1.0f; //follow the player
	if (transAmtY < 1.0f) transAmtY = 1.0f;
	else if (transAmtY > mapHeight*TILE_SIZE - 1.0f) transAmtY = mapHeight*TILE_SIZE - 1.0f;


	//glTranslatef(player->xPos*-1.0, player->yPos*-1.0, 0.0);
	glTranslatef(transAmt, transAmtY, 0.0);

	renderLevel();

	glPushMatrix();
	for (size_t i = 0; i < enemies.size(); i++){
		glPushMatrix();
	}

	//DrawSpriteSheetSprite(spriteImg, 50, 16, 8, player->xPos + .1f, player->yPos, player->xRad, player->yRad, 0.0f);

	exit->Render();
	glPopMatrix();
	player->Render();
	

	for (size_t i = 0; i < enemies.size(); i++){
		//printf("render enemy at %f, %f", enemies[0]->xPos, enemies[0]->yPos);
		glPopMatrix();
		enemies[i]->Render();
		
	}


	DrawText(textImg, "Deaths:", 0.10, -0.06, 0.71, 0.9, 1.0, 1.0, 1.0, 1.0);
	DrawText(textImg, std::to_string(deaths), 0.10, -0.06, 1.05, 0.9, 1.0, 1.0, 1.0, 1.0);




	SDL_GL_SwapWindow(displayWindow);
	
}
void GameClass::Update(float elapsed) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		//input
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
				// DO AN ACTION WHEN SPACE IS PRESSED!
				//if (!player) break;
				printf("%i", player->collidedBottom);
				player->jump();
			}
			//if (event.key.keysym.scancode == SDL_SCANCODE_Z) {
			//	//do for all dynamic entities
			//	player->rotateGravL(); 
			//	
			//}
			//if (event.key.keysym.scancode == SDL_SCANCODE_X) {
			//	//do for all dynamic entities
			//	player->rotateGravR();
			//}

			//other keys...

			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				state = STATE_MENU;
			}

			if (event.key.keysym.scancode == SDL_SCANCODE_W) {
				//do for all dynamic entities
				player->setGrav(GRAV_UP);
				//box->setGrav(GRAV_UP);
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_A) {
				//do for all dynamic entities
				player->setGrav(GRAV_LEFT);
				//box->setGrav(GRAV_LEFT);
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_S) {
				//do for all dynamic entities
				player->setGrav(GRAV_DOWN);
				//box->setGrav(GRAV_DOWN);
			}
			if (event.key.keysym.scancode == SDL_SCANCODE_D) {
				//do for all dynamic entities
				player->setGrav(GRAV_RIGHT);
				//box->setGrav(GRAV_RIGHT);
			}



		}
		else if (event.type == SDL_JOYBUTTONDOWN && event.jbutton.button == 10) {
			printf("\njumps!!!!!");
			player->jump();

			// event.jbutton.which tells us which controller (e.g. 0,1,etc.)
			// event.jbutton.button tells us which button was pressed (0,1,2…etc)
		}

	}
}
bool GameClass::UpdateAndRender() {
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;


	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		//case for fixed update
		
		switch (state)
		{
		case STATE_MENU:
			FixedUpdateMenu();
			break;
		case STATE_GAME:
			FixedUpdate();
			break;
		case STATE_GAMEOVER:
			//FixedUpdateGO();

			break;
		}
		
		
		
		


	}
	timeLeftOver = fixedElapsed;
	//case for update and render


	switch (state)
	{
	case STATE_MENU:
		UpdateMenu();
		RenderMenu();
		break;
	case STATE_GAME:
		Update(elapsed);
		Render();
		break;
	case STATE_GAMEOVER:
		UpdateGO();
		RenderGO();
		break;
	}






	
	
	return done;
}

void GameClass::FixedUpdateMenu(){
	logoPos -= 0.005f;
	if (logoPos < 0) logoPos = 0;
	deaths = 0;
	lastDeaths = 0;

}

void GameClass::UpdateMenu(){

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
				// DO AN ACTION WHEN SPACE IS PRESSED!
				//if (!player) break;
				levelNum = 1;
				InitLevel();
				state = STATE_GAME;
			}
		}
	}



}

void GameClass::UpdateGO(){

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				// DO AN ACTION WHEN SPACE IS PRESSED!
				//if (!player) break;
				state = STATE_MENU;
			}
		}
	}



}


void GameClass::FixedUpdate(){
	//prints once per second
	//static int counter = 0;
	//if(!(counter % 60)) printf("a");
	//counter++;


	player->playerInput();
	player->FixedUpdate(levelData, mapHeight, mapWidth);
	
	for (size_t i = 0; i < enemies.size(); i++){
		if (enemies[i]->isVisable){
			enemies[i]->FixedUpdate(levelData, mapHeight, mapWidth);
			enemies[i]->xAccel = enemies[i]->enemyAccel;
			enemyCollision(player, enemies[i]);
			//check player collisions
		}

	}

	if (player->collidesWith(exit)) { 
		if (levelNum == 5) { state = STATE_GAMEOVER; return; }
		levelNum++;

		InitLevel();
		Mix_PlayChannel(-1, finishSound, 0);
		
		
		
		printf("winner!"); }

	if (deaths > lastDeaths){
		InitLevel();
		lastDeaths = deaths;
		Mix_PlayChannel(-1, deathSound, 0);
	
	}

	for (size_t i = 0; i < enemies.size(); i++){
		if (distance(player, enemies[i]) < 0.6f)
			enemies[i]->playerClose = true;
		
	
	}

}

float GameClass::distance(Entity* ent1, Entity* ent2){
	float xDist = ent2->xPos - ent1->xPos;
	float yDist = ent2->yPos - ent1->yPos;
	return sqrt(xDist*xDist + yDist*yDist);
}


void GameClass::enemyCollision(Entity* player, Entity* enemy){
	if (player->collidesWith(enemy))
	{ 
		if (player->yPos > enemy->yPos + enemy->yRad){
			printf("\nKILL\n");
			player->yVel = 1.0f;
			enemy->isVisable = false;
			Mix_PlayChannel(-1, player->killSound, 0);
		}
		else{ 
			printf("\nENEMY!!!\n"); 
			Mix_PlayChannel(-1, deathSound, 0);
			//player->ResetDynamic();
			//InitLevel();
			deaths++;
		}
		
	
	
	}
	else return;
	



}

/////////////////////////////
//file stuff

void GameClass::ReadTileMapFile() {

	std::string levelName= LEVEL_1;

	if (levelNum == 1) levelName = LEVEL_1;
	else if (levelNum == 2) levelName = LEVEL_2;
	else if (levelNum == 3) levelName = LEVEL_3;
	else if (levelNum == 4) levelName = LEVEL_4;
	else if (levelNum == 5) levelName = LEVEL_5;

	std::ifstream infile(levelName);
	std::string line;
	while (std::getline(infile, line)) {
		if (line == "[header]" && !ReadTileMapHeaderData(infile)) {
			return;
		}
		else if (line == "[layer]") {
			printf("\ndebug at layer tile layer");
			ReadTileMapLayerData(infile);
		}
		else if (line == "[Object Layer 1]") {
			printf("\ndebug at obj layer");
			ReadTileMapEntityData(infile);
		}
	}
}

bool GameClass::ReadTileMapHeaderData(std::ifstream &stream) {
	std::string line;
	mapWidth = -1;
	mapHeight = -1;
	while (std::getline(stream, line)) {
		if (line == "") { break; }
		std::istringstream sStream(line);
		std::string key, value;
		std::getline(sStream, key, '=');
		std::getline(sStream, value);
		if (key == "width") {
			mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}

	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else {
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool GameClass::ReadTileMapLayerData(std::ifstream &stream) {
	std::string line;
	while (std::getline(stream, line)) {
		if (line == "") { break; }
		std::istringstream sStream(line);
		std::string key, value;
		std::getline(sStream, key, '=');
		std::getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				std::getline(stream, line);
				std::istringstream lineStream(line);
				std::string tile;
				for (int x = 0; x < mapWidth; x++) {
					std::getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 12;
					}
				}
			}
		}
	}
	return true;
}

bool GameClass::ReadTileMapEntityData(std::ifstream &stream) {
	std::string line, type;
	while (std::getline(stream, line)) {
		if (line == "") { break; }
		std::istringstream sStream(line);
		std::string key, value;
		std::getline(sStream, key, '=');
		std::getline(sStream, value);
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {
			std::istringstream lineStream(value);
			std::string xPosition, yPosition;
			std::getline(lineStream, xPosition, ',');
			std::getline(lineStream, yPosition, ',');
			float placeX = atoi(xPosition.c_str()) / 1 * TILE_SIZE + TILE_SIZE;
			float placeY = atoi(yPosition.c_str()) / 1 * -TILE_SIZE;
			if (type == "player"){
				printf("in here\n");

				//player = new Entity(.33f, -1.0f, 0.06f, 0.06f);

				player = new Entity(placeX, placeY, "player");
				printf("\n ");
				cout << xPosition << " " << yPosition <<endl;
				printf("%f, %f", placeX, placeY);
				}
			else if (type == "enemy")
			{
				enemies.push_back(new Entity(placeX, placeY, "enemy"));
				printf("ENEMY\n");



			}
			else if (type == "goal")
			{
				exit = new Entity(placeX, placeY, "goal");
				printf("EXIT\n");
				printf("\n ");
				cout << xPosition << " " << yPosition << endl;
				printf("%f, %f", placeX, placeY);
			}
		}
	}
	return true;
}


