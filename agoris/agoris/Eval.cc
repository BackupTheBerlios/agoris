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

using namespace std;

//! Standard constructor
Eval::Eval() {
  brd::BitBoard bit = 1;
 
  for (unsigned long i = 0; i < 64; i++)
    mask[i] = bit << i;
}


//! Evaluate current board situation for current player
/** This method returns a score associated with the board situation.
 *  It considers material values, piece safety, check possibilities, etc.
 *  @param aBoard is a pointer to the current chess board
 *  @return The score associated with the current game situation, seen from the current player's viewpoint
 *  @see genMaterialScore()
 *  @see genPieceSafetyScore()
 *  @see genChecksScore()
 *  @see genPromotionsScore()
 *  @see genCastlingScore()
 */
double Eval::doEval(brd::Board* aBoard) {
  double curScore = 0;
  brd::Position curPos = aBoard->getBoard();

  // Mobility
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

  // Score for possible pawn promotions
  curScore += genPromotionsScore(aBoard);

  // Score if castling is still possible
  curScore += genCastlingScore(aBoard);

  return curScore;
}


//! Generate material score for the current player
/** This method evaluates the material score for the current player. It adds material values for each piece
 *  and returns the total score.
 *  @param aBoard is a pointer to the current chess board
 *  @return The material score
 */
double Eval::genMaterialScore(brd::Board *aBoard) {
  double curScore = 0;
  brd::Position curPos = aBoard->getBoard();

  for (int i = 0; i < 64; i++) {
    if ((int)curPos.square[i].getColor() == aBoard->getTurn()) {
      switch (curPos.square[i].getPiece()) {
      case PAWN:
	curScore += aBoard->getPieceValue(PAWN);
	break;
      case ROOK:
	curScore += aBoard->getPieceValue(ROOK);
	break;
      case KNIGHT:
	curScore += aBoard->getPieceValue(KNIGHT);
	break;
      case BISHOP:
	curScore += aBoard->getPieceValue(BISHOP);
	break;
      case QUEEN:
	curScore += aBoard->getPieceValue(QUEEN);
	break;
      case KING:
	curScore += aBoard->getPieceValue(KING);
	break;
      }
    }
  }

  return curScore;
}


//! Generate score for castling possbility
/** This method returns a positive score if castling is still possible
 *  @param aBoard is a pointer to the current chess board
 *  @return Returns 1 if castling is possible, 0 otherwise
 */
double Eval::genCastlingScore(brd::Board *aBoard) {
  if (aBoard->getTurn() == WHITE && aBoard->isWhiteCastlingPossible())
    return 1;
  else if (aBoard->getTurn() == BLACK && aBoard->isBlackCastlingPossible())
    return 1;
  else
    return 0;
}


//! Generate score for pawn promotions
/** This method returns a positive score if promotions are possible.
 *  @param aBoard is a pointer to the current chess board
 *  @return Returns the number of possible promotions times QUEEN_VAL (material value for the queen)
 */
double Eval::genPromotionsScore(brd::Board *aBoard) {
  return (double)(aBoard->getPromotions() * aBoard->getPieceValue(QUEEN));
}


//! Generate score for possible checks
/**
 *  @param aBoard is a pointer to the current chess board
 *  @return Returns the number of possible checks
 */
double Eval::genChecksScore(brd::Board *aBoard) {
  return aBoard->getChecks();
}


//! Generate score for piece safety
/** This method returns a positive score if own pieces are protected by own pieces.
 *  It adds a special bonus if it's a pawn that protects another piece.
 *  @param aBoard is a pointer to the current chess board
 *  @return Returns the score for protected own pieces
 */
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
