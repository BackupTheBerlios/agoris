// Game.cc - source file for the Agoris program
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

#include "Game.hh"
#include "Board.hh"


Game::Game() {
  humanColor = true;
  boardSearch.setBoard(&theBoard);
}


void Game::nextTurn(void) {
  theBoard.nextTurn();
}


void Game::changeSides(void) {
  humanColor = !humanColor;
}


bool Game::getAIColor(void) {
  return !humanColor;
}


int Game::makeMove(brd::Move newMove) {
  return theBoard.makeMove(newMove);
}


bool Game::isValidMove(brd::Move newMove) {
  return theBoard.isValidMove(newMove);
}


brd::Move Game::calculateMove(int depth = 3) {
  boardSearch.miniMax(&theBoard, depth);
  return theBoard.getBestMove();
}


brd::Position Game::getBoard(void) {
  return theBoard.getBoard();
}


brd::Move Game::getBestMove(void) {
  return theBoard.getBestMove();
}


int Game::getCheckmate(void) {
  return theBoard.getCheckmate();
}
