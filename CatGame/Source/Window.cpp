#include "Library.h"

//Enable window +++++++++++++++++++
Window::Window()
{
	//Set up screen (Starting windowed)
	//screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

	screen = SDL_CreateWindow("Please stop jumping off that platform.",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_INPUT_GRABBED);
	sdlRenderer = SDL_CreateRenderer(screen, -1, 0);

	background = NULL;
	target = NULL;
	EaseIn = 0;
	EaseOut = 0;

	if( screen == NULL )
	{
		windowOK = false;
		return;
	}
	else
	{
		windowOK = true;
	}

	//SDL_WM_SetCaption( "Please stop jumping off that platform.", NULL);

	//Set windowed flag
	windowed = true;
}

//Toggle fullscreen+++++++++++++++++++++++

void Window::toggle_fullscreen()
{
	//Check if screen currently windowed
	if( windowed == true )
	{
		//Set screen to fullscreen
		screen = SDL_CreateWindow("Please stop jumping off that platform.",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			0,
			0,
			SDL_WINDOW_FULLSCREEN_DESKTOP);

		if( screen == NULL )
		{
			windowOK = false;
			return;
		}

		//Set windowed flag
		windowed = false;
	}
	//If screen currently fullscreen
	else if( windowed == false )
	{
		//Remove fullscreen flag
		screen = SDL_CreateWindow("Please stop jumping off that platform.",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_INPUT_GRABBED);

		if( screen == NULL )
		{
			windowOK = false;
			return;
		}

		//Set windowed flag
		windowed = true;
	}
}

//Window event handler+++++++++++++++++++++++++++++
void Window::handle_events()
{
	if( windowOK == false)
	{
		return;
	}

	//Toggle fullscreen key (currently backspace)
	if( ( event.type == SDL_KEYUP ) && ( event.key.keysym.sym == SDLK_BACKSPACE ) )
	{
		toggle_fullscreen();
	}

	//Detect window focus loss
	else if( event.type == SDL_WINDOWEVENT )
	{
		//Detect window minimize
		if( event.window.event & SDL_WINDOWEVENT_MINIMIZED)
		{
			
		}
		//Detect keyboard focus loss
		else if( event.window.event & SDL_WINDOWEVENT_FOCUS_LOST)
		{

		}
		//Detect mouse focus loss
		else if( event.window.event & SDL_WINDOWEVENT_LEAVE)
		{

		}
	}

	//If screen has been altered
	else if( event.window.event == SDL_WINDOWEVENT_EXPOSED)
	{
		SDL_RenderPresent(sdlRenderer);
	}
}

//Send error flag +++++++++++++++
bool Window::error()
{
	return !windowOK;
}

//load files
bool Window::load_files(std::string& BgImage)
{
	background = load_image( BgImage );

	if(BgImage == "" || background == NULL )
	{
		return false;
	}
	return true;
}

SDL_Window* Window::getScreen()
{
	return screen;
}

SDL_Renderer* Window::getRenderer()
{
	return sdlRenderer;
}

SDL_Surface* Window::getBG()
{
	return background;
}

void Window::showBG()
{
	apply_surface( 0, 0, 1, 0, &camera );
}

void Window::update_Cam()
{
	xDiff = target->x - camera.x;
	yDiff = target->y - camera.y;

	// Calculate Easing.
	if(EaseIn > 0){
		EaseIn -= 5;
	}
	if(EaseOut > 0 && (EaseOut >= abs(xDiff) || EaseOut >= abs(yDiff)) ){
		xVel = (int)floor(xDiff*.5);
		yVel = (int)floor(yDiff*.5);
	}
	// Make sure The Ease amounts are not negative
	if(EaseIn < 0){
		EaseIn = 0;
	}
	if(EaseOut < 0){
		EaseOut = 0;
	}
	//If The it has slowed down enough, Just snap to target
	if(xVel < 3 && xVel > -3){
		xVel = 0;
		camera.x = target->x;
	}
	if(yVel < 3 && yVel > -3){
		yVel = 0;
		camera.y = target->y;
	}
	//If not too slow, travel to target.
	if(xVel != 0 || yVel != 0){
		if(xVel > 0){
			camera.x += xVel - EaseIn;
		}
		else{
			camera.x += xVel + EaseIn;
		}
		if(yVel > 0){
			camera.y += yVel - EaseIn;
		}
		else{
			camera.y += yVel + EaseIn;
		}
	}
}

void Window::set_target(SDL_Rect* tar)
{
	target = tar;
	//Clear all other values to snap to target
	xVel = 0;
	yVel = 0;
	EaseIn = 0;
	EaseOut = 0;
}

void Window::set_target(SDL_Rect* tar, int Vel, int ease_in, int ease_out)
{
	target = tar;								//set target
	int base = target->x - camera.x;
	int height = target->y - camera.y;
	float angle = (float)base/height;					//Temporarily use angle, this is not the actual angle yet.
	angle = tan(angle);							//Then some trig to get x and y velocities.
	xVel = (int)floor(Vel * cos(angle));
	yVel = (int)floor(Vel * sin(angle));
	// Set the ease Values
	EaseIn = ease_in;
	EaseIn = ease_out;
}

void Window::apply_surface ( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip)
{
	//Temporary rectangle to hold offsets
	SDL_Rect offset;

	//Give offsets to rectangle
	offset.x = x;
	offset.y = y;

	//Apply surface to screen (Blit)
	SDL_BlitSurface( source, clip, destination, &offset );
}

void Window::apply_surface ( int x, int y, int sou, int dest, SDL_Rect* clip)
{
	SDL_Surface* source = NULL;
	SDL_Window* wSource = NULL;
	SDL_Surface* destination = NULL;
	SDL_Window* wDestination = NULL;
	
	switch(sou)
	{
	case 0:
		wSource = screen;
		break;
	case 1:
		source = background;
		break;
	default:
		wSource = screen;
		break;
	}

	switch(dest)
	{
	case 0:
		wDestination = screen;
		break;
	case 1:
		destination = background;
		break;
	default:
		wDestination = screen;
		break;
	}

	//Temporary rectangle to hold offsets
	SDL_Rect offset;

	//Give offsets to rectangle
	offset.x = x;
	offset.y = y;

	//Apply surface to screen (Blit)
	SDL_BlitSurface( source, clip, destination, &offset );
}

void Window::apply_surface ( int x, int y, SDL_Surface* source, int dest, SDL_Rect* clip)
{
	SDL_Surface* destination = NULL;
	SDL_Window* wDestination = NULL;

	//Temporary rectangle to hold offsets
	SDL_Rect offset;

	switch(dest)
	{
	case 0:
		wDestination = screen;
		//Give offsets to rectangle
		offset.x = x - camera.x;
		offset.y = y - camera.y;
		break;
	case 1:
		destination = background;
		//Give offsets to rectangle
		offset.x = x;
		offset.y = y;
		break;
	default:
		wDestination = screen;
		break;
	}

	//Apply surface to screen (Blit)
	SDL_BlitSurface( source, clip, destination, &offset );

}

bool Window::return_windowOK(){

	return windowOK;

}

/*void Window::apply_surface ( int x, int y, int sou, SDL_Surface* destination, SDL_Rect* clip)
{
	SDL_Surface* source = NULL;

	switch(sou)
	{
	case 0:
		source = screen;
		break;
	case 1:
		source = background;
		break;
	default:
		source = screen;
		break;
	}

	//Temporary rectangle to hold offsets
	SDL_Rect offset;

	//Give offsets to rectangle
	offset.x = x;
	offset.y = y;

	//Apply surface to screen (Blit)
	SDL_BlitSurface( source, clip, destination, &offset );
}*/