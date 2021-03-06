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
	//Flags
	onGround = false;
	onLeft = false;
	onRight = false;
	left = false;
	right = false;
	knockbackX = 0;

	//For Jacks Camera Offset
	xOffset = 0;
	yOffset = 0;
	EaseIn = 0, EaseOut = 0, xCamVel = 0, yCamVel = 0;

	//Create Jack's hitbox
	box.x = (Sint16)(x + (JACK_WIDTH / 4));
	box.y = (Sint16)(y);
	box.w = 150;
	box.h = JACK_HEIGHT;

	//Create feet hitbox
	feet.x = (Sint16)(x + (JACK_WIDTH / 2));
	feet.y = (Sint16)(y + (JACK_HEIGHT + 1));
	feet.w = 1;
	feet.h = 1;

	//Initate Pointer to Platform jack is standing on
	dummyPlat.x = 0;
	dummyPlat.y = 0;
	dummyPlat.w = 0;
	dummyPlat.h = 0;
	standingOn = &dummyPlat;
	touchSide = &dummyPlat;

	//Animation variables
	frame = 0;
	delay = 0;
	status = JACK_RIGHT;
	facing = JACK_RIGHT;

	// ---- Load Files ----
	load_files();

	// ---- Clip sprite sheet ----
	set_clips();
}

bool Jack::load_files()
{
	//Load images For Jacks Sprite
	jackRun = load_image( "Images/JackRun.png" );
	jackJump = load_image( "Images/JackJump.png" );

	if ( jackRun == NULL || jackJump == NULL)
	{
		return false;
	}

	return true;
}

void Jack::move()
{
	//An X movement key is depressed, but there is no movement.
	if(left == true && xVel == 0 && knockbackX == 0){
		 xVel -= 10;
	}

	if(right == true && xVel == 0 && knockbackX == 0){
		xVel += 10;
	}

	if(right == false && left == false && knockbackX == 0){
		xVel = 0;
	}

	//If knockback active, start correcting it.
	if( knockbackX != 0 )
	{
		xVel += 0.5;
		knockbackX -= 0.5;
	}

	//If Side flags are set, call to check if they must be reset
	if( onRight == true ){
		if( touchSide->y > (box.y+(box.h*.75)) || box.y > touchSide->y+touchSide->h ){	
			onRight = false;
		}
	}
	else if( onLeft == true ){
		if( touchSide->y > (box.y+(box.h*.75)) || box.y > touchSide->y+touchSide->h ){	
			onLeft = false;
		}
	}
	//Horizontal Movement
	//Apply velocity if not touching sides in that direction
	if(onRight == false && xVel > 0){				//travel right
		x += (int)xVel;
		onLeft = false;				//unset the flag for side collide
		touchSide = &dummyPlat;
	}
	else if(onLeft == false && xVel < 0){			//travel left
		x += (int)xVel;
		onRight = false;			//unset the flag for side collide
		touchSide = &dummyPlat;
	}

	//Check if Jack left the area
	if( ( x < 0 ) || ( x + JACK_WIDTH > LEVEL_WIDTH ) )
	{
		//Move back
		x -= (int)xVel;

		//Stop horizontal knockback
		if( knockbackX != 0)
		{
			knockbackX = 0;
			xVel = 0;
		}
	}

	//Up/Down Movement
	y += (int)yVel;

	//Final checks to clear the onGround flag before gravity is applied
	if( onGround = true ){
		if( yVel < 0 ) {
			onGround = false;
			standingOn = &dummyPlat;
		}
		else if( (feet.x+5 < standingOn->x) || (feet.x-5 > standingOn->x+standingOn->w) ){
			onGround = false;
			standingOn = &dummyPlat;
		}
	}

	//Apply gravity if not on ground
	if( ( y + JACK_HEIGHT > LEVEL_HEIGHT == false ) && (onGround == false) )
	{
		//Apply gravity
		yVel += 0.5;
	}

	if( ( y + JACK_HEIGHT > LEVEL_HEIGHT ) )
	{
		//Player fell. Reset to start
		player_health = 10;
		xVel = 0;
		yVel = 0;
		x = 0;
		y = 0;
		knockbackX = 0;
	}

	//Update Jack's hitbox after all movements
	shift_boxes();
}

void Jack::show(Window& aWindow)
{
	lastStatus = status; // Record Previous status

	// ---- SET STATUS ---- //
	//If moving left
	if( xVel < 0)
	{
		//Set animation left
		status = JACK_LEFT;

		//Set facing flag
		facing = JACK_LEFT;

		if( yVel <= 0 && onGround == false)
		{
			status = JACK_LEFT_JUMP;
		}
		if( yVel > 0 && onGround == false)
		{
			status = JACK_LEFT_FALL;
		}
	}

	//If moving right
	else if( xVel > 0)
	{
		//Set animation right
		status = JACK_RIGHT;

		//Set facing flag
		facing = JACK_RIGHT;

		if( yVel <= 0 && onGround == false)
		{
			status = JACK_RIGHT_JUMP;
		}

		if( yVel > 0 && onGround == false)
		{
			status = JACK_RIGHT_FALL;
		}
	}

	//If no x movement
	else if(xVel == 0)
	{
		if(onGround == false)	// if in air
		{
			if(facing == JACK_LEFT)
			{
				if(yVel <= 0)
				{
					status = JACK_LEFT_JUMP;
				}
				else if (yVel > 0)
				{
					status = JACK_LEFT_FALL;
				}
			}
			else if(facing == JACK_RIGHT)
			{
				if(yVel <= 0)
				{
					status = JACK_RIGHT_JUMP;
				}
				else if (yVel > 0)
				{
					status = JACK_RIGHT_FALL;
				}
			}
		}
		else	//if on ground
		{
			//show a standing frame for facing left and facing right.
			if( facing == JACK_RIGHT )
			{
				status = JACK_RIGHT;
				delay = 0;	// should reset delay so the first running frame is held.
			}

			else if( facing == JACK_LEFT)
			{
				status = JACK_LEFT;
				delay = 0;	// should reset delay so the first running frame is held.
			}
		}
	}

	// ---- ADVANCE FRAME ---- //

	//Increase delay
	delay++;

	//Reset to starting frame if changing animation.
	if(lastStatus != status)
	{
		frame = 0;
	}

	//If jumping
	if( onGround == false) 
	{
		//Hold 4th frame
		if ( frame >= 3 ) 
		{
			//Reset delay To hold frame
			delay = 0;
		}
	}
	if ( delay >= 10 )
	{
		//Reset delay
		delay = 0;
		//Move to next frame
		frame++;
	}

	//Loop animation
	if( frame >= 6)
	{
		frame = 0;
	}

// ---- SHOW PROPER FRAME ---- //
	switch(status) //The plan is that camera will now not be global, but everything will have its own camera.
	{				//Then the main camera is The camera of the window. Meaning this Will have to change apply_surface.
					//The new apply surface will remove the camera.x and such here, because it will be in the function instead. 
					//The camera in the Window class is used, so this class has camera values, but the Window's values apply what is seen.
	case JACK_RIGHT:
		aWindow.apply_surface( (int)x, (int)y, jackRun, 0, &clipsRight[ frame ] );
		break;
	case JACK_LEFT:
		aWindow.apply_surface( (int)x, (int)y, jackRun, 0, &clipsLeft[ frame ] );
		break;
	case JACK_RIGHT_JUMP:
		aWindow.apply_surface( (int)x, (int)y, jackJump, 0, &clipsJumpRightStart[ frame ] );
		break;
	case JACK_LEFT_JUMP:
		aWindow.apply_surface( (int)x, (int)y, jackJump, 0, &clipsJumpLeftStart[ frame ] );
		break;
	case JACK_RIGHT_FALL:
		aWindow.apply_surface( (int)x, (int)y, jackJump, 0, &clipsJumpRightEnd[ frame ] );
		break;
	case JACK_LEFT_FALL:
		aWindow.apply_surface( (int)x, (int)y, jackJump, 0, &clipsJumpLeftEnd[ frame ] );
		break;
	}
}

void Jack::set_camera()
{
	//Center camera over Jack
	camera.x = (Sint16)(( x + JACK_WIDTH / 2 ) - SCREEN_WIDTH / 2);
	camera.y = (Sint16)(( y + JACK_HEIGHT / 2 ) - SCREEN_HEIGHT / 2);

	//If Offsets are set, Ease the camera to offsets.
	if(xOffset != 0 || yOffset != 0){
		xDiff = xOffset - xOff;
		yDiff = yOffset - yOff;
		// Calculate Easing.
		if(EaseIn > 0){
			EaseIn -= 2;
		}
		if(EaseOut > 0 && (EaseOut >= abs(xDiff) || EaseOut >= abs(yDiff)) ){
			xCamVel = (int)floor(xDiff*.5);
			yCamVel = (int)floor(yDiff*.5);
		}
		// Make sure The Ease amounts are not negative
		if(EaseIn < 0){
			EaseIn = 0;
		}
		if(EaseOut < 0){
			EaseOut = 0;
		}
		//If The it has slowed down enough, Just snap to target
		if(xCamVel < 3 && xCamVel > -3){
			xCamVel = 0;
			xOff = xOffset;
			xDiff = 0;
		}
		if(yCamVel < 3 && yCamVel > -3){
			yCamVel = 0;
			yOff = yOffset;
			yDiff = 0;
		}
		//If not too slow, travel to target.
		if(xCamVel != 0 || yCamVel != 0){
			if(xCamVel > 0){
				xOff += xCamVel - EaseIn;
			}
			else{
				xOff += xCamVel + EaseIn;
			}
			if(yCamVel > 0){
				yOff += yCamVel - EaseIn;
			}
			else{
				yOff += yCamVel + EaseIn;
			}
		}
		camera.x += (Sint16)xOff;
		camera.y += (Sint16)yOff;
	}

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

void Jack::set_camera2()
{
	//Center camera over Jack
	camera2.x = (Sint16)(LEVEL_WIDTH / 2);
	camera2.y = (Sint16)(LEVEL_HEIGHT / 2);
}

void Jack::shift_camera(int xoffset, int yoffset)
{
	xOffset = xoffset;
	yOffset = yoffset;
	xDiff = 0, yDiff = 0, xOff = 0, yOff = 0;
	//Temporary Values for Easing
	EaseIn = 10, EaseOut = 20, xCamVel = 0, yCamVel = 10;
}

void Jack::shift_boxes()
{
	//Shift Jack's hitbox
	box.x = (Sint16)(x + (JACK_WIDTH / 4));
	box.y = (Sint16)(y);

	//Shift the feet's hitbox
	feet.x = (Sint16)(x + (JACK_WIDTH / 2));
	feet.y = (Sint16)(y + (JACK_HEIGHT + 1));
}

void Jack::set_clips()
{
	//Clip Sprites
	//Right movement
	for(int j=0; j<6; j++)
	{
		clipsRight[j].x = 0;
		clipsRight[j].y = JACK_HEIGHT * j;
		clipsRight[j].w = JACK_WIDTH;
		clipsRight[j].h = JACK_HEIGHT;
	}

	//Left Movement
	for(int j=0; j<6; j++)
	{
		clipsLeft[j].x = JACK_WIDTH;
		clipsLeft[j].y = JACK_HEIGHT * j;
		clipsLeft[j].w = JACK_WIDTH;
		clipsLeft[j].h = JACK_HEIGHT;
	}

	//Right jump
	for(int j=0; j<4; j++)
	{
		clipsJumpRightStart[j].x = JACK_WIDTH * j;
		clipsJumpRightStart[j].y = 0;
		clipsJumpRightStart[j].w = JACK_WIDTH;
		clipsJumpRightStart[j].h = JACK_HEIGHT;
	}

	//Right fall
	for(int j=0; j<4; j++)
	{
		clipsJumpRightEnd[j].x = JACK_WIDTH * (4+j);
		clipsJumpRightEnd[j].y = 0;
		clipsJumpRightEnd[j].w = JACK_WIDTH;
		clipsJumpRightEnd[j].h = JACK_HEIGHT;
	}

	//Left jump
	for(int j=0; j<4; j++)
	{
		clipsJumpLeftStart[j].x = JACK_WIDTH * (7-j);
		clipsJumpLeftStart[j].y = JACK_HEIGHT;
		clipsJumpLeftStart[j].w = JACK_WIDTH;
		clipsJumpLeftStart[j].h = JACK_HEIGHT;
	}

	//Left fall
	for(int j=0; j<4; j++)
	{
		clipsJumpLeftEnd[j].x = JACK_WIDTH * (3-j);
		clipsJumpLeftEnd[j].y = JACK_HEIGHT;
		clipsJumpLeftEnd[j].w = JACK_WIDTH;
		clipsJumpLeftEnd[j].h = JACK_HEIGHT;
	}
}

void Jack::Collide_Response(bool feet, bool head, bool touchRight, bool touchLeft, int edge, SDL_Rect& currentPlat)
{
	//Note: after changes, the "hit" bool should not be needed, and the platform pointer shouldn't either.
	//if the collision was with the feet, put on top, set the flag.
	if( feet == true && head == false){
		y = (edge - box.h)-1;
		yVel = 0;
		//the onGround flag stops gravity in move().
		onGround = true;
		//conditions in move() will unset the flag. 
		//That function needs values from the current platform to know when the player walks off.
		standingOn = &currentPlat;
	}
	else if(head == true){
		y = edge;
	}
	else if(touchRight == true){
		x = edge - box.w - ((JACK_WIDTH-box.w)/2) -6;			// for side collision. Upper is the area of the upper body that, when 
		onRight = true;											// collided, you fall as if hitting a wall, instead of landing on top.
		touchSide = &currentPlat;
	}
	else if(touchLeft == true){
		x = edge - ((JACK_WIDTH-box.w)/2);
		onLeft = true;
		touchSide = &currentPlat;
	}
	/*
	if( onRight == true || onLeft == true ){
		if( touchSide->y < (box.y+(box.h*.75)) && box.y < touchSide->y+touchSide->h ){	

		}	
	} //Apply velocity if not touching sides in that direction

	//check to see if the dimensions of this platform match the dimensions of the one that was stored as the one that was being stood on
	bool samePlat = false;
	if(standingOn->x == currentPlat.x && standingOn->y == currentPlat.y && standingOn->w == currentPlat.w && standingOn->h == currentPlat.h){
		samePlat = true;
	}
	else{
		samePlat = false;
	}*/
	shift_boxes();
}

// ---- Functions to control Jack -------------------------------------------------------------------
void Jack::accel(int xAccel, int yAccel)
{
	xVel += xAccel;
	yVel += yAccel;
}

void Jack::walk(int dir)	//Handles what happens when left and right keys are pressed, sets flags.
{
		switch (dir)	// get the direction command.
		{
		case 0:// left down
			left = true;
			xVel -= 10;
			if(right == true){
				xVel -= 10;
				right = false;
			}
			break;
		case 1:	// right down
			right = true;
			xVel += 10;
			if(left == true){
				xVel += 10;
				left = false;
			}
			break;
		case 2:
			if(left == true){// left up
			left = false;
			xVel += 10;
			}
			break;
		case 3:
			if(right == true){// right up
			right = false;
			xVel -= 10;
			}
			break;
		default:
			break;
		}
}

void Jack::stop()
{
	xVel = 0;
}

double Jack::Read(int val)
{
	double ret;
	switch(val)
	{
		case 0: ret = x; break;
		case 1: ret = y; break;
		case 2: ret = xVel; break;
		case 3: ret = yVel; break;
		case 4: ret = onGround; break;
		default: ret = x; break;
	}

	return ret;
}

SDL_Rect Jack::Read_rect(int val)
{
	SDL_Rect ret;
	switch(val)
	{
		case 0: ret = box; break;
		case 1: ret = feet; break;
		default: ret = box; break;
	}

	return ret;
}

SDL_Rect* Jack::get_camera()
{
	return &camera;
}

double Jack::get_camera_value(int val)
{
	double ret;
	switch(val)
	{
		case 0: ret = camera.x; break;
		case 1: ret = camera.y; break;
		case 2: ret = camera.w; break;
		case 3: ret = camera.h; break;
		default: ret = camera.x; break;
	}

	return ret;
}

double Jack::get_camera_value2(int val)
{
	double ret;
	switch(val)
	{
		case 0: ret = camera2.x; break;
		case 1: ret = camera2.y; break;
		case 2: ret = camera2.w; break;
		case 3: ret = camera2.h; break;
		default: ret = camera2.x; break;
	}

	return ret;
}

void Jack::take_damage(int taken)
{
	//TODO : Determine which side player was hit from
	
	//Apply knockback force
	yVel -= taken * 2;
	xVel -= taken * 3;
	onGround = false;

	//Knock down health
	player_health -= taken;

	//Set flag to compensate for movement changes
	knockbackX = ((double)taken) * 3;

	//Check if ded
	if( player_health <= 0 ) {
		xVel = 0;
		yVel = 0;
		x = 0;
		y = 0;
		knockbackX = 0;
		player_health = 10;
	}
}

int Jack::current_health()
{
	return player_health;
}

std::string Jack::current_health_HUD()
{
	std::string health_HUD = "";

	//Set string length based on current health
	for(int j=0; j<player_health; j++){
		health_HUD += "I";
	}

	return health_HUD;
}