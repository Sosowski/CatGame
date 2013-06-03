#include "Library.h"

//Screen is Initialized here and defined in Window.h
//This now works and is obtained by other functions with the getScreen function
SDL_Surface *screen = NULL;
//SDL_Surface *background = NULL;

//Enable window +++++++++++++++++++
Window::Window()
{
	//Set up screen (Starting windowed)
	screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	background = NULL;

	if( screen == NULL )
	{
		windowOK = false;
		return;
	}
	else
	{
		windowOK = true;
	}

	SDL_WM_SetCaption( "This is the real deal, nya! (Translator's note: nya means meow)", NULL);

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
		screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE | SDL_FULLSCREEN);

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
		screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

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
	else if( event.type == SDL_ACTIVEEVENT )
	{
		//Detect window minimize
		if( event.active.state & SDL_APPACTIVE)
		{
			if( event.active.gain == 0 )
			{
				//SDL_WM_SetCaption( "What, you hate cats now?", NULL);
			}
			else
			{
				//SDL_WM_SetCaption( "This is the real deal, nya! (Translator's note: nya means meow)", NULL);
			}
		}

		//Detect keyboard focus loss
		else if( event.active.state & SDL_APPINPUTFOCUS)
		{
			if( event.active.gain == 0)
			{
				//SDL_WM_SetCaption( "NYOOO, PUT THE KEYBOARD BACK!", NULL);
			}
			else
			{
				//SDL_WM_SetCaption( "This is the real deal, nya! (Translator's note: nya means meow)", NULL);
			}
		}
		//Detect mouse focus loss
		else if( event.active.state & SDL_APPMOUSEFOCUS)
		{
			if(event.active.gain == 0)
			{
				//SDL_WM_SetCaption( "You seem to be clicking in all the wrong places, meow.", NULL);
			}
			else
			{
				//SDL_WM_SetCaption( "This is the real deal, nya! (Translator's note: nya means meow)", NULL);
			}
		}
	}

	//If screen has been altered
	else if( event.type == SDL_VIDEOEXPOSE)
	{
		//Update screen
		if( SDL_Flip(screen) == -1)
		{
			windowOK = false;
			return;
		}
	}
}

//Send error flag +++++++++++++++
bool Window::error()
{
	return !windowOK;
}

//load files for basic stuffsuch as background
bool Window::load_files(std::string& BgImage)
{
	//background = load_image( BgImage );
	background = load_image( "bg.png" );
	if(BgImage == "" || background == NULL )
	{
		return false;
	}
	return true;
}

SDL_Surface* Window::getScreen()
{
	return screen;
}

SDL_Surface* Window::getBG()
{
	return background;
}

void Window::showBG()
{
	apply_surface( 0, 0, background, screen, &camera);
}