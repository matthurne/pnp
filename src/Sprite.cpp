
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
#include "Sprite.H"

using std::cout;
using std::endl;
using std::flush;

/* ---------- PUBLIC METHODS ---------- */

Sprite::Sprite(SpriteBase *base, SDL_Surface *screen)
:
//m_MotionVector(),
m_curFrameIndex(0),
m_Animating(false),
m_Drawn(false),
m_AnimSpeed(0.0),
m_PrevTicks(0),
m_SpriteBase(base),
m_Screen(screen),
m_BackReplacement(0),
m_PrevX(0.0),
m_PrevY(0.0)
{
	// If this Sprite has more than one frame, it will start out animating
	if(m_SpriteBase->numOfFrames() > 1) {
		m_Animating = true;
	}
	m_BackReplacement = SDL_DisplayFormat( m_SpriteBase->getFrame(0).getImage() );
}

Sprite::~Sprite() {
}

void Sprite::clearBG() {
	// If this Sprite has been drawn then we have to replace the background that
	// was on the screen before this Sprite was drawn before we draw it in its
	// new position.
	if(m_Drawn) {
        // Determine the previous placement of this Sprite on the screen.  Its
		// location is based on its center point.  However, SDL draws based at
		// the upper left-hand corner, so we must calculate that coordinate
		// here.
		SDL_Rect dest;
		dest.x = (int)(m_PrevX - getWidth() / 2);
		dest.y = (int)(m_PrevY - getHeight() / 2);
		dest.w = getWidth();
		dest.h = getHeight();
		SDL_BlitSurface(m_BackReplacement, 0, m_Screen, &dest);
	}
}

void Sprite::updateBG() {
	// Before we draw this Sprite, we need to store the image data already on
	// the screen at the position we are about to draw it on.
	
	// Determine the soon-to-be placement of this Sprite on the screen.  This
	// Sprite's location is based on its center point.  However, SDL draws based
	// at the upper left-hand corner, so we must calculate that coordinate here.
	SDL_Rect srcRect;
	srcRect.x = (int)(getX() - getWidth() / 2);
	srcRect.y = (int)(getY() - getHeight() / 2);
	srcRect.w = getWidth();
	srcRect.h = getHeight();
	SDL_BlitSurface(m_Screen, &srcRect, m_BackReplacement, 0);
	
	// Set the coordinates to be cleared before drawing this Sprite next time.
	m_PrevX = getX();
	m_PrevY = getY();
}

void Sprite::draw()
{
	if(m_Animating) {
		// Get the ticks since SDL was initialized
		int curTicks = SDL_GetTicks();
		// Calculate the ticks since the last time this Sprite switched frames
		double dt = (double)(curTicks - m_PrevTicks);
		m_PrevTicks = curTicks;
		
		// If the amount of time that has passed is equal to or greater than the
		// amount of time one frame should be displayed, switch to the next
		// frame
		if(dt / 1000 >= 1 / m_AnimSpeed) {
            // If we are currently on the last frame, jump to the first one.
            // Otherwise, jump to the next one.
            if (m_curFrameIndex >= (m_SpriteBase->numOfFrames() - 1)) {
				m_curFrameIndex = 0;
			} else {
                m_curFrameIndex++;
			}
		}
	}
	
	// If this method has been called, then this Sprite has been drawn
	if(!m_Drawn) {
		m_Drawn = true;
	}
	
	// Determine the placement of this Sprite on the screen.  Its location is
	// based on its center point.  However, SDL draws based at the upper
	// left-hand corner, so we must calculate that coordinate here.
	SDL_Rect dest;
	dest.x = (int)(getX() - getWidth() / 2);
	dest.y = (int)(getY() - getHeight() / 2);
	// Draw this Sprite's current frame
	SDL_BlitSurface(m_SpriteBase->getFrame(m_curFrameIndex).getImage(), 0, m_Screen, &dest);
}

bool Sprite::collision(Sprite& other) {
	// Calculate the positions of the edges of this Sprite
	double thisRightEdgeX = getX() + getWidth() / 2;
	double thisLeftEdgeX = getX() - getWidth() / 2;
	double thisTopEdgeY = getY() - getHeight() / 2;
	double thisBottomEdgeY = getY() + getHeight() / 2;
	// Calculate the positions of the edges of the other Sprite
	double otherRightEdgeX = other.getX() + other.getWidth() / 2;
	double otherLeftEdgeX = other.getX() - other.getWidth() / 2;
	double otherTopEdgeY = other.getY() - other.getHeight() / 2;
	double otherBottomEdgeY = other.getY() + other.getHeight() / 2;
	
	// If the right edge of this Sprite is to the left of the left edge of the
	// other Sprite, there is no collision
	if( thisRightEdgeX < otherLeftEdgeX ) {
		return false;
	}
	// If the left edge of this Sprite is to the right of the right edge of the
	// other Sprite, there is no collision
	if( thisLeftEdgeX > otherRightEdgeX ) {
		return false;
	}
	// If the bottom edge of this Sprite is above the top edge of the other
	// Sprite, there is no collision
	if( thisBottomEdgeY < otherTopEdgeY ) {
		return false;
	}
	// If the top edge of this Sprite is below the bottom edge of the other
	// Sprite, there is no collision
	if( thisTopEdgeY > otherBottomEdgeY ) {
		return false;
	}

	return true;

	/*
	double maskAX, maskAY, maskBX, maskBY, maskW, maskH;

	if( mX < sprite.mX ) {
		maskAX = sprite.mX - mX;
		maskBX = 0.0;
		maskW = mX + width() - sprite.mX;
		if (mX + width() > sprite.mX + sprite.width()) {
			maskW = (mX + width()) - (sprite.mX + sprite.width());
		} else {
			maskW = mX + width() - sprite.mX;
		}
	} else {
		maskAX = 0.0;
		maskBX = mX - sprite.mX;
		maskW = sprite.mX + sprite.width() - mX;
		if (sprite.mX + sprite.width() > mX + width()) {
			maskW = (sprite.mX + sprite.width()) - (mX + width());
		} else {
			maskW = sprite.mX + sprite.width() - mX;
		}
	}

	if( mY < sprite.mY ) {
		maskAY = sprite.mY - mY;
		maskBY = 0.0;
		if (mY + height() > sprite.mY + sprite.height()) {
			maskH = (mY + height()) - (sprite.mY + sprite.height());
		} else {
			maskH = mY + height() - sprite.mY;
		}
	} else {
		maskAY = 0.0;
		maskBY = mY - sprite.mY;
		if(sprite.mY + sprite.height() > mY + height()) {
			maskH = (sprite.mY + sprite.height()) - (mY + height());
		} else {
			maskH = sprite.mY + sprite.height() - mY;
		}
	}
	
	return mSpriteBase->bitmask(mFrame).collision( sprite.mSpriteBase->bitmask(mFrame),
													(int)maskAX, (int)maskAY,
													(int)maskBX, (int)maskBY,
													(int)maskW, (int)maskH );
	*/
}

/* ---------- PRIVATE METHODS ---------- */
