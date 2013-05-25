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
	plat1 = load_image( "plat1.png" );

	//Open font
	//font = TTF_OpenFont( "example.ttf", 28);

	if ( background == NULL || jackRun == NULL || jackJump == NULL || cursor == NULL || plat1 == NULL)
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
	SDL_FreeSurface ( plat1 );

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
	for(int j=0; j<6; j++){
		clipsRight[j].x = 0;
		clipsRight[j].y = JACK_HEIGHT * j;
		clipsRight[j].w = JACK_WIDTH;
		clipsRight[j].h = JACK_HEIGHT;
	}

	//Left Movement
	for(int j=0; j<6; j++){
		clipsLeft[j].x = JACK_WIDTH;
		clipsLeft[j].y = JACK_HEIGHT * j;
		clipsLeft[j].w = JACK_WIDTH;
		clipsLeft[j].h = JACK_HEIGHT;
	}

	//Right jump
	for(int j=0; j<4; j++){
		clipsJumpRightStart[j].x = JACK_WIDTH * j;
		clipsJumpRightStart[j].y = 0;
		clipsJumpRightStart[j].w = JACK_WIDTH;
		clipsJumpRightStart[j].h = JACK_HEIGHT;
	}

	//Right fall
	for(int j=0; j<4; j++){
		clipsJumpRightEnd[j].x = JACK_WIDTH * (4+j);
		clipsJumpRightEnd[j].y = 0;
		clipsJumpRightEnd[j].w = JACK_WIDTH;
		clipsJumpRightEnd[j].h = JACK_HEIGHT;
	}

	//Left jump
	for(int j=0; j<4; j++){
		clipsJumpLeftStart[j].x = JACK_WIDTH * (7-j);
		clipsJumpLeftStart[j].y = JACK_HEIGHT;
		clipsJumpLeftStart[j].w = JACK_WIDTH;
		clipsJumpLeftStart[j].h = JACK_HEIGHT;
	}

	//Left fall
	for(int j=0; j<4; j++){
		clipsJumpLeftEnd[j].x = JACK_WIDTH * (3-j);
		clipsJumpLeftEnd[j].y = JACK_HEIGHT;
		clipsJumpLeftEnd[j].w = JACK_WIDTH;
		clipsJumpLeftEnd[j].h = JACK_HEIGHT;
	}
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
	box.x = x + (JACK_WIDTH / 4);
	box.y = y;
	box.w = 150;
	box.h = JACK_HEIGHT;

	//Platform hitboxes
	plat1.x = 0;
	plat1.y = 750;
	plat1.w = 400;
	plat1.h = 100;

	plat2.x = 750;
	plat2.y = 1000;
	plat2.w = 400;
	plat2.h = 100;

	//Create probe hitbox
	probe.x = x + (JACK_WIDTH / 2);
	probe.y = y + (JACK_HEIGHT + 1);
	probe.w = 1;
	probe.h = 1;

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

	//Left/Right Movement
	x += xVel;

	//Call this to update Jack's hitbox after all movements
	shift_boxes();

	//Check if Jack left the area or walked into a platform
	if( ( x < 0 ) || ( x + JACK_WIDTH > LEVEL_WIDTH ) || (check_collision( box, plat1 ) ) || (check_collision( box, plat2 ) ) )
	{ 
		//Move back
		x -= xVel; 
		shift_boxes();
	} 

	//The probe isn't colliding with anything. We need to fall.
	if( ( y + JACK_HEIGHT > LEVEL_HEIGHT == false ) || (check_collision( probe, plat1 ) == false ) )
	{
		//Apply gravity
		yVel += 0.5;
		shift_boxes();
		onGround = false;
	}

	//Up/Down Movement
	y += yVel;

	shift_boxes();

	//Check if Jack left the area or hit a platform
	if( onGround == false ){
		if( ( y < 0 ) || (check_collision( box, plat1 ) ) || (check_collision( box, plat2 ) ) )
		{
			y -= yVel;
			yVel = 0;
			shift_boxes();
			onGround = true;
		}
	}

	if( ( y + JACK_HEIGHT > LEVEL_HEIGHT ) ){
		//Player fell. Reset to start
		yVel = 0;
		onGround = false;
		x = 0;
		y = 0;
		shift_boxes();

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

	//When the player presses S, the camera will shift down slightly.
	if( event.type == SDL_KEYDOWN )
	{
		//Set velocity
		switch( event.key.keysym.sym )
		{
		case SDLK_s: camera.y = (( y + JACK_HEIGHT / 2 ) - SCREEN_HEIGHT / 2) + 100;
		}
	}

	else if( event.type == SDL_KEYUP )
	{
		//Set velocity
		switch( event.key.keysym.sym )
		{
		case SDLK_s: camera.y = ( y + JACK_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;
		}
	}

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
	box.x = x + (JACK_WIDTH / 4);
	box.y = y;

	//Shift the probe's hitbox
	probe.x = x + (JACK_WIDTH / 2);
	probe.y = y + (JACK_HEIGHT + 1);
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

	//Create and set custom cursor
	Uint8 mask[] = {0x7F, 0xFF, 0x3F, 0xFE, 0x1F, 0xFC, 0x0F, 0xF8, 0x07, 0xF0, 0x03, 0xE0, 0x01, 0xC0, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0xc0, 0x03, 0xE0, 0x07, 0xF0, 0x0F, 0xF8, 0x1F, 0xFC, 0x3F, 0xFE, 0x7F, 0xFF };
    Uint8 data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    SDL_Cursor* cursor = SDL_CreateCursor(data, mask, 16, 24, 8, 0);
    SDL_SetCursor (cursor);

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

		//Jack movement
		walk.move();

		//Set camera
		walk.set_camera();

		//Show background
		apply_surface( 0, 0, background, screen, &camera);

		//Show platforms
		apply_surface( 0, 750, plat1, background, &camera);
		apply_surface( 750, 1000, plat1, background, &camera);

		//Show Jack
		walk.show();

		//Place custom cursor over mouse
		//apply_surface( xMouse, yMouse, cursor, background);

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