// Board.cc - source file for the Agoris program
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
#include <vector>
extern "C" {
#include <stdlib.h>
#include <math.h>
}
#include "Board.hh"

using namespace std;

namespace brd {

  /// Constructor, set original and destination locations to zero.
  BitBoardMove::BitBoardMove() {
    source = 0;
    dest = 0;
    score = 0;
  }

  /// Constructor, set location values to zero.
  Location::Location() {
    x = 0;
    y = 0;
  }

  /// Returns the original location of a moved piece.
  Location Move::source(void) {
    return from;
  }

  /// Returns the new location of a moved piece.
  Location Move::dest(void) {
    return to;
  }

  /// Sets the original location of a moved piece.
  void Move::setSource(Location newPos) {
    from = newPos;
  }

  /// Sets the new location of a moved piece.
  void Move::setDest(Location newPos) {
    to = newPos;
  }


  /// The constructor initializes the chess board.
  Board::Board() {
    // Few init values first
    checks = 0; checkmate = EMPTY; curTurn = WHITE; promotions = 0;
    curPos.whitePawns = 0; curPos.whiteRooks = 0; curPos.whiteKnights = 0; curPos.whiteBishops = 0;
    curPos.whiteQueens = 0; curPos.whiteKing = 0; curPos.whitePieces = 0;
    curPos.blackPawns = 0; curPos.blackRooks = 0; curPos.blackKnights = 0; curPos.blackBishops = 0;
    curPos.blackQueens = 0; curPos.blackKing = 0; curPos.blackPieces = 0;

    // Setup the mask and safetyBoard
    BitBoard bit = 1;
    for (unsigned long i = 0; i < 64; i++) {
      mask[i] = bit << i;
      safetyBoard[i] = 0;
    }
    
    // Setup the board
    for (int i = 0; i < 64; i++) {
      curPos.square[i].setColor(initColor[i]);
      curPos.square[i].setPiece(initPiece[i]);
    }
  
    // Initialize bit boards
    for (int i = 0; i < 64; i++) {
      if (curPos.square[i].getColor() == WHITE) {
	curPos.whitePieces |= mask[i];

	switch (curPos.square[i].getPiece()) {
	case PAWN:
	  curPos.whitePawns |= mask[i];
	  break;
	case ROOK:
	  curPos.whiteRooks |= mask[i];
	  break;
	case KNIGHT:
	  curPos.whiteKnights |= mask[i];
	  break;
	case BISHOP:
	  curPos.whiteBishops |= mask[i];
	  break;
	case QUEEN:
	  curPos.whiteQueens |= mask[i];
	  break;
	case KING:
	  curPos.whiteKing |= mask[i];
	  break;
	}
      }
      else if (curPos.square[i].getColor() == BLACK) {
	curPos.blackPieces |= mask[i];
	
	switch (curPos.square[i].getPiece()) {
	case PAWN:
	  curPos.blackPawns |= mask[i];
	  break;
	case ROOK:
	  curPos.blackRooks |= mask[i];
	  break;
	case KNIGHT:
	  curPos.blackKnights |= mask[i];
	  break;
	case BISHOP:
	  curPos.blackBishops |= mask[i];
	  break;
	case QUEEN:
	  curPos.blackQueens |= mask[i];
	  break;
	case KING:
	  curPos.blackKing |= mask[i];
	  break;
	}
      }
    }
  }


  //! Generate all pseudo-legal positions for the side that is about to make a move.
  /** This method creates all the pseudo-legal moves for the chess board and the side which is about to make a move.
   *  When it has finished move generation the flags for checkmate, number of possible checks and the safety-board will be set.
   *  @see getTurn()
   *  @see isCheckSituation()
   *  @see getChecks()
   *  @see getSafetyBoard()
   */
  void Board::genMoves(void) {
    vector<BitBoardMove> moveSet;
    allMoves.clear();                    // Clear the list of possible moves available for this particular board
    promotions = 0;                      // Clear the number of possible pawn promotions
    checks = 0;                          // Clear the number of possible check situations for this particular board
    checkmate = EMPTY;                   // Clear the checkmate-flag for this particular board

    // Safety board clear
    for (int i = 0; i < 64; i++)
      safetyBoard[i] = 0;

    // Generate move-bit-boards for each piece
    for (int i = 0; i < 64; i++) {
      if (curPos.square[i].getColor() == curTurn) {
	switch (curPos.square[i].getPiece()) {
	case PAWN:
	  moveSet = genPawnMoves(i);
	  allMoves.insert(allMoves.end(), moveSet.begin(), moveSet.end());
	  moveSet = genPawnCaptures(i);
	  allMoves.insert(allMoves.end(), moveSet.begin(), moveSet.end());
	  break;
	case ROOK:
	  moveSet = genRookMoves(i);
	  allMoves.insert(allMoves.end(), moveSet.begin(), moveSet.end());
	  moveSet = genRookCaptures(i);
	  allMoves.insert(allMoves.end(), moveSet.begin(), moveSet.end());
	  break;
	case KNIGHT:
	  moveSet = genKnightMoves(i);
	  allMoves.insert(allMoves.end(), moveSet.begin(), moveSet.end());
	  moveSet = genKnightCaptures(i);
	  allMoves.insert(allMoves.end(), moveSet.begin(), moveSet.end());
	  break;
	case BISHOP:
	  moveSet = genBishopMoves(i);
	  allMoves.insert(allMoves.end(), moveSet.begin(), moveSet.end());
	  moveSet = genBishopCaptures(i);
	  allMoves.insert(allMoves.end(), moveSet.begin(), moveSet.end());
	  break;
	case QUEEN:
	  moveSet = genQueenMoves(i);
	  allMoves.insert(allMoves.end(), moveSet.begin(), moveSet.end());
	  moveSet = genQueenCaptures(i);
	  allMoves.insert(allMoves.end(), moveSet.begin(), moveSet.end());
	  break;
	case KING:
	  moveSet = genKingMoves(i);
	  allMoves.insert(allMoves.end(), moveSet.begin(), moveSet.end());
	  moveSet = genKingCaptures(i);
	  allMoves.insert(allMoves.end(), moveSet.begin(), moveSet.end());
	  break;
	}
      }
    }
  }
  
  
  /** This method generates a list of possible moves for a pawn on location pawnLocation.
   *  It automatically determines the pawn's colour and takes care that the pawns move in the right
   *  direction only.
   *  @param pawnLocation an integer that represents the current pawn location on a bit board
   *  @return a vector containing the list of possible pawn moves
   */
  vector<BitBoardMove> Board::genPawnMoves(int pawnLocation) {
    vector<BitBoardMove> moves;
    int offset = 0, doubleOffset = 0;
    BitBoardMove myMove;
    myMove.source = mask[pawnLocation];
    
    if (curPos.square[pawnLocation].getColor() == WHITE) {
      offset = -8;
      doubleOffset = -16;
    }
    else {
      doubleOffset = 16;
      offset = 8;
    }

    // Check if the pawn on pawnLocation can move one row forward
    if ( possiblePawnMove(pawnLocation, offset) ) {
      myMove.dest = mask[pawnLocation + offset];
      moves.push_back(myMove);
    }
    
    // Check if the pawn on pawnLocation can move two rows forward
    if ( possiblePawnMove(pawnLocation, doubleOffset) ) {
      myMove.dest = mask[pawnLocation + doubleOffset];
      moves.push_back(myMove);
    }
    
    // Check promotions
    if ( (possiblePawnMove(pawnLocation, offset)) &&
	 ( (curPos.square[pawnLocation].getColor() == WHITE && ROW(pawnLocation) == 1) ||
	   (curPos.square[pawnLocation].getColor() == BLACK && ROW(pawnLocation) == 6) ) ) {
      promotions++;
    }

    return moves;
  }


  /** This method generates a list of possible captures for a pawn on location pawnLocation.
   * It automatically determines the pawn's colour and takes care that the pawns move in the right
   * direction only.
   * It also sets the flags in a safety board to determine the other own pieces that are protected by the pawn.
   * If the piece can do one or many moves that are threatening the opponent's king then an internal counter will be increased for each of them.
   * @see getSafetyBoard()
   * @see getChecks()
   * @param pawnLocation an integer that represents the current pawn location on a bit board
   * @return a vector containing the list of possible pawn captures
   */
  vector<BitBoardMove> Board::genPawnCaptures(int pawnLocation) {
    int capLeft = 0, capRight = 0;
    vector<BitBoardMove> captures;
    BitBoardMove myMove;
    myMove.source = mask[pawnLocation];

    if (curPos.square[pawnLocation].getColor() == WHITE) {
      capLeft = -9;
      capRight = -7;
    }
    else {
      capLeft = 7;
      capRight = 9;
    }
      
    if (possiblePawnCapture(pawnLocation, capLeft)) {
      myMove.dest = mask[pawnLocation+capLeft];
      if (curPos.square[pawnLocation+capLeft].getPiece() == KING) checks++;
      captures.push_back(myMove);
    }
    if (possiblePawnCapture(pawnLocation, capRight)) {
      myMove.dest = mask[pawnLocation+capRight];
      if (curPos.square[pawnLocation+capRight].getPiece() == KING) checks++;
      captures.push_back(myMove);
    }

    // Check promotions
    if ( ( (possiblePawnCapture(pawnLocation, capLeft)) && (possiblePawnCapture(pawnLocation, capRight)) ) &&
	 ( (curPos.square[pawnLocation].getColor() == WHITE && ROW(pawnLocation) == 1) ||
	   (curPos.square[pawnLocation].getColor() == BLACK && ROW(pawnLocation) == 6) ) ) {
      promotions++;
    }

    // Piece safety
    if ( COL(pawnLocation) > 0 && (pawnLocation+capLeft >= 0) && (mask[pawnLocation+capLeft] & curPos.whitePieces) )
      safetyBoard[pawnLocation+capLeft] += 100;
    if ( COL(pawnLocation) < 7 && (pawnLocation+capRight >= 0) && (mask[pawnLocation+capRight] & curPos.whitePieces) )
      safetyBoard[pawnLocation+capRight] += 100;

    return captures;
  }


  /** This method generates a list of possible captures for a rook on location rookLocation.
   *  It also sets the flags in a safety board to determine the other own pieces that are protected by the rook.
   *  If the piece can do one or many moves that are threatening the opponent's king then an internal counter will be increased for each of them.
   *  @see getSafetyBoard()
   *  @see getChecks()
   *  @param rookLocation an integer that represents the current rook location on a bit board
   *  @return a vector containing the list of possible rook captures
   */
  vector<BitBoardMove> Board::genRookCaptures(int rookLocation) {
    vector<BitBoardMove> captures;
    BitBoard *myPieces, oponents = 0;
    BitBoardMove myMove;
    myMove.source = mask[rookLocation];
    
    if (curTurn == WHITE) {
      myPieces = &curPos.whitePieces;
      oponents = curPos.blackPieces;
    }
    else {
      myPieces = &curPos.blackPieces;
      oponents = curPos.whitePieces;
    }

    for (int u = 0; u < 7; u++) {
      if ( !outOfBoundary(rookLocation, rookPosLeft[u]) ) {
	if ( mask[rookLocation+rookPosLeft[u]] & (mask[rookLocation+rookPosLeft[u]] & *myPieces) ) {
	  safetyBoard[rookLocation+rookPosLeft[u]] += 1;
	  break;
	}
	else if ( mask[rookLocation+rookPosLeft[u]] & (mask[rookLocation+rookPosLeft[u]] & oponents) ) {
	  myMove.dest = mask[rookLocation+rookPosLeft[u]];

	  if (curPos.square[rookLocation+rookPosLeft[u]].getPiece() == KING)
	    checks++;

	  captures.push_back(myMove);
	  break;
	}
      }
    }
    for (int u = 0; u < 7; u++) {
      if ( !outOfBoundary(rookLocation, rookPosRight[u]) ) {
	if ( mask[rookLocation+rookPosRight[u]] & (mask[rookLocation+rookPosRight[u]] & *myPieces) ) {
	  safetyBoard[rookLocation+rookPosRight[u]] += 1;
	  break;
	}
	else if ( mask[rookLocation+rookPosRight[u]] & (mask[rookLocation+rookPosRight[u]] & oponents) ) {
	  myMove.dest = mask[rookLocation+rookPosRight[u]];

	  if (curPos.square[rookLocation+rookPosRight[u]].getPiece() == KING)
	    checks++;

	  captures.push_back(myMove);
	  break;
	}
      }
    }
    for (int u = 0; u < 7; u++) {
      if ( rookLocation + rookPosUp[u] < 64 && rookLocation + rookPosUp[u] >= 0 ) {
	if ( mask[rookLocation+rookPosUp[u]] & (mask[rookLocation+rookPosUp[u]] & *myPieces) ) {
	  safetyBoard[rookLocation+rookPosUp[u]] += 1;
	  break;
	}
	else if ( mask[rookLocation+rookPosUp[u]] & (mask[rookLocation+rookPosUp[u]] & oponents) ) {
	  myMove.dest = mask[rookLocation+rookPosUp[u]];

	  if (curPos.square[rookLocation+rookPosUp[u]].getPiece() == KING)
	    checks++;

	  captures.push_back(myMove);
	  break;
	}
      }
    }
    for (int u = 0; u < 7; u++) {
      if ( rookLocation + rookPosDown[u] < 64 && rookLocation + rookPosDown[u] >= 0 ) {
	if ( mask[rookLocation+rookPosDown[u]] & (mask[rookLocation+rookPosDown[u]] & *myPieces) ) {
	  safetyBoard[rookLocation+rookPosDown[u]] += 1;
	  break;
	}
	else if ( mask[rookLocation+rookPosDown[u]] & (mask[rookLocation+rookPosDown[u]] & oponents) ) {
	  myMove.dest = mask[rookLocation+rookPosDown[u]];

	  if (curPos.square[rookLocation+rookPosDown[u]].getPiece() == KING)
	    checks++;

	  captures.push_back(myMove);
	  break;
	}
      }
    }
    
    return captures;
  }


  /** This method generates a list of possible moves for a rook on location rookLocation.
   *  @param rookLocation an integer that represents the current rook location on a bit board
   *  @return a vector containing the list of possible rook moves
   */
  vector<BitBoardMove> Board::genRookMoves(int rookLocation) {
    vector<BitBoardMove> moves;
    BitBoardMove myMove;
    myMove.source = mask[rookLocation];

    for (int u = 0; u < 7; u++) {
      if ( !outOfBoundary(rookLocation, rookPosLeft[u]) ) {
	if ( mask[rookLocation+rookPosLeft[u]] & (mask[rookLocation+rookPosLeft[u]] & (curPos.whitePieces|curPos.blackPieces)) )
	  break;
	else {
	  myMove.dest = mask[rookLocation+rookPosLeft[u]] & ~(curPos.whitePieces|curPos.blackPieces);
	  moves.push_back(myMove);
	}
      }
    }
    for (int u = 0; u < 7; u++) {
      if ( !outOfBoundary(rookLocation, rookPosRight[u]) ) {
	if ( mask[rookLocation+rookPosRight[u]] & (mask[rookLocation+rookPosRight[u]] & (curPos.whitePieces|curPos.blackPieces)) )
	  break;
	else {
	  myMove.dest = mask[rookLocation+rookPosRight[u]] & ~(curPos.whitePieces|curPos.blackPieces);
	  moves.push_back(myMove);
	}
      }
    }
    for (int u = 0; u < 7; u++) {
      if ( rookLocation + rookPosUp[u] < 64 && rookLocation + rookPosUp[u] >= 0 ) {
	if ( mask[rookLocation+rookPosUp[u]] & (mask[rookLocation+rookPosUp[u]] & (curPos.whitePieces|curPos.blackPieces)) )
	  break;
	else {
	  myMove.dest = mask[rookLocation+rookPosUp[u]] & ~(curPos.whitePieces|curPos.blackPieces);
	  moves.push_back(myMove);
	}
      }
    }
    for (int u = 0; u < 7; u++) {
      if ( rookLocation + rookPosDown[u] < 64 && rookLocation + rookPosDown[u] >= 0 ) {
	if ( mask[rookLocation+rookPosDown[u]] & (mask[rookLocation+rookPosDown[u]] & (curPos.whitePieces|curPos.blackPieces)) )
	  break;
	else {
	  myMove.dest = mask[rookLocation+rookPosDown[u]] & ~(curPos.whitePieces|curPos.blackPieces);
	  moves.push_back(myMove);
	}
      }
    }
    
    return moves;
  }


  /** This method generates a list of possible moves for a knight on location knightLocation.
   *  @param knightLocation an integer that represents the current knight location on a bit board
   *  @return a vector containing the list of possible knight moves
   */
  vector<BitBoardMove> Board::genKnightMoves(int knightLocation) {
    vector<BitBoardMove> moves;
    BitBoardMove myMove;
    myMove.source = mask[knightLocation];

    for (int u = 0; u < 8; u++) {
      if ( knightLocation + knightPos[u] < 64 && knightLocation + knightPos[u] >= 0 ) {
	if (mask[knightLocation+knightPos[u]] & ~(curPos.whitePieces|curPos.blackPieces)) {
	  if (!( (ROW(knightLocation) < 2 || COL(knightLocation) < 1) && knightPos[u] == -17 ) &&
	      !( (ROW(knightLocation) > 5 || COL(knightLocation) > 6) && knightPos[u] ==  17 ) &&
	      !( (ROW(knightLocation) < 2 || COL(knightLocation) > 6) && knightPos[u] == -15 ) &&
	      !( (ROW(knightLocation) > 5 || COL(knightLocation) < 1) && knightPos[u] ==  15 ) &&
	      !( (ROW(knightLocation) < 1 || COL(knightLocation) > 5) && knightPos[u] ==  -6 ) &&
	      !( (ROW(knightLocation) > 6 || COL(knightLocation) < 2) && knightPos[u] ==   6 ) &&
	      !( (ROW(knightLocation) > 6 || COL(knightLocation) > 5) && knightPos[u] ==  10 ) &&
	      !( (ROW(knightLocation) < 1 || COL(knightLocation) < 2) && knightPos[u] == -10 ) ) {
	    myMove.dest = mask[knightLocation+knightPos[u]];
	    moves.push_back(myMove);
	  }
	}
      }
    }

    return moves;
  }


  /** This method generates a list of possible captures for a knight on location knightLocation.
   *  It also sets the flags in a safety board to determine the other own pieces that are protected by the knight.
   *  If the piece can do one or many moves that are threatening the opponent's king then an internal counter will be increased for each of them.
   *  @see getSafetyBoard()
   *  @see getChecks()
   *  @param knightLocation an integer that represents the current knight location on a bit board
   *  @return a vector containing the list of possible knight captures
   */
  vector<BitBoardMove> Board::genKnightCaptures(int knightLocation) {
    vector<BitBoardMove> captures;
    BitBoardMove myMove;
    BitBoard oponents = 0;
    myMove.source = mask[knightLocation];

    if (curTurn == WHITE)
      oponents = curPos.blackPieces;
    else
      oponents = curPos.whitePieces;

    for (int u = 0; u < 8; u++) {
      if ( knightLocation + knightPos[u] < 64 && knightLocation + knightPos[u] >= 0 ) {
	if (mask[knightLocation+knightPos[u]] & oponents) {
	  if (!( (ROW(knightLocation) < 2 || COL(knightLocation) < 1) && knightPos[u] == -17 ) &&
	      !( (ROW(knightLocation) > 5 || COL(knightLocation) > 6) && knightPos[u] ==  17 ) &&
	      !( (ROW(knightLocation) < 2 || COL(knightLocation) > 6) && knightPos[u] == -15 ) &&
	      !( (ROW(knightLocation) > 5 || COL(knightLocation) < 1) && knightPos[u] ==  15 ) &&
	      !( (ROW(knightLocation) < 1 || COL(knightLocation) > 5) && knightPos[u] ==  -6 ) &&
	      !( (ROW(knightLocation) > 6 || COL(knightLocation) < 2) && knightPos[u] ==   6 ) &&
	      !( (ROW(knightLocation) > 6 || COL(knightLocation) > 5) && knightPos[u] ==  10 ) &&
	      !( (ROW(knightLocation) < 1 || COL(knightLocation) < 2) && knightPos[u] == -10 ) ) {
	    myMove.dest = mask[knightLocation+knightPos[u]];

	    if (curPos.square[knightLocation+knightPos[u]].getPiece() == KING)
	      checks++;

	    captures.push_back(myMove);
	  }
	}
	else if (mask[knightLocation+knightPos[u]] & oponents) {
	  if (!( (ROW(knightLocation) < 2 || COL(knightLocation) < 1) && knightPos[u] == -17 ) &&
	      !( (ROW(knightLocation) > 5 || COL(knightLocation) > 6) && knightPos[u] ==  17 ) &&
	      !( (ROW(knightLocation) < 2 || COL(knightLocation) > 6) && knightPos[u] == -15 ) &&
	      !( (ROW(knightLocation) > 5 || COL(knightLocation) < 1) && knightPos[u] ==  15 ) &&
	      !( (ROW(knightLocation) < 1 || COL(knightLocation) > 5) && knightPos[u] ==  -6 ) &&
	      !( (ROW(knightLocation) > 6 || COL(knightLocation) < 2) && knightPos[u] ==   6 ) &&
	      !( (ROW(knightLocation) > 6 || COL(knightLocation) > 5) && knightPos[u] ==  10 ) &&
	      !( (ROW(knightLocation) < 1 || COL(knightLocation) < 2) && knightPos[u] == -10 ) )
	    safetyBoard[knightLocation+knightPos[u]] += 1;
	}
      }
    }

    return captures;
  }

  
  /** This method generates a list of possible moves for a bishop on location bishopLocation.
   *  @param bishopLocation an integer that represents the current bishop location on a bit board
   *  @return a vector containing the list of possible bishop moves
   */
  vector<BitBoardMove> Board::genBishopMoves(int bishopLocation) {
    vector<BitBoardMove> moves;
    BitBoardMove myMove;
    myMove.source = mask[bishopLocation];

    for (int u = 0; u < COL(bishopLocation); u++) {
      if ( bishopLocation + bishPosLUp[u] < 64 && bishopLocation + bishPosLUp[u] >= 0 ) {
	if ( mask[bishopLocation+bishPosLUp[u]] & (mask[bishopLocation+bishPosLUp[u]] & (curPos.whitePieces|curPos.blackPieces)) )
	  break;
	else {
	  myMove.dest = mask[bishopLocation+bishPosLUp[u]] & ~(curPos.whitePieces|curPos.blackPieces);
	  moves.push_back(myMove);
	}
      }
    }
    for (int u = 0; u < COL(bishopLocation); u++) {
      if ( bishopLocation + bishPosLDown[u] < 64 && bishopLocation + bishPosLDown[u] >= 0 && (COL(bishopLocation) != 0) ) {
	if ( mask[bishopLocation+bishPosLDown[u]] & (mask[bishopLocation+bishPosLDown[u]] & (curPos.whitePieces|curPos.blackPieces)) )
	  break;
	else {
	  myMove.dest = mask[bishopLocation+bishPosLDown[u]] & ~(curPos.whitePieces|curPos.blackPieces);
	  moves.push_back(myMove);
	}
      }
    }
    for (int u = 0; u < 7 - COL(bishopLocation); u++) {
      if ( bishopLocation + bishPosRUp[u] < 64 && bishopLocation + bishPosRUp[u] >= 0 ) {
	if ( mask[bishopLocation+bishPosRUp[u]] & (mask[bishopLocation+bishPosRUp[u]] & (curPos.whitePieces|curPos.blackPieces)) )
	  break;
	else {
	  myMove.dest = mask[bishopLocation+bishPosRUp[u]] & ~(curPos.whitePieces|curPos.blackPieces);
	  moves.push_back(myMove);
	}
      }
    }
    for (int u = 0; u < 7 - COL(bishopLocation); u++) {
      if ( bishopLocation + bishPosRDown[u] < 64 && bishopLocation + bishPosRDown[u] >= 0 ) {
	if ( mask[bishopLocation+bishPosRDown[u]] & (mask[bishopLocation+bishPosRDown[u]] & (curPos.whitePieces|curPos.blackPieces)) )
	  break;
	else {
	  myMove.dest = mask[bishopLocation+bishPosRDown[u]] & ~(curPos.whitePieces|curPos.blackPieces);
	  moves.push_back(myMove);
	}
      }
    }

    return moves;
  }

  
   /** This method generates a list of possible captures for a bishop on location bishopLocation.
    *  It also sets the flags in a safety board to determine the other own pieces that are protected by the bishop.
    *  If the piece can do one or many moves that are threatening the opponent's king then an internal counter will be increased for each of them.
    *  @see getSafetyBoard()
    *  @see getChecks()
    *  @param bishopLocation an integer that represents the current bishop location on a bit board
    *  @return a vector containing the list of possible bishop captures
    */
  vector<BitBoardMove> Board::genBishopCaptures(int bishopLocation) {
    vector<BitBoardMove> captures;
    BitBoardMove myMove;
    BitBoard *myPieces, oponents = 0;
    myMove.source = mask[bishopLocation];

    if (curTurn == WHITE) {
      myPieces = &curPos.whitePieces;
      oponents = curPos.blackPieces;
    }
    else {
      myPieces = &curPos.blackPieces;
      oponents = curPos.whitePieces;
    }

    for (int u = 0; u < COL(bishopLocation); u++) {
      if ( bishopLocation + bishPosLUp[u] < 64 && bishopLocation + bishPosLUp[u] >= 0 ) {
	if ( mask[bishopLocation+bishPosLUp[u]] & (mask[bishopLocation+bishPosLUp[u]] & *myPieces) ) {
	  safetyBoard[bishopLocation+bishPosLUp[u]] += 1;
	  break;
	}
	else if ( mask[bishopLocation+bishPosLUp[u]] & (mask[bishopLocation+bishPosLUp[u]] & oponents) ) {
	  myMove.dest = mask[bishopLocation+bishPosLUp[u]];

	  if (curPos.square[bishopLocation+bishPosLUp[u]].getPiece() == KING)
	    checks++;

	  captures.push_back(myMove);
	  break;
	}
      }
    }
    for (int u = 0; u < COL(bishopLocation); u++) {
      if ( (bishopLocation + bishPosLDown[u]) < 64 && (bishopLocation + bishPosLDown[u]) >= 0 ) {
	if ( mask[bishopLocation+bishPosLDown[u]] & (mask[bishopLocation+bishPosLDown[u]] & *myPieces) ) {
	  safetyBoard[bishopLocation+bishPosLDown[u]] += 1;
	  break;
	}
	else if ( mask[bishopLocation+bishPosLDown[u]] & (mask[bishopLocation+bishPosLDown[u]] & oponents) ) {
	  myMove.dest = mask[bishopLocation+bishPosLDown[u]];

	  if (curPos.square[bishopLocation+bishPosLDown[u]].getPiece() == KING)
	    checks++;

	  captures.push_back(myMove);
	  break;
	}
      }
    }
    for (int u = 0; u < 7 - COL(bishopLocation); u++) {
      if ( bishopLocation + bishPosRUp[u] < 64 && bishopLocation + bishPosRUp[u] >= 0 ) {
	if ( mask[bishopLocation+bishPosRUp[u]] & (mask[bishopLocation+bishPosRUp[u]] & *myPieces) ) {
	  safetyBoard[bishopLocation+bishPosRUp[u]] += 1;
	  break;
	}
	else if ( mask[bishopLocation+bishPosRUp[u]] & oponents ) {
	  myMove.dest = mask[bishopLocation+bishPosRUp[u]];

	  if (curPos.square[bishopLocation+bishPosRUp[u]].getPiece() == KING)
	    checks++;

	  captures.push_back(myMove);
	  break;
	}
      }
    }
    for (int u = 0; u < 7 - COL(bishopLocation); u++) {
      if ( bishopLocation + bishPosRDown[u] < 64 && bishopLocation + bishPosRDown[u] >= 0 ) {
	if ( mask[bishopLocation+bishPosRDown[u]] & (mask[bishopLocation+bishPosRDown[u]] & *myPieces) ) {
	  safetyBoard[bishopLocation+bishPosRDown[u]] += 1;
	  break;
	}
	else if ( mask[bishopLocation+bishPosRDown[u]] & (mask[bishopLocation+bishPosRDown[u]] & oponents) ) {
	  myMove.dest = mask[bishopLocation+bishPosRDown[u]];

	  if (curPos.square[bishopLocation+bishPosRDown[u]].getPiece() == KING)
	    checks++;

	  captures.push_back(myMove);
	  break;
	}
      }
    }

    return captures;
  }


  /** This method generates a list of possible moves for a queen on location queenLocation.
   *  @param queenLocation an integer that represents the current queen location on a bit board
   *  @return a vector containing the list of possible queen moves
   */
  vector<BitBoardMove> Board::genQueenMoves(int queenLocation) {
    vector<BitBoardMove> diagonal;
    vector<BitBoardMove> straight;
    vector<BitBoardMove> allQMoves;
    diagonal = genBishopMoves(queenLocation);
    straight = genRookMoves(queenLocation);
    allQMoves = diagonal;
    allQMoves.insert(allQMoves.end(), straight.begin(), straight.end());
    return allQMoves;
  }


  /** This method generates a list of possible captures for a queen on location queenLocation.
   *  It also sets the flags in a safety board to determine the other own pieces that are protected by the queen.
   *  If the piece can do one or many moves that are threatening the opponent's king then an internal counter will be increased for each of them.
   *  @see getSafetyBoard()
   *  @see getChecks()
   *  @param queenLocation an integer that represents the current rook location on a bit board
   *  @return a vector containing the list of possible queen captures
   */
  vector<BitBoardMove> Board::genQueenCaptures(int queenLocation) {
    vector<BitBoardMove> diagonal;
    vector<BitBoardMove> straight;
    vector<BitBoardMove> allQCaps;
    diagonal = genBishopCaptures(queenLocation);
    straight = genRookCaptures(queenLocation);
    allQCaps = diagonal;
    allQCaps.insert(allQCaps.end(), straight.begin(), straight.end());
    return allQCaps;
  }


  /** This method generates a list of possible moves for a king on location kingLocation.
   *  @param bishopLocation an integer that represents the current king location on a bit board
   *  @return a vector containing the list of possible king moves
   */
  vector<BitBoardMove> Board::genKingMoves(int kingLocation) {
    vector<BitBoardMove> moves;
    BitBoardMove myMove;
    myMove.source = mask[kingLocation];
    
    for (int u = 0; u < 8; u++) {
      if ( kingLocation + kingPos[u] < 64 && kingLocation + kingPos[u] >= 0 ) {
	if  ( (abs(kingPos[u]) == 1 && !outOfBoundary(kingLocation, kingPos[u])) ||
	      (COL(kingLocation) < 7 && (kingPos[u] == -7 || kingPos[u] == 9)) ||
	      (COL(kingLocation) > 0 && (kingPos[u] == -9 || kingPos[u] == 7)) ||
	      (abs(kingPos[u]) == 8) ) {
	  if (mask[kingLocation+kingPos[u]] & ~(curPos.whitePieces|curPos.blackPieces)) {
	    myMove.dest = mask[kingLocation+kingPos[u]];
	    moves.push_back(myMove);
	  }
	}
      }
    }

    return moves;
  }


  /** This method generates a list of possible captures for a king on location kingLocation.
   *  It also sets the flags in a safety board to determine the other own pieces that are protected by the king.
   *  @see getSafetyBoard()
   *  @param kingLocation an integer that represents the current king location on a bit board
   *  @return a vector containing the list of possible king captures
   */
  vector<BitBoardMove> Board::genKingCaptures(int kingLocation) {
    vector<BitBoardMove> captures;
    BitBoardMove myMove;
    BitBoard oponents = 0, *myPieces;
    myMove.source = mask[kingLocation];

    if (curTurn == WHITE) {
      myPieces = &curPos.whitePieces;
      oponents = curPos.blackPieces;
    }
    else {
      oponents = curPos.whitePieces;
      myPieces = &curPos.blackPieces;
    }

    for (int u = 0; u < 8; u++) {
      if ( kingLocation + kingPos[u] < 64 && kingLocation + kingPos[u] >= 0 ) {
	if  ( (abs(kingPos[u]) == 1 && !outOfBoundary(kingLocation, kingPos[u])) ||
	      (COL(kingLocation) < 7 && (kingPos[u] == -7 || kingPos[u] == 9)) ||
	      (COL(kingLocation) > 0 && (kingPos[u] == -9 || kingPos[u] == 7)) ||
	      (abs(kingPos[u]) == 8) ) {
	  if (mask[kingLocation+kingPos[u]] & oponents) {
	    myMove.dest = mask[kingLocation+kingPos[u]];
	    captures.push_back(myMove);
	  }
	  else if (mask[kingLocation+kingPos[u]] & *myPieces)
	    safetyBoard[kingLocation+kingPos[u]] += 1;
	}
      }
    }
    
    return captures;
  }


  //! Check whether the chess piece on a certain location can move by offset bits.
  /** This method checks whether it is possible for the chess piece on a given location, to move
   *  over the current bit board by offset bits.
   *  @param pawnLocation is the current location of the chess piece
   *  @param offset is the number of bits the piece wants to move over the bit board
   *  @return true if the move is legal, false otherwise
   */
  bool Board::possiblePawnMove(int pawnLocation, int offset) {
    // White pawns can either move -8 or -16, black pawns can do 8 or 16 bits on the board
    // Everything else is not legal (unless it's a capture or promotion which is handled by other functions)
    if ( (getTurn() == WHITE) && (offset == -8) ) {
      if ( (ROW(pawnLocation) > 0) && (mask[pawnLocation-8] & ~(curPos.whitePieces|curPos.blackPieces)) )
	return true;
    }
    else if ( (getTurn() == WHITE) && (offset == -16) ) {
      if ( (ROW(pawnLocation) == 6) && (mask[pawnLocation-8] & ~(curPos.whitePieces|curPos.blackPieces)) && 
	   (mask[pawnLocation-16] & ~(curPos.whitePieces|curPos.blackPieces)) )
	return true;
    }
    else if ( (getTurn() == BLACK) && (offset == 8) ) {
      if ( (ROW(pawnLocation) < 7) && (mask[pawnLocation+8] & ~(curPos.whitePieces|curPos.blackPieces)) )
	return true;
    }
    else if ( (getTurn() == BLACK) && (offset == 16) ) {
      if ( (ROW(pawnLocation) == 1) && (mask[pawnLocation+8] & ~(curPos.whitePieces|curPos.blackPieces)) &&
	   (mask[pawnLocation+16] & ~(curPos.whitePieces|curPos.blackPieces)) )
	return true;
    }
    
    return false;
  }


  //! Check whether the chess piece on a certain location is able to capture the opponent's pieces, located offset bits away from the attacker.
  /** This method checks whether it is possible for the chess piece on a given location, to capture the opponent, which is located
   *  offsets bit away from the attacker.
   *  @param pawnLocation is the current location of the chess piece
   *  @param offset is the number of bits the piece wants to move over the bit board
   *  @return true if the capture is possible and legal, false otherwise
   */
  bool Board::possiblePawnCapture(int pawnLocation, int offset) {
    if ( (getTurn() == WHITE) && (offset == -7) ) {
      if ( COL(pawnLocation) < 7 && (pawnLocation-7 >= 0) && (mask[pawnLocation-7] & curPos.blackPieces) )
	return true;
    }
    else if ( (getTurn() == WHITE) && (offset == -9) ) {
      if ( COL(pawnLocation) > 0 && (pawnLocation - 9 >= 0) && (mask[pawnLocation-9] & curPos.blackPieces) )
	return true;
    }
    else if ( (getTurn() == BLACK) && (offset == 7) ) {
      if ( COL(pawnLocation) < 7 && (pawnLocation + 9 < 64) && (mask[pawnLocation+9] & curPos.whitePieces) )
	return true;
    }
    else if ( (getTurn() == BLACK) && (offset == 9) ) {
      if ( COL(pawnLocation) < 7 && (pawnLocation + 9 < 64) && (mask[pawnLocation+9] & curPos.whitePieces) )
	return true;
    }
    
    return false;
  }


  //! Take back the last move.
  void Board::undoMove(void) {
    curPos = *(history.end() - 1);
    history.pop_back();
  }


  //! Make a move on the chess board
  /** This method performs an actual move on the chess board and updates all the internal bit boards, too.
   *  @param newMove is the move that has to be made
   *  @see undoMove()
   */
  void Board::makeMove(Move newMove) {
    int row = newMove.source().y * 8;
    int offset = newMove.source().x;
    int source = row + offset, dest = 0;
    row = newMove.dest().y * 8;
    offset = newMove.dest().x;
    dest = row + offset;
    int movedPiece = curPos.square[source].getPiece();
    int movedColor = curPos.square[source].getColor();

    // Store current position in the history
    history.push_back(curPos);

    // Clear destination location
    if (movedColor == WHITE) {
      if (curPos.square[dest].getPiece() != EMPTY) {
	curPos.blackPieces ^= mask[dest];
	curPos.blackPawns ^= mask[dest];
	curPos.blackQueens ^= mask[dest];
	curPos.blackKing ^= mask[dest];
	curPos.blackRooks ^= mask[dest];
	curPos.blackBishops ^= mask[dest];
	curPos.blackKnights ^= mask[dest];
      }
    }
    else {
      if (curPos.square[dest].getPiece() != EMPTY) {
	curPos.whitePieces ^= mask[dest];
	curPos.whitePawns ^= mask[dest];
	curPos.whiteQueens ^= mask[dest];
	curPos.whiteKing ^= mask[dest];
	curPos.whiteRooks ^= mask[dest];
	curPos.whiteBishops ^= mask[dest];
	curPos.whiteKnights ^= mask[dest];
      }
    }

    // Set new position
    if (movedColor == WHITE) {
      switch (movedPiece) {
      case PAWN:
	curPos.whitePieces |= mask[dest];
	if (ROW(dest) == 0)
	  curPos.whiteQueens |= mask[dest];
	else
	  curPos.whitePawns |= mask[dest];
	break;
      case ROOK:
	curPos.whitePieces |= mask[dest];
	curPos.whiteRooks |= mask[dest];
	break;
      case KNIGHT:
	curPos.whitePieces |= mask[dest];
	curPos.whiteKnights |= mask[dest];
	break;
      case BISHOP:
	curPos.whitePieces |= mask[dest];
	curPos.whiteBishops |= mask[dest];
	break;
      case QUEEN:
	curPos.whitePieces |= mask[dest];
	curPos.whiteQueens |= mask[dest];
	break;
      case KING:
	curPos.whitePieces |= mask[dest];
	curPos.whiteKing |= mask[dest];
	break;
      }
    }
    else if (movedColor == BLACK) {
      switch (movedPiece) {
      case PAWN:
	curPos.blackPieces |= mask[dest];
	if (ROW(dest) == 7)
	  curPos.blackQueens |= mask[dest];
	else
	  curPos.blackPawns |= mask[dest];
	break;
      case ROOK:
	curPos.blackPieces |= mask[dest];
	curPos.blackRooks |= mask[dest];
	break;
      case KNIGHT:
	curPos.blackPieces |= mask[dest];
	curPos.blackKnights |= mask[dest];
	break;
      case BISHOP:
	curPos.blackPieces |= mask[dest];
	curPos.blackBishops |= mask[dest];
	break;
      case QUEEN:
	curPos.blackPieces |= mask[dest];
	curPos.blackQueens |= mask[dest];
	break;
      case KING:
	curPos.blackPieces |= mask[dest];
	curPos.blackKing |= mask[dest];
	break;
      }
    }
    curPos.square[dest].setColor(movedColor);
    if ( (curPos.square[source].getPiece() == PAWN) && ((getTurn() == WHITE && ROW(dest) == 0) || (getTurn() == BLACK && ROW(dest) == 7)) )
      curPos.square[dest].setPiece(QUEEN);
    else
      curPos.square[dest].setPiece(movedPiece);
    
    // Clear old position
    if (movedColor == WHITE) {
      switch (movedPiece) {
      case PAWN:
	curPos.whitePieces ^= mask[source];
	curPos.whitePawns ^= mask[source];
	break;
      case ROOK:
	curPos.whitePieces ^= mask[source];
	curPos.whiteRooks ^= mask[source];
	break;
      case KNIGHT:
	curPos.whitePieces ^= mask[source];
	curPos.whiteKnights ^= mask[source];
	break;
      case BISHOP:
	curPos.whitePieces ^= mask[source];
	curPos.whiteBishops ^= mask[source];
	break;
      case QUEEN:
	curPos.whitePieces ^= mask[source];
	curPos.whiteQueens ^= mask[source];
	break;
      case KING:
	curPos.whitePieces ^= mask[source];
	curPos.whiteKing ^= mask[source];
	break;
      }
    }
    else if (movedColor == BLACK) {
      switch (movedPiece) {
      case PAWN:
	curPos.blackPieces ^= mask[source];
	curPos.blackPawns ^= mask[source];
	break;
      case ROOK:
	curPos.blackPieces ^= mask[source];
	curPos.blackRooks ^= mask[source];
	break;
      case KNIGHT:
	curPos.blackPieces ^= mask[source];
	curPos.blackKnights ^= mask[source];
	break;
      case BISHOP:
	curPos.blackPieces ^= mask[source];
	curPos.blackBishops ^= mask[source];
	break;
      case QUEEN:
	curPos.blackPieces ^= mask[source];
	curPos.blackQueens ^= mask[source];
	break;
      case KING:
	curPos.blackPieces ^= mask[source];
	curPos.blackKing ^= mask[source];
	break;
      }
    }
    curPos.square[source].setPiece(EMPTY);
    curPos.square[source].setColor(EMPTY);
  }


  //! Check whether a move is really legal or not
  /** This method checks whether a move can be made or not. It checks the plain chess rules, but also
   *  if the current move would lead into a check mate situation.
   *  @param aMove is the move that has to be checked
   *  @return true if the move is legal and possible, false otherwise
   */
  bool Board::isValidMove(Move aMove) {
    if ((int)curPos.square[aMove.source().y * 8 + aMove.source().x].getColor() != getTurn())
      return false;           // Player tried to move wrong piece, return false

    if (isCheckSituation(aMove))
      return false;           // Return false if the current move would lead into a check mate situation

    switch(curPos.square[aMove.source().y * 8 + aMove.source().x].getPiece()) {
    case PAWN:
    case ROOK:
    case KNIGHT:
    case BISHOP:
    case QUEEN:
    case KING:
      return true;
      break;
    default:
      return false;           // Return false if the player tries to move an empty square
      break;
    }
  }
  

  //! Check if a move would lead the current player into check mate
  /** This method checks whether a new move would lead the current player into a check mate
   *  and therefore if the move is legal or not.
   *  @param aMove is the move which is about to be made and that has to be checked
   *  @return true if the move does not expose the king to the opponent, false otherwise
   */
  bool Board::isCheckSituation(Move aMove) {
    Board vBoard;
    
    vBoard.setBoard(getBoard());
    vBoard.makeMove(aMove);

    if (getTurn() == WHITE)
      vBoard.setTurn(BLACK);
    else
      vBoard.setTurn(WHITE);

    for (int i = 0; i < 64; i++) {
      if ((int)vBoard.getBoard().square[i].getColor() == vBoard.getTurn()) {
	switch (vBoard.getBoard().square[i].getPiece()) {
	case PAWN:
	  vBoard.genPawnCaptures(i);
	  break;
	case ROOK:
	  vBoard.genRookCaptures(i);
	  break;
	case KNIGHT:
	  vBoard.genKnightCaptures(i);
	  break;
	case BISHOP:
	  vBoard.genBishopCaptures(i);
	  break;
	case QUEEN:
	  vBoard.genQueenCaptures(i);
	  break;
	}
      }
      
      if (vBoard.getChecks() > 0)
	return true;
    }

    return false;
  }


  //! Check whether a move leads us right out of the boundaries of the chess board
  /** This method is to check whether a move from position by moveLength bits leads
   *  right out of the borders of the current chess board.
   *  Checking is actually done via the infamous mailbox-Matrix that is also implemented
   *  in T. Kerrigans's simple chess programm.
   *  @param position is the current's piece location on the board
   *  @param moveLength is the number of bits this piece wants to move over the board
   */
  bool Board::outOfBoundary(int position, int moveLength) {
    if (moveLength >= 1) {
      for (int i = 1; i <= moveLength; i++)
	if (mailbox[mailbox64[position]+i] == -1)
	  return true;
    }
    else {
      for (int i = -1; i >= moveLength; i--)
	if (mailbox[mailbox64[position]+i] == -1)
	  return true;
    }
    return false;
  }


  //! Call this after every move a player has made to change turns
  void Board::nextTurn(void) {
    curTurn == WHITE ? curTurn = BLACK : curTurn = WHITE;
  }


  //! Get the color of the current player who is next to move
  /**
   *  @return WHITE if player white is about to move, BLACK otherwise
   */
  int Board::getTurn(void) {
    return curTurn;
  }


  //! Set the current turn to a certain player's color
  /**
   *  @param color is the color of the player/side that should move next (WHITE or BLACK)
   */
  void Board::setTurn(int color) {
    curTurn = color;
  }


  //! An administrative function that should not be deployed in real life games. It visualizes a BitBoard on standard-out.
  void Board::printBitBoard(BitBoard myBoard) {
    BitBoard bit = 1;

    for (int i = 0, row = 0; i < 64; i++, row++) {
      if (row == 8) {
	cout << endl;
	row = 0;
      }
      if ( (bit << i) & myBoard )
	cout << "X ";
      else
	cout << ". ";
    }
    cout << endl << endl;
  }

  
  //! The method genMoves() generates all possible moves for the board and doArrayMove() makes the pos-th move of them.
  /** This method makes a move from the 'global' move list, generated by genMoves(). It makes the pos-th move in this
   *  list by calling makeMove().
   *  @param pos is the number of the move to make
   *  @see genMoves()
   *  @see makeMove()
   */
  void Board::doArrayMove(int pos) {
    Move myMove;
    Location myLocation;
    double bit = 0;
    
    // Check whether pos > allMoves.size() and throw if necessary
    bit = LG(allMoves[pos].source);
    myLocation.x = COL((int)rint(bit));
    myLocation.y = ROW((int)rint(bit));
    myMove.setSource(myLocation);

    bit = LG(allMoves[pos].dest);
    myLocation.x = COL((int)rint(bit));
    myLocation.y = ROW((int)rint(bit));
    myMove.setDest(myLocation);

    makeMove(myMove);
  }

  
  //! Return the pos-th move from a list of possible moves, generated by genMoves()
  /** This method returns a move from the 'global' list of moves, generated by genMoves().
   *  @param pos identifies the number of the move to return
   *  @return The move refered to by pos
   *  @see genMoves()
   */
  Move Board::getArrayMove(int pos) {
    Move myMove;
    Location myLocation;
    double bit = 0;

    bit = LG(allMoves[pos].source);
    myLocation.x = COL((int)rint(bit));
    myLocation.y = ROW((int)rint(bit));
    myMove.setSource(myLocation);

    bit = LG(allMoves[pos].dest);
    myLocation.x = COL((int)rint(bit));
    myLocation.y = ROW((int)rint(bit));    
    myMove.setDest(myLocation);

    return myMove;
  }

  
  //! Return the list of possible moves for the board, generated by genMoves()
  /** This method only returns something sensible if genMoves() was called to generate a valid move list.
   *  @return A vector that contains all possible moves for the current player on the board.
   *  @see genMoves()
   */
  vector<BitBoardMove> Board::getMoves(void) {
    return allMoves;
  }
  

  //! Set the internal variable that stores the best move for the player
  /** This method is used to define the best move for a player. It is used by a computation and search function
   *  to define the best possible move for the computer.
   *  @param bMove represents the best move
   *  @see getBestMove()
   */
  void Board::setBestMove(Move bMove) {
    bestMove = bMove;
  }


  //! Return the best move for the computer player
  /** This method returns the best move (according to an eval, computation and search function) that has been stored
   *  via setBestMove().
   *  @return The best move for the computer player.
   */
  Move Board::getBestMove(void) {
    return bestMove;
  }


  //! Return the current game situation, i.e. the entire board representation
  /** This method returns the entire game situation, represented in the class Position.
   *  Agoris stores the current game in a class called Position.
   *  @return A class that represents the game, incl. all bit boards.
   *  @see setBoard()
   *  @see Position()
   */
   Position Board::getBoard(void) {
    return curPos;
  }


  //! Set the board to a new game situation and update all bit boards and piece constellations
  /** This method can be used to set the current board layout and game situation to an entirely
   *  new situation and status.
   *  Agoris stores the current game in a class called Position.
   *  @param newPos is the new game and board situation
   *  @see getBoard()
   *  @see Position()
   */
  void Board::setBoard(Position newPos) {
    curPos = newPos;
  }

  
  //! Return a pointer that contains information about protected pieces for the current player
  /** This method gives information about the number of own pieces that are protected by other pieces.
   *  The data is stored in an array of integers where 100 or more points mean that a pawn is protecting this piece
   *  while small numbers greater than one mean that this particular piece is protected by non-pawn pieces,
   *  e.g. safetyBoard[5] == 3 means that a piece is protected by three other pieces.
   *  safetyBoard[6] == 101 means this piece is protected by one pawn and one other piece.
   *  @return The safety board as integer pointer.
   */
  int* Board::getSafetyBoard(void) {
    return safetyBoard;
  }


  //! Return the number of checks for the current board
  /** 
   *  @return An integer that contains the number of checks for the current board
   */
  int Board::getChecks(void) {
    return checks;
  }

  
  //! Use this function to define which side is check mate
  /** 
   *  @param aColor is used to set a certain color (WHITE, BLACK or default EMPTY) check mate
   *  @see getCheckMate()
   */
  void Board::setCheckmate(int aColor) {
    checkmate = aColor;
  }


  //! Use this function to find out whether a player is check mate or not
  /**
   *  @return The color of the player who is checkmate (WHITE or BLACK), or EMPTY
   */
  int Board::getCheckmate(void) {
    return checkmate;
  }

  
  //! Return the number of possible pawn promotions for the current player on the board
  /**
   *  @return The number of possible pawn promotions on the board for the current player
   */
  int Board::getPromotions(void) {
    return promotions;
  }

}
