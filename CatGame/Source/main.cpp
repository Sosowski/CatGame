/*CATGAME - Because there exists a parasite that alters your perception of cats.

CURRENT NOTABLE BUGS
-Mix_PlayChannel(-1, irrelevent, irrelevent) returns -1 like there was an error, but there isn't one. -1 Is supposed to select first available
channel, so this is a feature we will need to have.

TO DO MORE ACCURATE ERROR HANDLING
#include Windows.h, MessageBox(NULL, "MessageBox Text", "MessageBox caption", MB_OK);
Throw it into the error check statement you think is screwing up before the return

Please do not #include Windows.h normally due to the increased build times.

FOR DEBUG
//Convert to string
std::stringstream caption;

//Generate string
caption << "test " << variable << " ";

//Set caption
SDL_WM_SetCaption( caption.str().c_str(), NULL);
*/

#include "Library.h"

//---- Define Stuff that is used globally here.
//SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

std::string BgImage = "";

//Stores data for all current projectiles
std::vector<Projectile> projectiles;

//Stores data for all platforms
std::vector<Platform> platforms;

//Temp storage for mouse position on click for projectiles
int ClickX, ClickY;
//Temp storage for projectile target
SDL_Rect ProjectileTarget;

//FUNCTIONS------------------------------------------------------------------------------------------------

//Load / optimise images before displaying, create pointer to optimized image ---- Now defined as global in FunctionLib
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

	//Cirumvents "not all paths return a value" warning
	return 0;
}

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

// This loads the files necesary for the base game. Files for jack and others are in another function in thier class.
bool load_files(Window aWindow)
{
	bool BG = false;
	//Load images
	//example = load_image( "example.png" );
	//USED TO BE: background = load_image( "bg.png" );
	BG = aWindow.load_files(BgImage);
	bullet = load_image( "Images/bullet.png" );
	//Jacks Files for sprites moved to Jack.h and Jack.cpp

	//Open font
	font = TTF_OpenFont( "Images/QuartzMS.ttf", 28);

	if (bullet == NULL || BG == false)
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

void handle_events(Jack& player)//right now only takes thing of type Jack, ultimately I wanna be able to make that able to be switched with some other sprite.
{
	//Create a projectile on left mouse button click
	if(event.type == SDL_MOUSEBUTTONDOWN)
	{
		if(event.button.button == SDL_BUTTON_LEFT)
		{
					//Get the current coordinates of the mouse click
					SDL_GetMouseState(&ClickX, &ClickY);
					//Set attributes for the target hitbox
					ProjectileTarget.x = ClickX + (Sint16)player.get_camera_value(0);
					ProjectileTarget.y = ClickY + (Sint16)player.get_camera_value(1);
					ProjectileTarget.w = 1;
					ProjectileTarget.h = 1;
					//Create the projectile, origin being Jack
					Projectile tmp_proj( ( player.Read(0) + (JACK_WIDTH / 2) + 50 ), ( player.Read(1) + (JACK_HEIGHT / 2) - 50 ), 5.0, ProjectileTarget);
					//Add it to the projectile vector
					projectiles.push_back(tmp_proj);
		}
	}

	//Check for keypress
	if( event.type == SDL_KEYDOWN )
	{
		switch( event.key.keysym.sym )
		{
			case SDLK_w:
				if(player.Read(4) == 1)	//retreive onGround flag from player, check if it is set.
				{
					player.accel(0,-20);
				}
				break;
			case SDLK_a: player.walk(0); break;	//Move Left
			case SDLK_d: player.walk(1); break;	//Move right
			case SDLK_s: player.shift_camera(0,100); break;	//shift camera down a bit
				//TEMPORARY TESTING!!!!!!!!!!!
				//Make player take damage ranging from 1-10
			case SDLK_1: player.take_damage(1); break;
			case SDLK_2: player.take_damage(2); break;
			case SDLK_3: player.take_damage(3); break;
			case SDLK_4: player.take_damage(4); break;
			case SDLK_5: player.take_damage(5); break;
			case SDLK_6: player.take_damage(6); break;
			case SDLK_7: player.take_damage(7); break;
			case SDLK_8: player.take_damage(8); break;
			case SDLK_9: player.take_damage(9); break;
			case SDLK_0: player.take_damage(10); break;
		}
	}
	//Check for key release
	else if( event.type == SDL_KEYUP )
	{
		switch( event.key.keysym.sym )
		{
			case SDLK_w: ; break;
			case SDLK_a: player.walk(2); break;
			case SDLK_d: player.walk(3); break;
			case SDLK_s: player.shift_camera(0,0); break;	//shift camera back to where it was.
		}
	}
	
}

//Remove all objects and close program
void clean_up()
{
	//Free images
	//SDL_FreeSurface ( example );
	//SDL_FreeSurface ( background );
	SDL_FreeSurface ( jackRun );
	SDL_FreeSurface ( jackJump );
	SDL_FreeSurface ( bullet );
	SDL_FreeSurface ( plat1 );
	SDL_FreeSurface ( plat2 );

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

	//Set Current Background Image Name
	BgImage = "Images/bg.png";

	//Load files
	if( load_files(myWindow) == false)
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

	//Create timer for clock
	Timer clock;

	//Create Jack Sprite
	Jack walk;

	//Set Windows Camera focus to jack
	myWindow.set_target(walk.get_camera());

	//Create Platforms
	Platform platform1;
	Platform platform2(750, 1000 , 500 , 100 , 2);
	Platform platform3(1300, 750, 700, 50, 3);

	//Load Platform images
	plat1 = platform1.Return(1);
	plat2 = platform2.Return(2);
	plat3 = platform3.Return(3);

	//Add platforms to vector
	platforms.push_back(platform1);
	platforms.push_back(platform2);
	platforms.push_back(platform3);

	//Start clock timer
	clock.start();

	//Begin loop to hold screen open
	while( quit == false )
	{
		//Start the fps timer
		fps.start();

		//Event handler
		while( SDL_PollEvent( & event ) )
		{
			//Handle Controls
			handle_events(walk);

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

		//Projectile movement
		//Check if there are any projectiles before proceeding
		if(projectiles.size()>=1)
		{
			//Loop for how many projectiles we have
			 for(int proj=0; proj < (signed int) projectiles.size(); proj++)
             {
					//Check for disabled projectile
					if(projectiles.at(proj).is_disabled())
                    {
                        //Delete this projectile
						projectiles.erase(projectiles.begin() + proj);
					} 
					else 
					{
						projectiles.at(proj).move();
                    }
             }
		}

		//Do collision checking for all current platforms
		if(platforms.size() >= 1)
		{			
			for(int pr=0; pr < (signed int)platforms.size(); pr++)
			{
				SDL_Rect PlatRect = {platforms.at(pr).Read(0), platforms.at(pr).Read(1), platforms.at(pr).Read(2), platforms.at(pr).Read(3)};
				if(pr == (platforms.size() - 1))
				{
					walk.Collide_Check(PlatRect, 1);
				}
				else
				{
					walk.Collide_Check(PlatRect, 0);
				}
			}
		}

		//Set Jacks camera
		walk.set_camera();

		//Move the main camera
		myWindow.update_Cam();

		//Show background
		myWindow.showBG();

		//Show platforms
		//plat1.show(myWindow);
		if(platforms.size() >= 1)
		{
			for(int pr=0; pr < (signed int)platforms.size(); pr++)
			{
				//Find platform type, place on screen the correct platform
				switch (platforms.at(pr).Read(4))
				{
				case 1:
					myWindow.apply_surface((int)platforms.at(pr).Read(0), (int)platforms.at(pr).Read(1), plat1, 1, &camera);
					break;
				case 2:
					myWindow.apply_surface((int)platforms.at(pr).Read(0), (int)platforms.at(pr).Read(1), plat2, 1, &camera);
					break;
				case 3:
					myWindow.apply_surface((int)platforms.at(pr).Read(0), (int)platforms.at(pr).Read(1), plat3, 1, &camera);
					break;
				default:
					break;
				}
			}
		}

		//Show Jack
		walk.show(myWindow);

		//Show projectiles
		if(projectiles.size() >= 1)
		{
			for(int pr=0; pr < (signed int)projectiles.size(); pr++)
			{
				//If projectile is not disabled
				if(!projectiles.at(pr).is_disabled())
				{
					myWindow.apply_surface((int)projectiles.at(pr).get_x(), (int)projectiles.at(pr).get_y(), bullet, 0, &camera);
				}
			}
		}

		//Create string for timer
		std::stringstream time;

		//Convert time to string

		if((clock.get_ticks_clock() / 1000) <= 9)
		{
			time << clock.return_min() << ":0" << clock.get_ticks_clock() / 1000;
		}
		else
		{
			time << clock.return_min() << ":" << clock.get_ticks_clock() / 1000;
		}

		//Render time surface
		seconds = TTF_RenderText_Solid( font, time.str().c_str(), textColor);

		//Apply time surface
		myWindow.apply_surface( (walk.get_camera_value(0) + (SCREEN_WIDTH / 2) ), (walk.get_camera_value(1) + (SCREEN_HEIGHT - 50)), seconds, 0, &camera);

		//Free time surface
		SDL_FreeSurface( seconds );

		//Get keystate
		Uint8 *keystates = SDL_GetKeyState ( NULL );

		//Update Screen
		if( SDL_Flip ( myWindow.getScreen() ) == -1 )
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


// ---- For Global use, Defined in FunctionLib ----
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