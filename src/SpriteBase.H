
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

#ifndef GUARD_SPRITEBASE_H
#define GUARD_SPRITEBASE_H

#include <vector>
#include <string>
#include "SpriteFrame.H"
 
class SpriteBase {
	public:
		SpriteBase(const std::string dir);
		~SpriteBase();
		
		int numOfFrames() const { return m_Frame.size(); }
		SpriteFrame& getFrame(int index) { return m_Frame[index]; }
		int getWidth() const { return m_Frame[0].getWidth(); }
		int getHeight() const { return m_Frame[0].getHeight(); }
	
	private:
		std::vector<SpriteFrame> m_Frame;
};

#endif
