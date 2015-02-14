
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

#include <iostream>
#include "SDL.h"
#include "Bitmask.H"

Bitmask::Bitmask(SDL_Surface *surface)
:
mSurface(surface),
mWidth(mSurface->w),
mHeight(mSurface->h)
{
	//std::cout << "Width: " << mWidth << std::endl;//TEMP
	//std::cout << "Height: " << mHeight << std::endl;//TEMP
	//Allocate the two-dimensional array to match the size of our SDL_Surface
	mBits = new bool*[mWidth];
	for(int x = 0; x < mWidth; ++x) {
		mBits[x] = new bool[mHeight];
	}

	if(SDL_MUSTLOCK(mSurface)) {
		SDL_LockSurface(mSurface);
	}

	Uint32 pixel;
	Uint32 transparent = SDL_MapRGB(mSurface->format, 0xFF, 0x00, 0x00);
	for(int x = 0; x < mWidth; ++x) {
		for(int y = 0; y < mHeight; ++y) {

			pixel = getPixel(x, y);

			if(pixel == transparent) {
				mBits[x][y] = false;
			} else {
				mBits[x][y] = true;
			}
		}
	}

	if(SDL_MUSTLOCK(mSurface)) {
		SDL_UnlockSurface(mSurface);
	}
	
	//for(int y = 0; y < mHeight; ++y) {
	//	for(int x = 0; x < mWidth; ++x) {
	//		std::cout << mBits[x][y];
	//	}
	//	std::cout << std::endl;
	//}
	
}

Bitmask::~Bitmask() {
	for(int x = mWidth - 1; x >= 0; --x) {
		delete[] mBits[x];
	}
	delete[] mBits;
}

bool Bitmask::collision(const Bitmask& mask,
						int AX, int AY, int BX, int BY,
						int width, int height) const {
	//std::cout << "Width: " << width << std::endl;
	//std::cout << "Height: " << height << std::endl;
	bool collision = false;
	for(int y = 0; y < height; ++y) {
		for(int x = 0; x < width; ++x) {
			bool bit = mBits[AX + x][AY + y] && mask.mBits[BX + x][BY + y];
			if(bit) {
				collision = true;
			}
			//std::cout << bit << std::flush;//TEMP
		}
		//std::cout << std::endl;//TEMP
	}
	return collision;
}

Uint32 Bitmask::getPixel(int x, int y)
{
    int bpp = mSurface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8*)mSurface->pixels + y * mSurface->pitch + x * bpp;

    switch(bpp) {
    case 1: /* 8 bpp */
        return *p;

    case 2: /* 16 bpp */
        return *(Uint16*)p;

    case 3: /* 24 bpp */
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            return p[0] << 16 | p[1] << 8 | p[2];
		}
        else {
            return p[0] | p[1] << 8 | p[2] << 16;
		}

    case 4: /* 32 bpp */
        return *(Uint32*)p;

    default:
        return 0;
    }
}
