
/***************************************************************************
 *   Copyright (C) 2005 Matthew R. Hurne                                   *
 *                                                                         *
 *   This file is part of PNP.                                             *
 *                                                                         *
 *   PNP is free software; you can redistribute it and/or modify it under  *
 *   the terms of the GNU General Public License as published by the Free  *
 *   Software Foundation; either version 2 of the License, or (at your     *
 *   option) any later version.                                            *
 *                                                                         *
 *   PNP is distributed in the hope that it will be useful, but WITHOUT    *
 *   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or *
 *   FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License *
 *   for more details.                                                     *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with PNP; if not, write to the Free Software Foundation, Inc.,  *                                    *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

/**
 * @mainpage PNP's Not Pong!
 *
 * @section Introduction
 *   Welcome to the API documentation for PNP.
 */

#include <iostream>
#include "SDL.h"
#include "SDL_ttf.h"
#include "PNP.H"

using std::cout;
using std::cerr;
using std::flush;
using std::endl;

int main(int argc, char *argv[]) {
	/* Initialize SDL */
	if( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 ) {
		cerr << "Could not initialize SDL: " << SDL_GetError() << endl;
		SDL_Quit();
		return -1;
	}

	SDL_Surface *screen = SDL_SetVideoMode(800, 600, 0, SDL_SWSURFACE);
	if(!screen) {
		cerr << "Could not set video mode: " << SDL_GetError() << endl;
		SDL_Quit();
		return -1;
	}
	
	/* Initialize SDL_ttf */
	if (TTF_Init() < 0) {
		cerr << "Could not initialize SDL_ttf: " << TTF_GetError() << endl;
		TTF_Quit();
		SDL_Quit();
		return -1;
	}
	
	PNP pnp(screen);
	SDL_ShowCursor(0);
	pnp.run();

	TTF_Quit();
	SDL_Quit();
	return 0;
}
