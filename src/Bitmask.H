
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

#ifndef GUARD_BITMASK_H
#define GUARD_BITMASK_H

#include "SDL.h"

class Bitmask {
	public:
		Bitmask(SDL_Surface *surface);
		~Bitmask();
		
		bool getBit(int x, int y) { return mBits[x][y]; }
		void setBit(int x, int y, bool value) { mBits[x][y] = value; }
		bool collision(const Bitmask& mask, int AX, int AY, int BX, int BY, int width, int height) const;

	private:
		SDL_Surface *mSurface;
		int mWidth, mHeight;
		bool **mBits;
		
		Uint32 getPixel(int x, int y);
};

#endif
