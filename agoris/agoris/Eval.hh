// Eval.hh - source file for the Agoris program
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

#include "Board.hh"

class Eval {
private:
  brd::BitBoard mask[64];
  double genChecksScore(brd::Board*);
  double genPromotionsScore(brd::Board*);
  double genMaterialScore(brd::Board*);
  double genPieceSafetyScore(brd::Board*);
  double genPawnScore(brd::Board*, int);
  double genKnightScore(brd::Board*, int);
  double genBishopScore(brd::Board*, int);
  double genRookScore(brd::Board*, int);
  double genQueenScore(brd::Board*, int);
  double genKingScore(brd::Board*, int);

public:
  Eval();
  double doEval(brd::Board*);
};
