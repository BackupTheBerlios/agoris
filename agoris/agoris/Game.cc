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
#include "Square.hh"


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


void Game::makeMove(brd::Move newMove) {
  theBoard.makeMove(newMove);
}


//! Checks whether newMove is valid for the current player
bool Game::isValidMove(brd::Move newMove) {
  if (theBoard.isValidMove(newMove)) {
    brd::Board tempBoard;
    tempBoard.setBoard(theBoard.getBoard());
    tempBoard.setTurn(WHITE);
    tempBoard.genMoves();

    for (unsigned int i = 0; i < tempBoard.getMoves().size(); i++)
      if (tempBoard.getArrayMove(i).source().x == newMove.source().x && tempBoard.getArrayMove(i).source().y == newMove.source().y &&
	  tempBoard.getArrayMove(i).dest().x == newMove.dest().x && tempBoard.getArrayMove(i).dest().y == newMove.dest().y )
	return true;
  }    
  
  return false;
}


//! Calculate best possible move with a given search depth and a given algorithm
/** This function generates the best move for the computer player.
 *  It lets you choose the algorithm to use and also what search depth to use.
 *  @param algorithm 0 if minimax algorithm is prefered, 1 for alpha-beta pruning
 *  @param depth is the search depth
 *  @return The best possible move for the computer player
 */
brd::Move Game::calculateMove(int algorithm, int depth = 3) {
  if (algorithm == 0)
    boardSearch.miniMax(&theBoard, depth);
  else
    boardSearch.alphaBeta(&theBoard, -INFINITY, INFINITY, depth);

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
