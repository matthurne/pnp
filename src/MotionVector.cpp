
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

#include <cmath>
#include "SDL.h"
#include "MotionVector.H"

MotionVector::MotionVector()
:
m_X(0.0),
m_Y(0.0),
m_Angle(0.0),
m_Speed(100.0),
m_Acceleration(0.0),
m_PrevTicks(0),
m_isMoving(false)
{
}

MotionVector::~MotionVector() {
}

void MotionVector::move() {
	if(!m_isMoving) {
		m_PrevTicks = SDL_GetTicks();
		m_isMoving = true;
	}

	// Get the ticks since SDL was initialized
	int curTicks = SDL_GetTicks();
	// Calculate the ticks since the last time this MotionVector was moved
	int dt = curTicks - m_PrevTicks;
	m_PrevTicks = curTicks;

	double seconds = (double)dt / 1000;
	// Calculate the distance to move in pixels
	double distance = m_Speed * seconds;
	
	// Time for some trigonometry . . .
	double dx, dy;
	dx = distance * cos(m_Angle);
	dy = distance * -sin(m_Angle); // -sin because the coordinate system is upside-down
	// Move this MotionVector!
	m_X += dx;
	m_Y += dy;
	
	// Calculate the new speed based on the current acceleration
	m_Speed = m_Speed + m_Acceleration * seconds;
}
