
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
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstdlib>
#include "SDL.h"
#include "SDL_image.h"
#include "SpriteBase.H"

using std::cerr;
using std::flush;
using std::endl;
using std::vector;
using std::string;
using std::getline;
using std::ifstream;
using std::ostringstream;
using std::istringstream;
using std::isdigit;
using std::isspace;
using std::atoi;

SpriteBase::SpriteBase(const string dir)
{
	ostringstream oss;
	string filename;
	ifstream inFileStream;

	// The file describing this SpriteBase is called "info" within the directory
	// holding the sprite data which is provided by the caller
	filename = dir + "/info";
	
	// Open the file
	inFileStream.open(filename.c_str());
	if (!inFileStream) {
		cerr << "Could not open info file " << filename << endl;
		return;
	}
	
	// Skip non-digit characters
	char ch;
	while(inFileStream >> ch && !isdigit(ch)) {
		// Nothing to do here, we're just skipping anything that's not a digit
	}
	
	// Read in the number of frames for this SpriteBase
	while(isdigit(ch)) {
		oss << ch;
		inFileStream >> ch;
	}
	// The loop reads one past the last digit of the number, so we need to put
	// back the non-digit character that came after the last digit
	inFileStream.putback(ch);
	int totalFrameCount = atoi( (oss.str()).c_str() );
	if( !(totalFrameCount > 0) ) {
		cerr << "There was a problem reading the number of frames" << endl;
		return;
	}
	
	// Read in the information for each frame
	// The filename for the current frame's image
	string name;
	// The time to pause after displaying the current frame
	int delay = 0;
	// The RGB values for pixels in the frame image that should be rendered as
	// transparent
	int r = 0, g = 0, b = 0;
	// Temporary input buffer
	string buffer;
	int curFrameCount = 0;
	SpriteFrame *sf;

	// For each remaining line in the file or until we've read in the total
	// number of frames
	while(getline(inFileStream, buffer) && curFrameCount < totalFrameCount) {
		// Don't read this line if it is a comment, has leading whitespace, or
		// is an empty line
		if(buffer[0] != '#' && !isspace(buffer[0]) && buffer.size() != 0) {
			istringstream iss(buffer);
			// Read in the information for the current frame
			iss >> name >> delay >> r >> g >> b;
			// Build the full path and filename to the image file relative to
			// the base directory
			filename = dir + '/' + name;
			
			// Load the image
			SDL_Surface *temp;
			temp = IMG_Load(filename.c_str());
			if(temp == 0) {
				cerr << "Couldn't load frame image: " << SDL_GetError() << endl;
				return;
			}
			
			// The RGB value is only to be taken as transparency if the Red
			// component is not 0
			if(r >= 0) {
				// Tell SDL which color pixels in this image to render
				// transparent
				SDL_SetColorKey(temp, SDL_SRCCOLORKEY, SDL_MapRGB(temp->format, r, g, b));
			}
			
			// Construct and store the new SpriteFrame
			sf = new SpriteFrame( SDL_DisplayFormat(temp), delay );
			m_Frame.push_back(*sf);
			// Destroy the temporary image
			SDL_FreeSurface(temp);

			// On the the next frame, increase the number of frames that have
			// been created
			curFrameCount++;
		}//end if
	}//end while
	
	inFileStream.close();
}

SpriteBase::~SpriteBase() {
	// The SpriteFrames we allocated will be destroyed automatically by the
	// vector when this SpriteBase is destroyed . . . right?
}
