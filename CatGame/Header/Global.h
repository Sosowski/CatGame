#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include <string>
#include <sstream>
#include <vector>
#include <cmath>
#include <math.h>
#include "Windows.h"

//This file contains the global variables used across all files.
/*
A NOTE ABOUT STATIC AND EXTERN
STATIC declarations create a copy of the variable into each file. Modifications to a variable in one file
ARE NOT transferred to the other files. For manipulation between multiple files, see extern.

EXTERN declartions are used as such : One file declares the variable with no modifiers, the second points to
it by declaring the varaible as extern. Currently, only one of these extern declarations can created for the entire
project. It will be crucial to figure out a method to increase this limit in the future. 
*/

//Game State
//0 - Title Screen | 1 - In Game
static int GAME_STATE = 0;

//Jack sprite dimensions
const int JACK_WIDTH = 241;
const int JACK_HEIGHT = 294;

//Int usage is simply for labling purposes
const int JACK_RIGHT = 0;
const int JACK_LEFT = 1;
const int JACK_RIGHT_JUMP = 2;
const int JACK_RIGHT_FALL = 3;
const int JACK_LEFT_JUMP = 4;
const int JACK_LEFT_FALL = 5;

//Jack's health
static int player_health = 10;

//FPS Cap
const int FRAMES_PER_SECOND = 60;

//Starting Screen Attributes
static int SCREEN_WIDTH = 1280;
static int SCREEN_HEIGHT = 786;
static int SCREEN_BPP = 32;

//Stage Size
static int LEVEL_WIDTH = 5000;
static int LEVEL_HEIGHT = 2000;

//Camera
//extern SDL_Rect camera;
static SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
static SDL_Rect camera2 = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

//Platforms       xpos,ypos,xsize,ysize
//SDL_Rect plat1 = { 0, 500, 500, 100 };
//SDL_Rect plat2 = { 1000, 950, 300, 100 };
//SDL_Rect plat3 = { 500, 1500, 700, 50 };

//Player states
static bool jumping = true;

//Buttons
static SDL_Surface *startButton = NULL;

//Images
static SDL_Surface *background = NULL; //Needs to be fixed
static SDL_Surface *jackRun = NULL;
static SDL_Surface *jackJump = NULL;
static SDL_Surface *bullet = NULL;
static SDL_Surface *hud = NULL;
static SDL_Surface *object1 = NULL;

//Text
static SDL_Surface *seconds = NULL;
static SDL_Surface *healthHUD = NULL;

//Text color
static SDL_Color textColor = {155, 0, 0};

//Mouse cursor trackers
static int xMouse = 0;
static int yMouse = 0;

//Text messages
static SDL_Surface *message = NULL;

//Music
static Mix_Music *music = NULL;

//Sounds

//Event structure
static SDL_Event event;

//Font
static TTF_Font *font = NULL;

//Jack's sprites
static SDL_Rect clipsRight[6];
static SDL_Rect clipsLeft[6];
static SDL_Rect clipsJumpRightStart[4];
static SDL_Rect clipsJumpRightEnd[4];
static SDL_Rect clipsJumpLeftStart[4];
static SDL_Rect clipsJumpLeftEnd[4];

//Button clips
static SDL_Rect clips[4];

//Button hitbox
static SDL_Rect start_box;