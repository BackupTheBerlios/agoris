// Game.hh - source file for the Agoris program
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
#include "Search.hh"

class Game {
private:
  brd::Board theBoard;
  Search boardSearch;
  bool humanColor;

public:
  Game();
  void nextTurn(void);
  void changeSides(void);
  bool getAIColor(void);
  bool isValidMove(brd::Move);
  int makeMove(brd::Move);
  brd::Move calculateMove(int depth = 3);
  brd::Position getBoard(void);
  brd::Move getBestMove(void);
  int getCheckmate(void);
};
