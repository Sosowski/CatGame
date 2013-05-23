/*CATGAME - Because there exists a parasite that alters your perception of cats.

CURRENT NOTABLE BUGS
-Mix_PlayChannel(-1, irrelevent, irrelevent) returns -1 like there was an error, but there isn't one. -1 Is supposed to select first available
channel, so this is a feature we will need to have.

TO DO MORE ACCURATE ERROR HANDLING
#include Windows.h, MessageBox(NULL, "MessageBox Text", "MessageBox caption", MB_OK);
Throw it into the error check statement you think is screwing up before the return

FOR DEBUG
	//Convert to string
	std::stringstream caption;

	//Generate string
	caption << "test " << variable << " ";

	//Set caption
	SDL_WM_SetCaption( caption.str().c_str(), NULL);

*/

//TODO NEXT
/*Fix animations, Make sure standing frame doesn't pop up in the middle of jumping

*/

#include "Library.h"

//call for screen, currently declared in Window.cpp
extern SDL_Surface *screen;

SDL_Rect plat1 = { 0, 500, 500, 100 };
SDL_Rect plat2 = { 1000, 950, 300, 100 };
SDL_Rect plat3 = { 500, 1500, 700, 50 };

//FUNCTIONS------------------------------------------------------------------------------------------------

//Load / optimise images before displaying, create pointer to optimized image
SDL_Surface *load_image( std::string filename)
{
	//Temporary storage for image
	SDL_Surface* loadedImage = NULL;

	//Optimised image
	SDL_Surface* optimizedImage = NULL;

	//Load image
	//Supported : bmp, pnm, xpm, pcx, gif, jpeg, tga, png
	loadedImage = IMG_Load ( filename.c_str() );

	//Check for errors
	if( loadedImage != NULL )
	{
		//Create optimised image
		optimizedImage = SDL_DisplayFormatAlpha ( loadedImage );

		//Free old image
		SDL_FreeSurface ( loadedImage );

		//These checks should hopefully be self explanitory, so I'll only comment if it's purpose isn't clear
		if( optimizedImage != NULL )
		{
			//The following two statements handle color keying, if nessisary

			//Map the color key
			//Uint32 colorkey = SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF);

			//Render pixels for color specified above transparent
			//SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, colorkey);
		}

		//send optimised image
		return optimizedImage;
	}
}

//Apply an image onto screen
void apply_surface ( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
	//Temporary rectangle to hold offsets
	SDL_Rect offset;

	//Give offsets to rectangle
	offset.x = x;
	offset.y = y;

	//Apply surface to screen (Blit)
	SDL_BlitSurface( source, clip, destination, &offset );
}

//General collision detection function
//Send hitboxes here to compare

//Runs initilization functions
bool init()
{
	//Enable SDL subsystems
	if( SDL_Init ( SDL_INIT_EVERYTHING ) == -1)
	{
		return false;
	}

	//Enable TTF text
	if( TTF_Init() == -1)
	{
		return false;
	}

	//Enable Audio mixer
	//Frequency, Format, Audio channels, sample size
	//Mix_Init() Mix_Quit() to initialize and close anything other than WAV
	if ( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		return false;
	}

	//If everything set up
	return true;
}

bool load_files()
{
	//Load images
	//example = load_image( "example.png" );
	background = load_image( "bg.png" );
	jackRun = load_image( "JackRun.png" );
	jackJump = load_image( "JackJump.png" );
	cursor = load_image( "cursor.bmp" );

	//Open font
	//font = TTF_OpenFont( "example.ttf", 28);

	if ( background == NULL || jackRun == NULL || jackJump == NULL || cursor == NULL)
	{
		return false;
	}
	

	//Load music
	//music = Mix_LoadMUS ( "Example.wav" );

	/*if (music == NULL)
	{
		return false;
	}
	*/

	//Load sounds
	//sound = Mix_LoadWAV( "example.wav" );

	//If statement error check. You get the idea by now.

	//else
	return true;
}

//Remove all objects and close program
void clean_up()
{
	//Free images
	//SDL_FreeSurface ( example );
	SDL_FreeSurface ( background );
	SDL_FreeSurface ( jackRun );
	SDL_FreeSurface ( jackJump );
	SDL_FreeSurface ( cursor );

	//Free sounds
	//Mix_FreeChunk ( example );

	//Free Music
	//Mix_FreeMusic ( example );

	//Close font
	//TTF_CloseFont ( example );

	//Quit audio mixer
	Mix_CloseAudio();

	//Close TTF
	TTF_Quit();

	//Exit program
	SDL_Quit();
}

//CLASSES------------------------------------------------------------------------------------------
void set_clips()
{
	//Clip Sprites
	//Right movement
	clipsRight[0].x = 0;
	clipsRight[0].y = 0;
	clipsRight[0].w = JACK_WIDTH;
	clipsRight[0].h = JACK_HEIGHT;

	clipsRight[1].x = 0;
	clipsRight[1].y = JACK_HEIGHT;
	clipsRight[1].w = JACK_WIDTH;
	clipsRight[1].h = JACK_HEIGHT;

	clipsRight[2].x = 0;
	clipsRight[2].y = JACK_HEIGHT * 2;
	clipsRight[2].w = JACK_WIDTH;
	clipsRight[2].h = JACK_HEIGHT;

	clipsRight[3].x = 0;
	clipsRight[3].y = JACK_HEIGHT * 3;
	clipsRight[3].w = JACK_WIDTH;
	clipsRight[3].h = JACK_HEIGHT;

	clipsRight[4].x = 0;
	clipsRight[4].y = JACK_HEIGHT * 4;
	clipsRight[4].w = JACK_WIDTH;
	clipsRight[4].h = JACK_HEIGHT;

	clipsRight[5].x = 0;
	clipsRight[5].y = JACK_HEIGHT * 5;
	clipsRight[5].w = JACK_WIDTH;
	clipsRight[5].h = JACK_HEIGHT;

	//Left Movement
	clipsLeft[0].x = JACK_WIDTH;
	clipsLeft[0].y = 0;
	clipsLeft[0].w = JACK_WIDTH;
	clipsLeft[0].h = JACK_HEIGHT;

	clipsLeft[1].x = JACK_WIDTH;
	clipsLeft[1].y = JACK_HEIGHT;
	clipsLeft[1].w = JACK_WIDTH;
	clipsLeft[1].h = JACK_HEIGHT;

	clipsLeft[2].x = JACK_WIDTH;
	clipsLeft[2].y = JACK_HEIGHT * 2;
	clipsLeft[2].w = JACK_WIDTH;
	clipsLeft[2].h = JACK_HEIGHT;

	clipsLeft[3].x = JACK_WIDTH;
	clipsLeft[3].y = JACK_HEIGHT * 3;
	clipsLeft[3].w = JACK_WIDTH;
	clipsLeft[3].h = JACK_HEIGHT;

	clipsLeft[4].x = JACK_WIDTH;
	clipsLeft[4].y = JACK_HEIGHT * 4;
	clipsLeft[4].w = JACK_WIDTH;
	clipsLeft[4].h = JACK_HEIGHT;

	clipsLeft[5].x = JACK_WIDTH;
	clipsLeft[5].y = JACK_HEIGHT * 5;
	clipsLeft[5].w = JACK_WIDTH;
	clipsLeft[5].h = JACK_HEIGHT;

	//Right jump
	clipsJumpRightStart[0].x = 0;
	clipsJumpRightStart[0].y = 0;
	clipsJumpRightStart[0].w = JACK_WIDTH;
	clipsJumpRightStart[0].h = JACK_HEIGHT;

	clipsJumpRightStart[1].x = JACK_WIDTH;
	clipsJumpRightStart[1].y = 0;
	clipsJumpRightStart[1].w = JACK_WIDTH;
	clipsJumpRightStart[1].h = JACK_HEIGHT;

	clipsJumpRightStart[2].x = JACK_WIDTH * 2;
	clipsJumpRightStart[2].y = 0;
	clipsJumpRightStart[2].w = JACK_WIDTH;
	clipsJumpRightStart[2].h = JACK_HEIGHT;

	clipsJumpRightStart[3].x = JACK_WIDTH * 3;
	clipsJumpRightStart[3].y = 0;
	clipsJumpRightStart[3].w = JACK_WIDTH;
	clipsJumpRightStart[3].h = JACK_HEIGHT;

	//Right fall
	clipsJumpRightEnd[0].x = JACK_WIDTH * 4;
	clipsJumpRightEnd[0].y = 0;
	clipsJumpRightEnd[0].w = JACK_WIDTH;
	clipsJumpRightEnd[0].h = JACK_HEIGHT;

	clipsJumpRightEnd[1].x = JACK_WIDTH * 5;
	clipsJumpRightEnd[1].y = 0;
	clipsJumpRightEnd[1].w = JACK_WIDTH;
	clipsJumpRightEnd[1].h = JACK_HEIGHT;

	clipsJumpRightEnd[2].x = JACK_WIDTH * 6;
	clipsJumpRightEnd[2].y = 0;
	clipsJumpRightEnd[2].w = JACK_WIDTH;
	clipsJumpRightEnd[2].h = JACK_HEIGHT;

	clipsJumpRightEnd[3].x = JACK_WIDTH * 7;
	clipsJumpRightEnd[3].y = 0;
	clipsJumpRightEnd[3].w = JACK_WIDTH;
	clipsJumpRightEnd[3].h = JACK_HEIGHT;

	//Left jump
	clipsJumpLeftStart[0].x = JACK_WIDTH * 7;
	clipsJumpLeftStart[0].y = JACK_HEIGHT;
	clipsJumpLeftStart[0].w = JACK_WIDTH;
	clipsJumpLeftStart[0].h = JACK_HEIGHT;

	clipsJumpLeftStart[1].x = JACK_WIDTH * 6;
	clipsJumpLeftStart[1].y = JACK_HEIGHT;
	clipsJumpLeftStart[1].w = JACK_WIDTH;
	clipsJumpLeftStart[1].h = JACK_HEIGHT;

	clipsJumpLeftStart[2].x = JACK_WIDTH * 5;
	clipsJumpLeftStart[2].y = JACK_HEIGHT;
	clipsJumpLeftStart[2].w = JACK_WIDTH;
	clipsJumpLeftStart[2].h = JACK_HEIGHT;

	clipsJumpLeftStart[3].x = JACK_WIDTH * 4;
	clipsJumpLeftStart[3].y = JACK_HEIGHT;
	clipsJumpLeftStart[3].w = JACK_WIDTH;
	clipsJumpLeftStart[3].h = JACK_HEIGHT;

	//Left fall
	clipsJumpLeftEnd[0].x = JACK_WIDTH * 3;
	clipsJumpLeftEnd[0].y = JACK_HEIGHT;
	clipsJumpLeftEnd[0].w = JACK_WIDTH;
	clipsJumpLeftEnd[0].h = JACK_HEIGHT;

	clipsJumpLeftEnd[1].x = JACK_WIDTH * 2;
	clipsJumpLeftEnd[1].y = JACK_HEIGHT;
	clipsJumpLeftEnd[1].w = JACK_WIDTH;
	clipsJumpLeftEnd[1].h = JACK_HEIGHT;

	clipsJumpLeftEnd[2].x = JACK_WIDTH;
	clipsJumpLeftEnd[2].y = JACK_HEIGHT;
	clipsJumpLeftEnd[2].w = JACK_WIDTH;
	clipsJumpLeftEnd[2].h = JACK_HEIGHT;

	clipsJumpLeftEnd[3].x = 0;
	clipsJumpLeftEnd[3].y = JACK_HEIGHT;
	clipsJumpLeftEnd[3].w = JACK_WIDTH;
	clipsJumpLeftEnd[3].h = JACK_HEIGHT;
}

Jack::Jack()
{
	//Initialize offsets
	x = 0;
	y = 0;

	//Initialize velocity
	xVel = 0;
	yVel = 0.5;

	//Create Jack's hitbox
	box.x = x;
	box.y = y;
	box.w = JACK_WIDTH;
	box.h = JACK_HEIGHT;

	//Create probe hitbox
	probe.w = JACK_WIDTH;
	probe.h = JACK_HEIGHT + 1;

	//Animation variables
	frame = 0;
	delay = 0;
	status = JACK_RIGHT;
	facing = JACK_RIGHT;
}

bool check_collision( SDL_Rect A, SDL_Rect B)
{
	//Rectangle sides
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	//Calculate sides
	leftA = A.x;
	rightA = A.x + A.w;
	topA = A.y;
	bottomA = A.y + A.h;

	//Calculate the sides of rect B
	leftB = B.x;
	rightB = B.x + B.w;
	topB = B.y;
	bottomB = B.y + B.h;

	//If any of the sides from A are outside of B
	if( bottomA <= topB )
	{
		return false;
	}
	if ( topA >= bottomB )
	{
		return false;
	}
	if( rightA <= leftB )
	{
		return false;
	}

	if( leftA >= rightB )
	{
		return false;
	}
	//If none of the sides from A are outside B
	return true;
}

void Jack::move()
{

	//Convert to string
	std::stringstream caption;

	//Generate string
	caption << "yvel " << yVel << " x " << x;

	//Set caption
	SDL_WM_SetCaption( caption.str().c_str(), NULL);

	//Move the square left or right
	x += xVel;

	//If the square went too far to the left or right or has collided with the wall
	if( ( x < 0 ) || ( x + JACK_WIDTH > LEVEL_WIDTH ) )
	{ 
		//Move back
		x -= xVel; 
	} 

	//Move the square up or down
	if(yVel != 0){
		yVel += 0.5;
	}
	y += yVel;

	//If the square went too far up or down or has collided with the wall
	if( ( y < 0 ) || ( y + JACK_HEIGHT > LEVEL_HEIGHT ) )
	{
		//Move back
		y -= yVel;
		yVel = 0;

	} 

}

void Jack::show()
{
	//If moving left
	if( xVel < 0)
	{
		//Set animation left
		lastStatus = status;
		status = JACK_LEFT;

		//Set facing flag
		facing = JACK_LEFT;

		if( yVel < 0){
			lastStatus = status;
			status = JACK_LEFT_JUMP;
		}

		if( yVel > 0){
			lastStatus = status;
			status = JACK_LEFT_FALL;
		}
		//Increase animation delay counter
		delay++;
		
		if( (status == JACK_LEFT_JUMP) || (status == JACK_LEFT_FALL) ){
			
			//Hold 4th frame
			if ( frame == 4 ) {
				//Reset delay for other statements
				delay = 0;
			}
			//Otherwise, increment
			else if ( delay == 10 ){
				//Reset delay
				delay = 0;
				
				//Move to next frame
				frame++;
			}
		}

		else if( delay == 10 ){
			//Reset delay
			delay = 0;

			//Move to next frame
			frame++;
		}

		//Done jumping, reset frame.
		if ((lastStatus == JACK_LEFT_JUMP) || (lastStatus == JACK_LEFT_FALL))
		{
			frame = 0;
		}

	}
	//If moving right
	else if( xVel > 0)
	{
		//Set animation right
		lastStatus = status;
		status = JACK_RIGHT;

		//Set facing flag
		facing = JACK_RIGHT;

		if( yVel < 0){
			lastStatus = status;
			status = JACK_RIGHT_JUMP;
		}

		if( yVel > 0){
			lastStatus = status;
			status = JACK_RIGHT_FALL;
		}

		//Increase delay
		delay++;

		if( (status == JACK_RIGHT_JUMP) || (status == JACK_RIGHT_FALL) ){
			
			//Hold 4th frame
			if ( frame == 4 ) {
				//Reset delay for other statements
				delay = 0;
			}
			//Otherwise, increment
			else if ( delay == 10 ){
				//Reset delay
				delay = 0;
				
				//Move to next frame
				frame++;
			}
		}

		else if( delay == 10 ){
			//Reset delay
			delay = 0;

			//Move to next frame
			frame++;
		}

		//Done jumping, reset frame.
		if ((lastStatus == JACK_RIGHT_JUMP) || (lastStatus == JACK_RIGHT_FALL))
		{
			frame = 0;
		}
	}
	//If falling with no x movement
	else if( (xVel == 0) && (yVel > 0) )
	{
		//If last status was jumping, reset the frame
		if( (lastStatus == JACK_RIGHT_JUMP) || (lastStatus == JACK_LEFT_JUMP))
		{
			frame = 0;
		}

		if( facing == JACK_RIGHT )
		{
			status = JACK_RIGHT_FALL;
			lastStatus = status;
		}
		else if( facing == JACK_LEFT)
		{
			status = JACK_LEFT_FALL;
			lastStatus = status;
		}

		//Increase delay
		delay++;

		if( (status == JACK_RIGHT_FALL) || (status == JACK_LEFT_FALL) ){
			
			//Hold 4th frame
			if ( frame == 4 ) {
				//Reset delay for other statements
				delay = 0;
			}
			//Otherwise, increment
			else if ( delay == 10 ){
				//Reset delay
				delay = 0;
				
				//Move to next frame
				frame++;
			}
		}

		//Done jumping, reset frame.
		if ( yVel == 0 )
		{
			//Reset frame
			frame = 0;
		}
	}

	//If jumping with no x movement
	else if( (xVel == 0) && (yVel < 0) )
	{
		if( facing == JACK_RIGHT )
		{
			lastStatus = status;
			status = JACK_RIGHT_JUMP;
		}
		else if( facing == JACK_LEFT)
		{
			lastStatus = status;
			status = JACK_LEFT_JUMP;
		}

		//Increase delay
		delay++;
		if( (status == JACK_RIGHT_JUMP) || (status == JACK_LEFT_JUMP) )
		{
			
			//Hold 4th frame
			if ( frame == 4 ) {
				//Reset delay for other statements
				delay = 0;
			}
			//Otherwise, increment
			else if ( delay == 10 ){
				//Reset delay
				delay = 0;
				
				//Move to next frame
				frame++;
			}
		}
	}

	//If standing
	else if( (xVel == 0) && (yVel == 0) )
	{
		//TODO: An actual standing sprite
		//Set frame to standing
		if( facing == JACK_RIGHT ) 
		{
			status = JACK_RIGHT;
		}
		if( facing == JACK_LEFT )
		{
			status = JACK_LEFT;
		}
		frame = 0;
	}
	//Loop animation
	if( frame >= 6)
	{
		frame = 0;
	}
	//Show Jack
	if( status == JACK_RIGHT )
	{
		apply_surface( x - camera.x, y - camera.y, jackRun, screen, &clipsRight[ frame ] );
	}
	else if( status == JACK_LEFT )
	{
		apply_surface( x - camera.x, y - camera.y, jackRun, screen, &clipsLeft[ frame ] );
	}
	else if( status == JACK_RIGHT_JUMP )
	{
		apply_surface( x - camera.x, y - camera.y, jackJump, screen, &clipsJumpRightStart[ frame ] );
	}
	else if( status == JACK_LEFT_JUMP )
	{
		apply_surface( x - camera.x, y - camera.y, jackJump, screen, &clipsJumpLeftStart[ frame ] );
	}
	else if( status == JACK_RIGHT_FALL )
	{
		apply_surface( x - camera.x, y - camera.y, jackJump, screen, &clipsJumpRightEnd[ frame ] );
	}
	else if( status == JACK_LEFT_FALL )
	{
		apply_surface( x - camera.x, y - camera.y, jackJump, screen, &clipsJumpLeftEnd[ frame ] );
	}
}

void Jack::handle_events()
{
	//Check for keypress
	if( event.type == SDL_KEYDOWN )
	{

		//Set velocity
		switch( event.key.keysym.sym )
		{
		case SDLK_d: xVel += JACK_WIDTH / 50; break;
		case SDLK_a: xVel -= JACK_WIDTH / 50; break;
		case SDLK_w:
			if( (yVel < 0) || (yVel > 0)){
				break;
			}
			yVel -= 20.1;
			break;	
		}
	}
	//Check for key release
	else if( event.type == SDL_KEYUP )
	{
		//Set velocity
		switch( event.key.keysym.sym )
		{
		case SDLK_d: xVel -= JACK_WIDTH / 50; break;
		case SDLK_a: xVel += JACK_WIDTH / 50; break;
		}
	}
}

void Jack::set_camera()
{
	//Center camera over Jack
	camera.x = ( x + JACK_WIDTH / 2 ) - SCREEN_WIDTH / 2;
	camera.y = ( y + JACK_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

	//Keep camera in bounds
	if( camera.x < 0 )
	{
		camera.x = 0;
	}
	if( camera.y < 0 )
	{
		camera.y = 0;
	}
	if( camera.x > LEVEL_WIDTH - camera.w )
	{
		camera.x = LEVEL_WIDTH - camera.w;
	}
	if( camera.y > LEVEL_HEIGHT - camera.h )
	{
		camera.y = LEVEL_HEIGHT - camera.h;
	}
}

void Jack::shift_boxes()
{
	//Shift Jack's hitbox
	box.x = x;
	box.y = y;
}

//MAIN FUNCTION-----------------------------------------------------------------------------------------------------

int main( int argc, char* args[])
{
	//Set a flag to keep the window on screen
	bool quit = false;

	//Enable packages
	if( init() == false)
	{
		return 1;
	}

	//Create window
	Window myWindow;

	//Error check window
	if( myWindow.error() == true)
	{
		return 1;
	}

	//Load files
	if( load_files() == false)
	{
		return 1;
	}

	//Hide the windows cursor
	SDL_ShowCursor(0);

	//Create FPS timer essentials
	int frame = 0;
	Timer fps;

	//Clip sprite sheet
	set_clips();

	//Create Jack Sprite
	Jack walk;

	//Begin loop to hold screen open
	while( quit == false )
	{
		//Start the fps timer
		fps.start();

		//Event handler
		while( SDL_PollEvent( & event ) )
		{
			//Handle Jack
			walk.handle_events();

			//Handle window events
			myWindow.handle_events();

			//Exit if window closed
			if( event.type == SDL_QUIT )
			{
				quit = true;
			}
		}

		//Check for window error
		if( myWindow.error() == true)
		{
			return 1;
		}

		//Get current mouse position
		SDL_GetMouseState(&xMouse, &yMouse);

		//Place custom cursor over mouse
		apply_surface( xMouse, yMouse, cursor, screen);

		//Jack movement
		walk.move();

		//Set camera
		walk.set_camera();

		//Show background
		apply_surface( 0, 0, background, screen, &camera);

		//Show platforms

		//Show Jack
		walk.show();

		//Get keystate
		Uint8 *keystates = SDL_GetKeyState ( NULL );

		//Update Screen
		if( SDL_Flip ( screen ) == -1 )
		{
			return 1;
		}

		//Increment frame counter
		frame++;

		//Frame rate cap
		if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
		{
			//Sleep remaining frame time
			SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
		}
	}

	//If done, clean up assets and quit
	clean_up();

	return 0;
}