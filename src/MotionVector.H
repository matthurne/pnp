
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

#ifndef GUARD_MOTIONVECTOR_H
#define GUARD_MOTIONVECTOR_H

/**
 * @brief A physics vector which uses SDL.
 *
 * An instance of the class keeps track not only of its direction, speed, and
 * acceleration, but also its location.  In addition, a MotionVector handles its
 * own movement when its move() and stop() methods are called.
 */
class MotionVector {
	public:
		/**
		 * @brief Default constructor.
		 */
		MotionVector();
		/**
		 * @brief Destructor.
		 */
		~MotionVector();
		
		
		/***** Movement *****/
		
		/**
		 * @brief Moves this MotionVector to a new position according to its
		 *        current position, direction, speed, and acceleration.
		 *
		 * Also changes this MotionVector's speed according to the amount of
		 * time since its last movement and its acceleration.
		 */
		void move();
		/**
		 * @brief Stops this MotionVector from moving any further.
		 *
		 * In fact, this method effectively resets this MotionVector's timer so
		 * that its position does not "jump" based on time elapsed while it was
		 * not moving.
		 */
		void stop() { m_isMoving = false; }
		
		
		/***** Properties *****/
		
		/**
		 * @brief Places this MotionVector at a new position.
		 *
		 * @param x the new x-coordinate
		 * @param y the new y-coordinate
		 */
		void setPosition(const double x, const double y) { m_X = x; m_Y = y; }
		/**
		 * @brief Places this MotionVector at a new x-coordinate without
		 *        changing its y-coordinate.
		 *
		 * @param x the new x-coordinate.
		 */
		void setX(const double x) { m_X = x; }
		/**
		 * @brief Places this MotionVector at a new y-coordinate without
		 *        changing its x-coordinate.
		 *
		 * @param y the new y-coordinate
		 */
		void setY(const double y) { m_Y = y; }
		/**
		 * @brief Changes the direction of this MotionVector.
		 *
		 * The new direction is specified as an angle measured in radians.
		 *
		 * @param angle the new direction as an angle measured in radians
		 */
		void setAngle(const double angle) { m_Angle = angle; }
		/**
		 * @brief Changes the speed of this MotionVector.
		 *
		 * The new speed is specified with the units coordinates per second.
		 * This could be easily used as pixels per second, or could be scaled to
		 * the desired units if necessary.
		 *
		 * @param speed the new speed
		 */
		void setSpeed(const double speed) { m_Speed = speed; }
		/**
		 * @brief Changes the acceleration rate of this MotionVector.
		 *
		 * The new acceleration rate is specified with the units coordinates per
		 * second squared.
		 *
		 * @param acceleration the new acceleration rate
		 */
		void setAcceleration(const double acceleration) { m_Acceleration = acceleration; }
		/**
		 * @brief Gets the x-coordinate of this MotionVector.
		 *
		 * @return the current x-coordinate
		 */
		double getX() const { return m_X; }
		/**
		 * @brief Gets the y-coordinate of this MotionVector.
		 *
		 * @return the current y-coordinate
		 */
		double getY() const { return m_Y; }
		/**
		 * @brief Gets this MotionVector's direction.
		 *
		 * The direction is returned as an angle measured in radians.
		 *
		 * @return the current direction as an angle measured in radians
		 */
		double getAngle() const { return m_Angle; }
		/**
		 * @brief Gets this MotionVector's speed.
		 *
		 * The speed is measured in coordinates per second.
		 *
		 * @return the current speed
		 */
		double getSpeed() const { return m_Speed; }
		/**
		 * @brief Gets this MotionVector's acceleration rate.
		 *
		 * The acceleration rate is measured in coordinates per second squared.
		 *
		 * @return the current acceleration rate
		 */
		double getAcceleration() const { return m_Acceleration; }
		/**
		 * @brief Checks whether this MotionVector is keeping track of the
		 *        time that has elapsed since its last move.
		 *
		 * In fact, this MotionVector's coordinates only change to reflect its
		 * movement when its move() method is called.  If this MotionVector is
		 * set to be moving, the next time its move() method is called it will
		 * move the proper distance from its position the last time move() was
		 * called.  This avoids this MotionVector needing its own thread while
		 * maintaining framerate-independent motion.
		 *
		 * @return true if this MotionVector is "moving", false otherwise
		 */
		bool isMoving() const { return m_isMoving; }

	private:
		/**
		 * @brief This MotionVector's x-coordinate.
		 */
		double m_X;
		/**
		 * @brief This MotionVector's y-coordinate.
		 */
		double m_Y;
		/**
		 * @brief The direction of this MotionVector as an angle measured in
		 *        radians.
		 */
		double m_Angle;
		/**
		 * @brief This MotionVector's speed in coordinates per second.
		 */
		double m_Speed;
		/**
		 * @brief This MotionVector's acceleration in coordinates per second
		 *        squared.
		 */
		double m_Acceleration;
		/**
		 * @brief The SDL tick count in milliseconds the last time this
		 *        MotionVector's move() method was called.
		 *
		 * This is used to support framerate
		 * independent motion; the distance this MotionVector's new coordinates
		 * will be from its current coordinates depends on the time that elapses
		 * in between the calls to move() and this MotionVector's speed.
		 */
		int m_PrevTicks;
		/**
		 * @brief Whether or not this MotionVector should keep track of the time
		 *        of its last movement.
		 *
		 * If this MotionVector is "moving", it needs to keep track of the time
		 * it last moved in order to determine the next position to move to.
		 */
		bool m_isMoving;
};

#endif
