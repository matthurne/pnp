
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

#ifndef GUARD_SPRITE_H
#define GUARD_SPRITE_H

#include "SDL.h"
#include "MotionVector.H"
#include "SpriteBase.H"
 
class Sprite {
	public:
		Sprite(SpriteBase *base, SDL_Surface *screen);
		virtual ~Sprite();

		/***** Display *****/
        void clearBG();
		void updateBG();
		void draw();
		
		/***** Movement *****/
		void move() { m_MotionVector.move(); }
		void stop() { m_MotionVector.stop(); }
		bool isMoving() { return m_MotionVector.isMoving(); }
		void setX(const double x) { m_MotionVector.setX(x); }
		void setY(const double y) { m_MotionVector.setY(y); }
		void setPosition(const double x, const double y) { m_MotionVector.setPosition(x,y); }
		void setAngle(const double angle) { m_MotionVector.setAngle(angle); }
		void setSpeed(const double speed) { m_MotionVector.setSpeed(speed); }
		void setAcceleration(const double accel) { m_MotionVector.setAcceleration(accel); }

		/***** Animation *****/
		void startAnim() { m_Animating = true; }
		void stopAnim() { m_Animating = false; }
		void toggleAnim() { m_Animating = !m_Animating; }
		void rewind() { m_curFrameIndex = 0; }
		void setAnimSpeed(double speed) { m_AnimSpeed = speed; }
		double getAnimSpeed() const { return m_AnimSpeed; }
		void setCurFrame(int frameIndex) { m_curFrameIndex = frameIndex; }
		int getCurFrame() const { return m_curFrameIndex; }
		
		/***** Properties *****/
		int getWidth() { return m_SpriteBase->getWidth(); }
		int getHeight() { return m_SpriteBase->getHeight(); }
		double getX() { return m_MotionVector.getX(); }
		double getY() { return m_MotionVector.getY(); }
		double getAngle() const { return m_MotionVector.getAngle(); }
		double getSpeed() const { return m_MotionVector.getSpeed(); }
		double getAcceleration() const { return m_MotionVector.getAcceleration(); }
		
		/***** Collision Detection *****/
		bool collision(Sprite& other);
		
	protected:
		/**
		 * This Sprite's MotionVector.
		 */
		MotionVector m_MotionVector;
		/**
		 * The current index of the SpriteFrame to display.
		 */
		int m_curFrameIndex;
		/**
		 * Whether or not this Sprite is currently animating.
		 */
		bool m_Animating;
		/**
		 * Whether or not this Sprite has been drawn at least once.
		 */
		bool m_Drawn;
		/**
		 * The speed for this Sprite's animation in frames / second.
		 */
		double m_AnimSpeed;
		/**
		 * The number of milliseconds since this Sprite last changed frames.
		 */
		int m_PrevTicks;
		/**
		 * This Sprite's SpriteBase.
		 */
		SpriteBase *m_SpriteBase;
		/**
		 * The screen this Sprite should draw itself to.
		 */
		SDL_Surface *m_Screen;
		/**
		 * The image this Sprite should replace itself with when it moves from
		 * its current position.
		 */
		SDL_Surface *m_BackReplacement;
		double m_PrevX;
		double m_PrevY;
};
 
#endif
 
