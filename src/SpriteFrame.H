
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

#ifndef GUARD_SPRITEFRAME_H
#define GUARD_SPRITEFRAME_H

#include "SDL.h"
#include "Bitmask.H"

class SpriteFrame {
	public:
		SpriteFrame(SDL_Surface *image, const int delay);
		~SpriteFrame();
		
		const Bitmask& getBitmask() const { return m_Mask; }
		int getWidth() const { return m_Image->w; }
		int getHeight() const { return m_Image->h; }
		SDL_Surface* getImage() const { return m_Image; }
		
	private:
		SDL_Surface *m_Image;
		Bitmask m_Mask;
};

#endif
