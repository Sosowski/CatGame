#include "Library.h"

Platform::Platform()
{
	//Platform default hitboxes
	plat.x = 0;
	plat.y = 750;
	plat.w = 400;
	plat.h = 100;
	//Initialize variables for loading image
	//plat1 = NULL;
	platType = 1;

	load_files();
}

Platform::Platform(int xNew,int yNew, int wNew, int hNew, int type)
{
	plat.x = xNew;
	plat.y = yNew;
	plat.w = wNew;
	plat.h = hNew;
	platType = type;

	load_files();
}

bool Platform::load_files()
{
	switch (platType)
	{
	case 1:
		plat1 = load_image( "Images/plat1.png" );
		if ( plat1 == NULL)
		{
			return false;
		}
		break;
	case 2:
		plat2 = load_image( "Images/plat2.png" );
		if ( plat2 == NULL)
		{
			return false;
		}
		break;
	case 3:
		plat3 = load_image( "Images/plat3.png" );
		if ( plat3 == NULL)
		{
			return false;
		}
		break;
	default:
		//Shouldn't happen
		break;
	}

	return true;
}

int Platform::Read(int val)
{
	int ret;
	switch (val)
	{
	case 0: // x value
		ret = plat.x;
		break;
	case 1: // y value
		ret = plat.y;
		break;
	case 2:	// width
		ret = plat.w;
		break;
	case 3:	// height
		ret = plat.h;
		break;
	case 4: //type
		ret = platType;
		break;
	default: // defualt is x value
		ret = plat.x;
		break;
	}
	return ret;
}

SDL_Surface *Platform::Return(int val)
{
	SDL_Surface *image = NULL;
	switch (val)
	{
	case 1:
		image = plat1;
		break;
	case 2:
		image = plat2;
		break;
	case 3:
		image = plat3;
		break;
	default:
		break;
	}
	return image;
}

void Platform::show(Window& aWindow)
{
	aWindow.apply_surface( plat.x, plat.y, plat1, 1, &camera);
}

bool Platform::Collide(Jack& player)
{
	bool hit = false, touchFeet = false, touchHead;
	int edge = 0, type = 0;
	SDL_Rect hitbox = player.Read_rect(0);
	SDL_Rect feet = player.Read_rect(1);
	SDL_Rect head = {hitbox.x,hitbox.y-1,hitbox.w,1};

	//check if the probes are colliding
	touchFeet = check_collision(plat,feet);
	touchHead = check_collision(plat,head);

	//if colliding with player hitbox
	if(check_collision(plat,hitbox) == true){
		hit=true;
		edge = plat.y-1;
		/*if(touchHead){
			edge = plat.y + plat.h + hitbox.h + 1;
		}
		else if(touchFeet) {
			edge=plat.y-1;
		}
		else {
			if( hitbox.x+(hitbox.w/2) < plat.x+(plat.w/2) ) {
				edge = plat.x-hitbox.w-5;	
			}
			else{
				edge = plat.y+plat.w+1;
			}
		}*/
	}
	player.Collide_Response(hit,touchFeet,touchHead,edge);
	if(hit == true || touchFeet){
		return true;
	}
	else{
		return false;
	}
}

Triangle::Triangle()
{
	//Platform default hitboxes
	plat.x = 0;
	plat.y = 750;
	plat.w = 400;
	plat.h = 100;
	//Initialize variables for loading image
	//plat1 = NULL;
	platType = 1;

	load_files();
}

bool Triangle::Collide(Jack& player)
{
	bool hit = false, touchFeet = false, touchHead;
	int edge = 0, type = 0;
	SDL_Rect hitbox = player.Read_rect(0);
	SDL_Rect feet = player.Read_rect(1);
	SDL_Rect head = {hitbox.x,hitbox.y-1,hitbox.w,1};

	//check if the probes are colliding
	touchFeet = check_collision(plat,feet);
	touchHead = check_collision(plat,head);

	//if colliding with player hitbox
	if(check_collision(plat,hitbox) == true){
		if(touchHead){
			edge = plat.y + plat.h + 1;
		}
		else if(touchFeet) {
			edge=plat.y-1;
		}
		else {
			if( hitbox.x+(hitbox.w/2) < plat.x+(plat.w/2) ) {
				edge = plat.x;	
			}
			else{
				edge = plat.y+plat.w;
			}
		}
		hit=true;
	}

	player.Collide_Response(hit,touchFeet,touchHead,edge);

	if(hit == true || touchFeet == true || touchHead == true){
		return true;
	}
	else{
		return false;
	}
}