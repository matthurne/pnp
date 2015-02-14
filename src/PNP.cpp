
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
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "PNP.H"
#include "Sprite.H"
#include "SpriteBase.H"
#include "Player.H"
#include "constants.H"

/* ---------- PUBLIC METHODS -------- */
PNP::PNP(SDL_Surface *screen)
:
m_Screen(screen),
m_Background(0),
m_TextSurface(0),
m_TextBGOne(0),
m_TextBGTwo(0),
m_PaddleBase("data/paddle"),
m_BallBase("data/ball"),
m_LPaddle(&m_PaddleBase, m_Screen),
m_RPaddle(&m_PaddleBase, m_Screen),
m_Ball(&m_BallBase, m_Screen),
m_PlayerOne("Player One"),
m_PlayerTwo("Player Two"),
m_Font(0),
m_IsPaused(true),
m_ProjectedY(m_Screen->h / 2),
m_YIsProjected(false),
m_DifficultyLevel(0)
{
	// Load the background image
	m_Background = imageLoad("data/background.jpg");
	// Set up the text system (player score surfaces, etc.)
	initText();
	// Set the movement speed of the paddles in pixels/second
	m_LPaddle.setSpeed(200.0);
	m_RPaddle.setSpeed(200.0);
	
	// Initialize the random number generator
	std::srand( static_cast<unsigned>(std::time(0)) );
}

PNP::~PNP() {
	// Free up resources - there aren't destructors for everything because SDL
	// is a C library!
	SDL_FreeSurface(m_Background);
	SDL_FreeSurface(m_TextSurface);
	SDL_FreeSurface(m_TextBGOne);
	SDL_FreeSurface(m_TextBGTwo);
	TTF_CloseFont(m_Font);
}

int PNP::run() {
	// Draw the background
	SDL_BlitSurface(m_Background, 0, m_Screen, 0);
	
	newRound();

	// Game loop flag
	bool finished = false;
	
	// Main game loop
	while(!finished) {

		SDL_Event event;
		// Check for events - if there are events on the queue SDL_PollEvent
		// puts the next event into its SDL_event parameter and returns true.
		// Onces all the events on the queue have been read and processed, it
		// will return false and the loop will finish.
		while ( SDL_PollEvent(&event) ) {
			switch (event.type) {
				case SDL_QUIT:
                    finished = true;
                    break;
                    
				case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							finished = true;
							break;
							
                        case SDLK_SPACE:
							m_IsPaused = !m_IsPaused;
							break;

						case SDLK_d:
							m_DifficultyLevel++;
							if(m_DifficultyLevel >= 2) {
								m_DifficultyLevel = 0;
							}
					}
					break;
			}
    	}
		
		// Check whether the game is paused; if it is, stop all Sprites from
		// moving.  Otherwise, work out which Sprites should move and where and
		// move them.
		if (m_IsPaused) {
			m_Ball.stop();
			m_LPaddle.stop();
			m_RPaddle.stop();
		} else {
            // Get the state of the keyboard as of the last call to
			// SDL_PumpEvents(), which is called by SDL_PollEvent() so we don't
			// need to call it here, we can just get the state.
			Uint8 *keys;
			keys = SDL_GetKeyState(0);
			
			if ( keys[SDLK_UP] ) {
            	// UP ARROW key  - move the player's paddle up
				m_LPaddle.setAngle(PI / 2);
				m_LPaddle.move();
				if( (m_LPaddle.getY() - m_LPaddle.getHeight() / 2) < 0 ) {
					m_LPaddle.setY(0 + m_LPaddle.getHeight() / 2);
				}
			} else if ( keys[SDLK_DOWN] ) {
				// DOWN ARROW key - move the player's paddle down
            	m_LPaddle.setAngle(3 * PI / 2);
				m_LPaddle.move();
				if( (m_LPaddle.getY() + m_LPaddle.getHeight() / 2) > m_Screen->h ) {
					m_LPaddle.setY(m_Screen->h - m_LPaddle.getHeight() / 2);
				}
			} else {
				m_LPaddle.stop();
			}
			
        	// Move the AI player
        	if(m_DifficultyLevel == 0) {
				simpleMoveAI();
			} else {
				moveAI();
			}
			
			// Move the ball!
			m_Ball.move();
		
			//Check for collisions
			//Ball collided with left paddle
			if( m_Ball.collision(m_LPaddle) ) {
				m_Ball.setX( m_LPaddle.getX() + (m_LPaddle.getWidth() / 2) + (m_Ball.getWidth() / 2) );
				double angleIncrement = (PI / 4) / (m_LPaddle.getHeight() / 2);
				double multiplier = m_LPaddle.getY() - m_Ball.getY();
				double angle = multiplier * angleIncrement;
				if(angle < 0) {
					angle += 2 * PI;
				}
				m_Ball.setAngle(angle);
			}
			//Ball collided with right paddle
			if( m_Ball.collision(m_RPaddle) ) {
            	m_Ball.setX( m_RPaddle.getX() - (m_RPaddle.getWidth() / 2) - (m_Ball.getWidth() / 2) );
				double angleIncrement = (PI / 4) / (m_RPaddle.getHeight() / 2);
				double multiplier = m_RPaddle.getY() - m_Ball.getY();
				double angle = multiplier * angleIncrement;
				if(angle < 0) {
					angle += 2 * PI;
					m_Ball.setAngle( 3 * PI - angle );
				} else {
					m_Ball.setAngle( PI - angle );
				}
				m_YIsProjected = false;
			}
		
			//Ball collided with left wall
			if( (m_Ball.getX() - m_Ball.getWidth() / 2 ) <= 0 ) {
				//Point for PlayerTwo, start new round
				m_PlayerTwo.scored();
				newRound();
			}
			//Ball collided with right wall
			if( (m_Ball.getX() + m_Ball.getWidth() / 2) >= m_Screen->w ) {
				//Point for PlayerOne, start new round
				m_PlayerOne.scored();
				newRound();
			}
		
			//Ball collided with top wall
			if( (m_Ball.getY() - m_Ball.getHeight() / 2) <= 0 ) {
				// Mirror reflect
				m_Ball.setY(m_Ball.getHeight() / 2);
				m_Ball.setAngle( 2 * PI - m_Ball.getAngle() );
			}
			//Ball collided with bottom wall
			if( (m_Ball.getY() + m_Ball.getHeight() / 2) >= m_Screen->h ) {
				// Mirror reflect
				m_Ball.setY( m_Screen->h - (m_Ball.getHeight() / 2) );
				m_Ball.setAngle( 2 * PI - m_Ball.getAngle() );
			}
		}//end if

		// Redraw everything
		redraw();
		
	}//end while
	
	// Return successful exit status
	return 0;
}


/* ---------- PRIVATE METHODS -------- */

void PNP::initText() {
	// The text color in RGB format: white
	m_FontColor.r = 255;
	m_FontColor.g = 255;
	m_FontColor.b = 255;

	// Open and initialize the font
	//                    file               , ptsize
	m_Font = TTF_OpenFont("data/ttf/Vera.ttf", 32);
	if (!m_Font) {
		std::cerr << "Could not open font: " << TTF_GetError() << std::endl;
	}

	// Create an initial surface with a score so we can use its width and height
	// for our background surfaces
	//                                    font, text, fg
	SDL_Surface *temp = TTF_RenderText_Blended(m_Font, "0", m_FontColor);
	if(!temp) {
		std::cerr << "Could not render text: " << TTF_GetError() << std::endl;
	}
	// Make sure we are using the display surface format
	SDL_Surface *temp2 = SDL_DisplayFormat(temp);
	// Free our first temporary surface
	SDL_FreeSurface(temp);
	
	// Create the player score background surfaces, using the temporary
	// surface's dimensions and format
	m_TextBGOne = SDL_CreateRGBSurface(SDL_HWSURFACE,            // flags
										temp2->w,                // width
										temp2->h,                // height
										16,                      // depth
										temp2->format->Rmask,    // Rmask
										temp2->format->Gmask,    // Gmask
										temp2->format->Bmask,    // Bmask
										temp2->format->Amask);   // Amask
	m_TextBGTwo = SDL_CreateRGBSurface(SDL_HWSURFACE,            // flags
										temp2->w,                // width
										temp2->h,                // height
										16,                      // depth
										temp2->format->Rmask,    // Rmask
										temp2->format->Gmask,    // Gmask
										temp2->format->Bmask,    // Bmask
										temp2->format->Amask);   // Amask

	// Define the rectangle areas the player scores will be placed on the screen
	m_POneScoreRect.x = 25;       m_POneScoreRect.y = 10;
	m_POneScoreRect.w = temp2->w; m_POneScoreRect.h = temp2->h;
	m_PTwoScoreRect.x = m_Screen->w - temp2->w - 25;      m_PTwoScoreRect.y = 10;
	m_PTwoScoreRect.w = temp2->w; m_PTwoScoreRect.h = temp2->h;

	// Free the second temporary surface
	SDL_FreeSurface(temp2);

	// Store the backgrounds of the player scores for clearing and updating the
	// scores later
	SDL_BlitSurface(m_Background, &m_POneScoreRect, m_TextBGOne, 0);
	SDL_BlitSurface(m_Background, &m_PTwoScoreRect, m_TextBGTwo, 0);
}

void PNP::redraw() {
    // Redraw the player scores
	drawScores();
	// Redraw sprites
	m_Ball.clearBG();
	m_LPaddle.clearBG();
	m_RPaddle.clearBG();
	m_Ball.updateBG();
	m_LPaddle.updateBG();
	m_RPaddle.updateBG();
	m_Ball.draw();
	m_LPaddle.draw();
	m_RPaddle.draw();
	// Update the screen
	SDL_Flip(m_Screen);
}

void PNP::drawScores() {
	// Clear the scores
	SDL_BlitSurface(m_TextBGOne, 0, m_Screen, &m_POneScoreRect);
	SDL_BlitSurface(m_TextBGTwo, 0, m_Screen, &m_PTwoScoreRect);
	
	// We'll use these to convert the scores to char*'s
    std::ostringstream ossOne, ossTwo;
    
    // Draw PlayerOne's score
	ossOne << m_PlayerOne.getScore();
	if(!(m_TextSurface = TTF_RenderText_Blended(m_Font, ossOne.str().c_str(), m_FontColor))) {
		std::cerr << "Could not render text: " << TTF_GetError() << std::endl;
	} else {
		SDL_BlitSurface(m_TextSurface, 0, m_Screen, &m_POneScoreRect);
		SDL_FreeSurface(m_TextSurface);
	}
	
	// Draw PlayerTwo's score
	ossTwo << m_PlayerTwo.getScore();
	if(!(m_TextSurface = TTF_RenderText_Blended(m_Font, ossTwo.str().c_str(), m_FontColor))) {
		std::cerr << "Could not render text: " << TTF_GetError() << std::endl;
	} else {
		SDL_BlitSurface(m_TextSurface, 0, m_Screen, &m_PTwoScoreRect);
		SDL_FreeSurface(m_TextSurface);
	}
}

Player* PNP::checkForWinner() {
	if(m_PlayerOne.getScore() >= 5) {
		return &m_PlayerOne;
	} else if(m_PlayerTwo.getScore() >= 5) {
		return &m_PlayerTwo;
	}
	return 0;
}

void PNP::newRound() {
	// Check for a winner
	Player *winner = checkForWinner();
	if(winner != 0) {
		m_PlayerOne.resetScore();
		m_PlayerTwo.resetScore();
	}
	
	// Reposition each sprite
	
	m_LPaddle.setPosition( 15, m_Screen->h / 2 );
	//m_LPaddle.setPosition( 15, 500 );//TEMP
	m_RPaddle.setPosition( m_Screen->w - 15, m_Screen->h / 2 );
	m_Ball.setPosition( m_Screen->w / 2, m_Screen->h / 2 );
	//m_Ball.setPosition( 20, 275 );//TEMP

	// Reset the ball's direction, speed and acceleration rate
	
	m_Ball.setAngle(PI);
	//m_Ball.setAngle(0.316 * PI);//TEMP
	m_Ball.setSpeed(400.0);
	m_Ball.setAcceleration(10.0);
	
	// Pause the game so the player can get ready for the intense action
	m_IsPaused = true;
}

double PNP::projectY(double xParam, double yParam, double thetaParam) {
    // During trig calculations, a is the vertical side, b is the horizontal
	// side and c is the hypotenuse.
	double a, b, c, theta;
	// x and y will be actual coordinates based on our calculations.
	double x, y;

	x = xParam;
	y = yParam;
	theta = thetaParam;

	b = m_RPaddle.getX() - xParam;

	if( theta > 0 && theta < (PI / 2) ) {
	// The ball is moving upward.
        // Known: theta, b
		// Find: c, a
		// c = b / cos(theta)
		// a = c * sin(theta)
		c = b / std::cos(theta);
		a = c * std::sin(theta);

		y = yParam - a;

		if(y < 0) {
		// The projected y-coordinate is off the playing field, so it will be
		//   reflected off the top wall.  So we know that at the point of
		//   reflection, y will be 0.  We need to find the new x-coordinate. We
		//   will use the smaller triangle formed between the ball and the point
		//   of reflection to do so.
		    y = 0;
		    // The length of the vertical side of the small triangle.
		    a = yParam - y;

		    // Known: theta, a
			// Find: c, b
			// c = a / sin(theta)
			// b = c * cos(theta)
			
			// The length of the hypotenuse of the small triangle.
			c = a / std::sin(theta);
			// The lenght of the horizontal side of the small triangle.
			b = c * std::cos(theta);
			// The new x-coordinate will be the current x-coordinate plus the
			//   horizontal side of the small triangle.
			x += b;
			
			// The angle must be reflected.
			theta = 2 * PI - theta;
			
			// Do everything over again, this time with the ball at the point of
			// reflection we just calculated, and headed downward.
			y = projectY(x, y, theta);
		}
	} else if( theta < (2 * PI) && theta > (3 * PI / 2) ) {
	// The ball is moving downward.
        // Known: theta, b
		// Find: c, a
		// c = b / cos(theta)
		// a = c * sin(theta)
		c = b / std::cos(theta);
		a = c * -( std::sin(theta) );

		y = yParam + a;

		if(y >= m_Screen->h) {
		// The projected y-coordinate is off the playing field, so it will be
		//   reflected off the bottom wall.  So we know that at the point of
		//   reflection, y will be m_Screen->h.  We need to find the new
		//   x-coordinate. We will use the smaller triangle formed between the
		//   ball and the point of reflection to do so.
		    // The last y-coordinate of the screen is 1 less than the height of
		    //   the screen; 0-based indexing!
		    y = m_Screen->h - 1;
		    // The length of the vertical side of the small triangle.
		    a = y - yParam;

		    // Known: theta, a
			// Find: c, b
			// c = a / sin(theta)
			// b = c * cos(theta)

			// The length of the hypotenuse of the small triangle.
			c = a / -( std::sin(theta) );
			// The lenght of the horizontal side of the small triangle.
			b = c * std::cos(theta);
			// The new x-coordinate will be the current x-coordinate plus the
			//   horizontal side of the small triangle.
			x += b;

			// The angle must be reflected.
			theta = 2 * PI - theta;

			// Do everything over again, this time with the ball at the point of
			// reflection we just calculated, and headed downward.
			y = projectY(x, y, theta);
		}
	}
	// Return the y-coordinate when it is on the playing field.
	return y;
}

void PNP::moveAI() {
	if( m_Ball.getAngle() > 0 && (m_Ball.getAngle() < (PI / 2) || m_Ball.getAngle() > (3 * PI / 2)) ) {
		if (!m_YIsProjected) {
			m_ProjectedY = projectY( m_Ball.getX(), m_Ball.getY(), m_Ball.getAngle() );
			m_YIsProjected = true;
		}
	
		double diff = m_ProjectedY - m_RPaddle.getY();

		if( diff >  randomInRange(1, 32) ) {
			// If the ball is below the paddle, move down
    		m_RPaddle.setAngle(3 * PI / 2);
			m_RPaddle.move();
			if( (m_RPaddle.getY() + m_RPaddle.getHeight() / 2) > m_Screen->h ) {
				m_RPaddle.setY(m_Screen->h - m_RPaddle.getHeight() / 2);
			}
		} else if(diff < -randomInRange(1, 32) ) {
        	// If the ball is above the paddle, move up
        	m_RPaddle.setAngle(PI / 2);
			m_RPaddle.move();
			if( (m_RPaddle.getY() - m_RPaddle.getHeight() / 2) < 0 ) {
				m_RPaddle.setY(0 + m_RPaddle.getHeight() / 2);
			}
		} else {
			m_RPaddle.stop();
		}
	} else {
		if( m_RPaddle.getY() < (m_Screen->h / 2 - 5) ) {
		// Ball is above center of screen, move down
			m_RPaddle.setAngle(3 * PI / 2);
			m_RPaddle.move();
		} else if ( m_RPaddle.getY() > (m_Screen->h / 2 + 5) ) {
		// Ball is below center of screen, move up
			m_RPaddle.setAngle(PI / 2);
			m_RPaddle.move();
		} else {
			m_RPaddle.stop();
		}
	}
}

void PNP::simpleMoveAI() {
    double diff = m_Ball.getY() - m_RPaddle.getY();

	if(diff > 1) {
        // If the ball is below the paddle, move down
    	m_RPaddle.setAngle(3 * PI / 2);
		m_RPaddle.move();
		if( (m_RPaddle.getY() + m_RPaddle.getHeight() / 2) > m_Screen->h ) {
			m_RPaddle.setY(m_Screen->h - m_RPaddle.getHeight() / 2);
		}
	} else if(diff < -1) {
        // If the ball is above the paddle, move up
        m_RPaddle.setAngle(PI / 2);
		m_RPaddle.move();
		if( (m_RPaddle.getY() - m_RPaddle.getHeight() / 2) < 0 ) {
			m_RPaddle.setY(0 + m_RPaddle.getHeight() / 2);
		}
	} else {
		m_RPaddle.stop();
	}
}

SDL_Surface* PNP::imageLoad(const char *file) const {
	SDL_Surface *temp1, *temp2;
	temp1 = IMG_Load(file);
	temp2 = SDL_DisplayFormat(temp1);
	SDL_FreeSurface(temp1);
	return temp2;
}

int PNP::randomInRange(int lowerBound, int upperBound)
{
    if(lowerBound > upperBound){
        std::swap(lowerBound, upperBound);
    }
    int range = upperBound - lowerBound + 1;
    return lowerBound + int(range * std::rand() / (RAND_MAX + 1.0) );
}
