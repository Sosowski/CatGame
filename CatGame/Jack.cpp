//#include "Jack.h"
//#include "Global.h"
#include "Library.h"

Jack::Jack()
{
	// ---- Initialize variables, set flags ----
	//X and Y offsets
	x = 0;
	y = 0;
	yLast = 0;
	//Velocity
	xVel = 0;
	yVel = 0.5;

	//Create Jack's hitbox
	box.x = x + (JACK_WIDTH / 4);
	box.y = y;
	box.w = 150;
	box.h = JACK_HEIGHT;

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

	//Platform hitboxes (Might not be needed later)
	plat1.x = 0;
	plat1.y = 750;
	plat1.w = 400;
	plat1.h = 100;

	plat2.x = 750;
	plat2.y = 1000;
	plat2.w = 400;
	plat2.h = 100;

	// ---- Load Files ---- 
	load_files();

	// ---- Clip sprite sheet ----
	set_clips();
}

bool Jack::load_files()
{
	//Load images For Jacks Sprite
	jackRun = load_image( "JackRun.png" );
	jackJump = load_image( "JackJump.png" );

	//Open font
	//font = TTF_OpenFont( "example.ttf", 28);

	if ( jackRun == NULL || jackJump == NULL)
	{
		return false;
	}
	
	//Extra Stuff for later?
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

void Jack::move()
{
	//Convert to string
	std::stringstream caption;

	//Generate string
	//caption << "yvel: " << yVel << " xvel: " << xVel << " x pos: " << x << " y pos: " << y << " onground " << onGround;
	caption << "probe.x " << probe.x << " probe.y " << probe.y << " clipsRight[2].y " << clipsRight[2].y << " camera x: " << camera.x << " camera y: " << camera.y << " Status: " << status;

	//Set caption
	SDL_WM_SetCaption( caption.str().c_str(), NULL);

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

void Jack::set_camera()
{
	//Center camera over Jack
	camera.x = ( x + JACK_WIDTH / 2 ) - SCREEN_WIDTH / 2;
	camera.y = ( y + JACK_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

	//When the player presses S, the camera will shift down slightly.
	//if( Down == true )
	//{
	//	camera.y = (( y + JACK_HEIGHT / 2 ) - SCREEN_HEIGHT / 2) + 100;
	//}

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

void Jack::set_clips()
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

// ---- Functions to control Jack -------------------------------------------------------------------
void Jack::accel(int xAccel, int yAccel)
{
	xVel += xAccel;
	yVel += yAccel;
}

void Jack::stop()
{
	xVel = 0;
}

float Jack::Read(int val)
{
	float ret;
	switch(val)
	{
	case 0:  ret = x; break;
	case 1:  ret = y; break;
	case 2:  ret = xVel; break;
	case 3:  ret = yVel; break;
	default: ret = x; break;
	}

	return ret;
}

/*
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
*/