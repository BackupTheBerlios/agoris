// Square.hh - source file for the Agoris program
// Copyright (c) 2001  Andreas Bauer <baueran@in.tum.de>
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
// USA.

#ifndef __SQUARE_HH_
#define __SQUARE_HH_

#define PAWN     0
#define KNIGHT   1
#define BISHOP   2
#define ROOK     3
#define QUEEN    4
#define KING     5
#ifndef EMPTY
#define EMPTY    6
#endif

#define INFINITY     10000000
#define PAWN_VAL     1
#define KNIGHT_VAL   3
#define BISHOP_VAL   3.5
#define ROOK_VAL     5
#define QUEEN_VAL    10
#define KING_VAL     10000


class Square {
private:
  unsigned int color;
  unsigned int piece;

public:
  Square();
  void setColor(unsigned int);
  unsigned int getColor(void);
  void setPiece(unsigned int);
  unsigned int getPiece(void);
};

#endif
