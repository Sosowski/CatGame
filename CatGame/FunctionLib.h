/*	Here are all of the functions we will be using Globally They should be accompanied by	*/
/*	descriptions and what file they are defined in.											*/
#include "SDL.h"
#include <string>
#include <sstream>

extern bool check_collision( SDL_Rect A, SDL_Rect B);

extern void apply_surface ( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL );

extern SDL_Surface *load_image( std::string filename);
