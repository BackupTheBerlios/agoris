// Search.hh - source file for the Agoris program
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

#ifndef _SEARCH_HH_
#define _SEARCH_HH_

#include "Board.hh"
#include "Timer.hh"

class Search {
private:
  brd::Board* theBoard;
  Timer clock;
  double maxTime;
  int minDepth;

public:
  Search();
  Search(brd::Board*);
  void initTimer(void);
  double alphaBeta(brd::Board*, double, double, int depth = 5);
  double miniMax(brd::Board*, int depth = 3);
  void setBoard(brd::Board*);
  void setMaxTime(double);
  void setMinDepth(int);
};

#endif
