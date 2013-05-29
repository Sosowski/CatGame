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
	box.x = (Sint16)(x + (JACK_WIDTH / 4));
	box.y = (Sint16)(y);
	box.w = 150;
	box.h = JACK_HEIGHT;

	//Create probe hitbox
	probe.x = (Sint16)(x + (JACK_WIDTH / 2));
	probe.y = (Sint16)(y + (JACK_HEIGHT + 1));
	probe.w = 1;
	probe.h = 1;

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
	jackRun = load_image( "JackRun.png" );
	jackJump = load_image( "JackJump.png" );

	if ( jackRun == NULL || jackJump == NULL)
	{
		return false;
	}

	return true;
}

void Jack::move()
{
	//Left/Right Movement
	x += xVel;

	//Check if Jack left the area
	if( ( x < 0 ) || ( x + JACK_WIDTH > LEVEL_WIDTH ) )
	{
		//Move back
		x -= xVel;
	}

	//Up/Down Movement
	y += yVel;

	//The probe isn't colliding with anything. We need to fall.
	if( ( y + JACK_HEIGHT > LEVEL_HEIGHT == false ) && (onGround == false) )
	{
		//Apply gravity
		yVel += 0.5;
	}

	if( ( y + JACK_HEIGHT > LEVEL_HEIGHT ) )
	{
		//Player fell. Reset to start
		yVel = 0;
		x = 0;
		y = 0;
	}

	//Update Jack's hitbox after all movements
	shift_boxes();
}

void Jack::show()
{
	lastStatus = status; // Record Previous status
	
//Convert to string
std::stringstream caption;

//Generate string
caption << "onGround: " << onGround << " Status: " << status << " facing: " << facing << " frame: " << frame;

//Set caption
SDL_WM_SetCaption( caption.str().c_str(), NULL);

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
	switch(status) 
	{
	case JACK_RIGHT:
		apply_surface( (int)x - camera.x, (int)y - camera.y, jackRun, screen, &clipsRight[ frame ] );
		break;
	case JACK_LEFT:
		apply_surface( (int)x - camera.x, (int)y - camera.y, jackRun, screen, &clipsLeft[ frame ] );
		break;
	case JACK_RIGHT_JUMP:
		apply_surface( (int)x - camera.x, (int)y - camera.y, jackJump, screen, &clipsJumpRightStart[ frame ] );
		break;
	case JACK_LEFT_JUMP:
		apply_surface( (int)x - camera.x, (int)y - camera.y, jackJump, screen, &clipsJumpLeftStart[ frame ] );
		break;
	case JACK_RIGHT_FALL:
		apply_surface( (int)x - camera.x, (int)y - camera.y, jackJump, screen, &clipsJumpRightEnd[ frame ] );
		break;
	case JACK_LEFT_FALL:
		apply_surface( (int)x - camera.x, (int)y - camera.y, jackJump, screen, &clipsJumpLeftEnd[ frame ] );
		break;
	}
}

void Jack::set_camera()
{
	//Center camera over Jack
	camera.x = (Sint16)(( x + JACK_WIDTH / 2 ) - SCREEN_WIDTH / 2);
	camera.y = (Sint16)(( y + JACK_HEIGHT / 2 ) - SCREEN_HEIGHT / 2);

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
	box.x = (Sint16)(x + (JACK_WIDTH / 4));
	box.y = (Sint16)(y);

	//Shift the probe's hitbox
	probe.x = (Sint16)(x + (JACK_WIDTH / 2));
	probe.y = (Sint16)(y + (JACK_HEIGHT + 1));
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

void Jack::Collide_Check(SDL_Rect plat)
{
	if(check_collision(plat,probe) == true){
		y = (plat.y - JACK_HEIGHT);
		yVel = 0;
		shift_boxes();
		onGround = true;
	}
	else{
		onGround = false;
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