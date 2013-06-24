#include "Library.h"

StartMenu::StartMenu(){
	start_box.x = (SCREEN_WIDTH / 2) - 100;
	start_box.y = (SCREEN_HEIGHT / 2) - 50;
	start_box.w = 200;
	start_box.h = 100;

	status = 1;

	// ---- Load Files ----
	load_files();

	// ---- Clip sprite sheet ----
	clip_images();
}

bool StartMenu::load_files(){

	//Load images for buttons
	startButton = load_image( "Images/StartButton.png" );

	if ( startButton == NULL)
	{
		return false;
	}

	return true;

}

void StartMenu::clip_images(){
	
	//Clip the button images
	//Over - 0 | Out - 1 | Down - 2 | Up - 3
	clips[0].x = 0;
	clips[0].y = 0;
	clips[0].w = 200;
	clips[0].h = 100;

	clips[1].x = 200;
	clips[1].y = 0;
	clips[1].w = 200;
	clips[1].h = 100;

	clips[2].x = 0;
	clips[2].y = 100;
	clips[2].w = 200;
	clips[2].h = 100;
	
	clips[3].x = 200;
	clips[3].y = 100;
	clips[3].w = 200;
	clips[3].h = 100;
}

void StartMenu::show(Window& aWindow, int status){

	//Show the proper button
	aWindow.apply_surface( (SCREEN_WIDTH / 2) - 100, (SCREEN_HEIGHT / 2) - 50, startButton, 0, &clips[ status ] );
}

int StartMenu::state_return(){

	return GAME_STATE;

}

SDL_Rect StartMenu::button_box(){

	return start_box;

}