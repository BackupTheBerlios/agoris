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

using namespace brd;

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
  bool isValidMove(Move);
  void makeMove(Move);
  Move calculateMove(int, int depth = 3);
  Position getBoard(void);
  Move getBestMove(void);
  int getCheckmate(void);
  
  void setPawnValue(double val = 1);
  void setKnightValue(double val = 3);
  void setBishopValue(double val = 3.5);
  void setRookValue(double val = 5);
  void setQueenValue(double val = 10);
  void setKingValue(double val = 10000);

  double pawnVal(void);
  double knightVal(void);
  double bishopVal(void);
  double rookVal(void);
  double queenVal(void);
  double kingVal(void);
};
