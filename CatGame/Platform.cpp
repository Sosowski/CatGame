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

	load_files();
}

Platform::Platform(int xNew,int yNew, int wNew, int hNew)
{
	plat.x = xNew;
	plat.y = yNew;
	plat.w = wNew;
	plat.h = hNew;

	load_files();
}

bool Platform::load_files()
{
	//Load images For Platform
	plat1 = load_image( "plat1.png" );

	if ( plat1 == NULL)
	{
		return false;
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
	default: // defualt is x value
		ret = plat.x;
		break;
	}
	return ret;
}

void Platform::show(Window& aWindow)
{
	aWindow.apply_surface( plat.x, plat.y, plat1, 1, &camera);
}