// Eval.cc - source file for the Agoris program
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


#include <vector>
#include "Eval.hh"
#include "Board.hh"
#include "Square.hh"
extern "C" {
#include <math.h>
}


Eval::Eval() {
  brd::BitBoard bit = 1;
 
  for (unsigned long i = 0; i < 64; i++)
    mask[i] = bit << i;
}


double Eval::doEval(brd::Board* aBoard) {
  double curScore = 0;
  brd::Position curPos = aBoard->getBoard();

  for (int i = 0; i < 64; i++) {
    if ((int)curPos.square[i].getColor() == aBoard->getTurn()) {
      switch (curPos.square[i].getPiece()) {
      case PAWN:
	curScore += genPawnScore(aBoard, i);
	break;
      case ROOK:
	curScore += genRookScore(aBoard, i);
	break;
      case KNIGHT:
	curScore += genKnightScore(aBoard, i);
	break;
      case BISHOP:
	curScore += genBishopScore(aBoard, i);
	break;
      case QUEEN:
	curScore += genQueenScore(aBoard, i);
	break;
      case KING:
	curScore += genKingScore(aBoard, i);
	break;
      }
    }
  }

  // Material score
  curScore += genMaterialScore(aBoard);

  // Piece safety
  curScore += genPieceSafetyScore(aBoard);

  // Checks (has to be called AFTER piece safety and material score checks!)
  curScore += genChecksScore(aBoard);

  return curScore;
}


double Eval::genMaterialScore(brd::Board *aBoard) {
  double curScore = 0;
  brd::Position curPos = aBoard->getBoard();

  for (int i = 0; i < 64; i++) {
    if ((int)curPos.square[i].getColor() == aBoard->getTurn()) {
      switch (curPos.square[i].getPiece()) {
      case PAWN:
	curScore += PAWN_VAL;
	break;
      case ROOK:
	curScore += ROOK_VAL;
	break;
      case KNIGHT:
	curScore += KNIGHT_VAL;
	break;
      case BISHOP:
	curScore += BISHOP_VAL;
	break;
      case QUEEN:
	curScore += QUEEN_VAL;
	break;
      case KING:
	curScore += KING_VAL;
	break;
      }
    }
  }

  return curScore;
}


double Eval::genChecksScore(brd::Board *aBoard) {
  return aBoard->getChecks();
}


double Eval::genPieceSafetyScore(brd::Board *aBoard) {
  double score = 0;
  int* safetyBoard = aBoard->getSafetyBoard();

  for (int i = 0; i < 64; i++) {
    if (safetyBoard[i] >= 100)
      score += 0.3;
    else if ( safetyBoard[i] > 1 && safetyBoard[i] < 100 )
      score += 2;
    else
      score += safetyBoard[i];
  }
  
  return score;
}


double Eval::genPawnScore(brd::Board *aBoard, int pawnPos) {
  double curScore = 0;
  vector<brd::BitBoardMove> pawnMoves;
  vector<brd::BitBoardMove> pawnCaps;

  // Mobility
  pawnMoves = aBoard->genPawnMoves(pawnPos);
  pawnCaps = aBoard->genPawnCaptures(pawnPos);
  curScore += sqrt(pawnMoves.size() + pawnCaps.size() * 2);

  // Pawn credit
  if (aBoard->getTurn() == WHITE) {
    for (int i = 0; i < 64; i++) {
      if ( ((aBoard->getBoard()).whitePawns & mask[i]) && (i < 48) )
	curScore += 0.3;
    }
  }
  else {
    for (int i = 0; i < 64; i++) {
      if ( ((aBoard->getBoard()).blackPawns & mask[i]) && (i > 15) )
	curScore += 0.3;
    }
  }

  return curScore;
}


double Eval::genKnightScore(brd::Board *aBoard, int knightPos) {
  double curScore = 0;
  vector<brd::BitBoardMove> knightMoves;
  vector<brd::BitBoardMove> knightCaps;

  // Mobility
  knightMoves = aBoard->genKnightMoves(knightPos);
  knightCaps = aBoard->genKnightCaptures(knightPos);
  curScore += sqrt(knightMoves.size() + knightCaps.size() * 2);

  return curScore;
}


double Eval::genBishopScore(brd::Board *aBoard, int bishPos) {
  double curScore = 0;
  vector<brd::BitBoardMove> bishMoves;
  vector<brd::BitBoardMove> bishCaps;

  // Mobility
  bishMoves = aBoard->genBishopMoves(bishPos);
  bishCaps = aBoard->genBishopCaptures(bishPos);
  curScore += sqrt(bishMoves.size() + bishCaps.size() * 2);

  return curScore;
}


double Eval::genRookScore(brd::Board *aBoard, int rookPos) {
  double curScore = 0;
  vector<brd::BitBoardMove> rookMoves;
  vector<brd::BitBoardMove> rookCaps;

  // Mobility
  rookMoves = aBoard->genRookMoves(rookPos);
  rookCaps = aBoard->genRookCaptures(rookPos);
  curScore += sqrt(rookMoves.size() + rookCaps.size() * 2);

  return curScore;
}


double Eval::genQueenScore(brd::Board *aBoard, int queenPos) {
  double curScore = 0;
  vector<brd::BitBoardMove> queenMoves;
  vector<brd::BitBoardMove> queenCaps;

  // Mobility
  queenMoves = aBoard->genQueenMoves(queenPos);
  queenCaps = aBoard->genQueenCaptures(queenPos);
  curScore += sqrt(queenMoves.size() + queenCaps.size() * 2);

  return curScore;
}


double Eval::genKingScore(brd::Board *aBoard, int kingPos) {
  double curScore = 0;
  vector<brd::BitBoardMove> kingMoves;
  vector<brd::BitBoardMove> kingCaps;

  // Mobility
  kingMoves = aBoard->genKingMoves(kingPos);
  kingCaps = aBoard->genKingCaptures(kingPos);
  curScore += sqrt(kingMoves.size() + kingCaps.size() * 2);

  return curScore;
}
