// Square.cc - source file for the Agoris program
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

#include "Square.hh"

Square::Square() {
  color = 0; piece = 0;
}

void Square::setColor(unsigned int newCol) {
  color = newCol;
}

unsigned int Square::getColor(void) {
  return color;
}

void Square::setPiece(unsigned int newPiece) {
  piece = newPiece;
}

unsigned int Square::getPiece(void) {
  return piece;
}
