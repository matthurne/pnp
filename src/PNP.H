
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

#ifndef GUARD_PONG_H
#define GUARD_PONG_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "Sprite.H"
#include "SpriteBase.H"
#include "Player.H"

class PNP {
	public:
		PNP(SDL_Surface *screen);
		~PNP();
		
		int run();

	private:
		SDL_Surface *m_Screen;
		SDL_Surface *m_Background;
		SDL_Surface *m_TextSurface;
		SDL_Surface *m_TextBGOne;
		SDL_Surface *m_TextBGTwo;
		SpriteBase m_PaddleBase;
		SpriteBase m_BallBase;
		Sprite m_LPaddle;
		Sprite m_RPaddle;
		Sprite m_Ball;
		Player m_PlayerOne;
		Player m_PlayerTwo;
		SDL_Color m_FontColor;
		TTF_Font *m_Font;
		SDL_Rect m_POneScoreRect;
		SDL_Rect m_PTwoScoreRect;
		bool m_IsPaused;
		double m_ProjectedY;
		bool m_YIsProjected;
		int m_DifficultyLevel;
		
		void initText();
		void redraw();
		void drawScores();
		Player* checkForWinner();
		void newRound();
		double projectY(double x, double y, double theta);
		void moveAI();
		void simpleMoveAI();
		SDL_Surface* imageLoad(const char *file) const;
		int randomInRange(int lowerBound, int upperBound);
};

#endif
