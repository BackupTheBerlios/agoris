// Search.cc - source file for the Agoris program
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

#include <iostream>
#include <string>
#include "Search.hh"
#include "Board.hh"
#include "Eval.hh"


Search::Search() {
}


Search::Search(brd::Board* newBoard) {
  theBoard = newBoard;
}


void Search::setBoard(brd::Board* newBoard) {
  theBoard = newBoard;
}


double Search::alphaBeta(brd::Board* vBoard, double alpha, double beta, int depth = 5) {
  if (depth <= 0) {
    Eval AI;
    return AI.doEval(vBoard);
  }
  
  double score = 0;
  double bestScore = -(vBoard->getPieceValue(INFINITY));
  int leftOuts = 0;
  brd::Board newVBoard;
  vBoard->genMoves();

  for (unsigned int i = 0; i < vBoard->getMoves().size() && bestScore < beta; i++) {
    // Do not allow illegal moves, such as those that lead right into check mate, etc.
    if (!vBoard->isValidMove(vBoard->getArrayMove(i))) {
      leftOuts++;
      continue;
    }
    
    vBoard->doArrayMove(i);
    newVBoard.setBoard(vBoard->getBoard());
    vBoard->undoMove();

    if (vBoard->getTurn() == WHITE)
      newVBoard.setTurn(BLACK);
    else
      newVBoard.setTurn(WHITE);
    
    if (bestScore > alpha)
      alpha = bestScore;

    score = -alphaBeta(&newVBoard, -beta, -alpha, depth - 1);

    if (score > bestScore) {
      bestScore = score;
      vBoard->setBestMove(vBoard->getArrayMove(i));

#ifdef DEBUG
      cout << vBoard->getBestMove().source().x << ":" << vBoard->getBestMove().source().y << " - " <<
	vBoard->getBestMove().dest().x << ":" << vBoard->getBestMove().dest().y << endl;
#endif
    }
  }
  
  // See whether we are check mate
  if (leftOuts == (int)vBoard->getMoves().size())
    vBoard->setCheckmate(vBoard->getTurn());

  return bestScore;
}


double Search::miniMax(brd::Board* vBoard, int depth = 3) {
  if (depth == 0) {
    Eval AI;
    return AI.doEval(vBoard);
  }

  double score = 0;
  double bestScore = -INFINITY;
  int leftOuts = 0;

  brd::Board newVBoard;
  vBoard->genMoves();
  
  for (unsigned int i = 0; i < vBoard->getMoves().size(); i++) {
    // Do not allow illegal moves, such as those that lead right into check mate, etc.
    if (!vBoard->isValidMove(vBoard->getArrayMove(i))) {
      leftOuts++;
      continue;
    }
    
    vBoard->doArrayMove(i);
    newVBoard.setBoard(vBoard->getBoard());
    
    if (vBoard->getTurn() == WHITE)
      newVBoard.setTurn(BLACK);
    else
      newVBoard.setTurn(WHITE);
    
    score = -miniMax(&newVBoard, depth - 1);
    
    if (score > bestScore) {
      bestScore = score;
      vBoard->setBestMove(vBoard->getArrayMove(i));
    }
    
    vBoard->undoMove();
  }

  // See whether we are check mate
  if (leftOuts == (int)vBoard->getMoves().size())
    vBoard->setCheckmate(vBoard->getTurn());

  return bestScore;
}
