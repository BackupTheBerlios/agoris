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

  BitBoardMove::BitBoardMove() {
    source = 0;
    dest = 0;
    score = 0;
  }

  Location::Location() {
    x = 0;
    y = 0;
  }

  Location Move::source(void) {
    return from;
  }

  Location Move::dest(void) {
    return to;
  }

  void Move::setSource(Location newPos) {
    from = newPos;
  }

  void Move::setDest(Location newPos) {
    to = newPos;
  }


  Board::Board() {
    // Few init values first
    checks = 0;
    checkmate = EMPTY;
    curTurn = WHITE;
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

  
  // Create the pseudo-legal moves
  void Board::genMoves(void) {
    allMoves.clear();
    checks = 0;
    checkmate = EMPTY;

    // Safety board clear
    for (int i = 0; i < 64; i++)
      safetyBoard[i] = 0;

    // Generate move-bit-boards for each piece
    for (int i = 0; i < 64; i++) {
      if (curPos.square[i].getColor() == curTurn) {
	switch (curPos.square[i].getPiece()) {
	case PAWN:
	  genPawnMoves(i);
	  genPawnCaptures(i);
	  break;
	case ROOK:
	  genRookMoves(i);
	  genRookCaptures(i);
	  break;
	case KNIGHT:
	  genKnightMoves(i);
	  genKnightCaptures(i);
	  break;
	case BISHOP:
	  genBishopMoves(i);
	  genBishopCaptures(i);
	  break;
	case QUEEN:
	  genQueenMoves(i);
	  genQueenCaptures(i);
	  break;
	case KING:
	  genKingMoves(i);
	  genKingCaptures(i);
	  break;
	}
      }
    }
  }
  
  
  vector<BitBoardMove> Board::genPawnMoves(int pawnLocation) {
    vector<BitBoardMove> moves;
    BitBoardMove myMove;
    myMove.source = mask[pawnLocation];
    
    if (curTurn == WHITE) {
      if ( (ROW(pawnLocation) > 0) && (mask[pawnLocation-8] & ~(curPos.whitePieces|curPos.blackPieces)) ) {
	myMove.dest = mask[pawnLocation-8];
	allMoves.push_back(myMove);
	moves.push_back(myMove);
      }
      if ( (ROW(pawnLocation) == 6) && (mask[pawnLocation-8] & ~(curPos.whitePieces|curPos.blackPieces)) && 
	   (mask[pawnLocation-16] & ~(curPos.whitePieces|curPos.blackPieces)) ) {
	myMove.dest = mask[pawnLocation-16];
	allMoves.push_back(myMove);
	moves.push_back(myMove);
      }
    }
    else {
      if ( (ROW(pawnLocation) < 7) && (mask[pawnLocation+8] & ~(curPos.whitePieces|curPos.blackPieces)) ) {
	myMove.dest = mask[pawnLocation+8];
	allMoves.push_back(myMove);
	moves.push_back(myMove);
      }
      if ( (ROW(pawnLocation) == 1) && (mask[pawnLocation+8] & ~(curPos.whitePieces|curPos.blackPieces)) &&
	   (mask[pawnLocation+16] & ~(curPos.whitePieces|curPos.blackPieces)) ) {
	myMove.dest = mask[pawnLocation+16];
	allMoves.push_back(myMove);
	moves.push_back(myMove);
      }
    }

    return moves;
  }


  vector<BitBoardMove> Board::genPawnCaptures(int pawnLocation) {
    vector<BitBoardMove> captures;
    BitBoardMove myMove;
    myMove.source = mask[pawnLocation];
    
    if (curTurn == WHITE) {
      if ( COL(pawnLocation) > 0 && pawnLocation - 9 >= 0 && (mask[pawnLocation-9] & curPos.blackPieces) ) {
	myMove.dest = mask[pawnLocation-9];
	
	if (curPos.square[pawnLocation-9].getPiece() == KING)
	  checks++;
	
	captures.push_back(myMove);
	allMoves.push_back(myMove);
      }
      if ( COL(pawnLocation) < 7 && pawnLocation - 7 >= 0 && (mask[pawnLocation-7] & curPos.blackPieces) ) {
	myMove.dest = mask[pawnLocation-7];

	if (curPos.square[pawnLocation-7].getPiece() == KING)
	  checks++;

	captures.push_back(myMove);
	allMoves.push_back(myMove);
      }

      // Piece safety
      if ( COL(pawnLocation) > 0 && pawnLocation - 9 >= 0 && (mask[pawnLocation-9] & curPos.whitePieces) )
	safetyBoard[pawnLocation-9] += 100;
      if ( COL(pawnLocation) < 7 && pawnLocation - 7 >= 0 && (mask[pawnLocation-7] & curPos.whitePieces) )
	safetyBoard[pawnLocation-7] += 100;
    }
    else {
      if ( COL(pawnLocation) < 7 && pawnLocation + 9 < 64 && (mask[pawnLocation+9] & curPos.whitePieces) ) {
	myMove.dest = mask[pawnLocation+9];

	if (curPos.square[pawnLocation+9].getPiece() == KING)
	  checks++;

	captures.push_back(myMove);
	allMoves.push_back(myMove);
      }
      if ( COL(pawnLocation) > 0 && pawnLocation + 7 < 64 && (mask[pawnLocation+7] & curPos.whitePieces) ) {
	myMove.dest = mask[pawnLocation+7];

	if (curPos.square[pawnLocation+7].getPiece() == KING)
	  checks++;

	captures.push_back(myMove);
	allMoves.push_back(myMove);
      }
      
      // Piece safety
      if ( COL(pawnLocation) < 7 && pawnLocation + 9 < 64 && (mask[pawnLocation+9] & curPos.blackPieces) )
	safetyBoard[pawnLocation+9] += 100;
      if ( COL(pawnLocation) > 0 && pawnLocation + 7 < 64 && (mask[pawnLocation+7] & curPos.blackPieces) )
	safetyBoard[pawnLocation+7] += 100;
    }

    return captures;
  }


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
	  allMoves.push_back(myMove);
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
	  allMoves.push_back(myMove);
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
	  allMoves.push_back(myMove);
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
	  allMoves.push_back(myMove);
	  break;
	}
      }
    }
    
    return captures;
  }


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
	  allMoves.push_back(myMove);
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
	  allMoves.push_back(myMove);
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
	  allMoves.push_back(myMove);
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
	  allMoves.push_back(myMove);
	}
      }
    }
    
    return moves;
  }


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
	    allMoves.push_back(myMove);
	  }
	}
      }
    }

    return moves;
  }


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
	    allMoves.push_back(myMove);
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
	  allMoves.push_back(myMove);
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
	  allMoves.push_back(myMove);
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
	  allMoves.push_back(myMove);
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
	  allMoves.push_back(myMove);
	}
      }
    }

    return moves;
  }

  
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
	  allMoves.push_back(myMove);
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
	  allMoves.push_back(myMove);
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
	  allMoves.push_back(myMove);
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
	  allMoves.push_back(myMove);
	  break;
	}
      }
    }

    return captures;
  }


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
	    allMoves.push_back(myMove);
	    moves.push_back(myMove);
	  }
	}
      }
    }

    return moves;
  }


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
	    allMoves.push_back(myMove);
	  }
	  else if (mask[kingLocation+kingPos[u]] & *myPieces)
	    safetyBoard[kingLocation+kingPos[u]] += 1;
	}
      }
    }
    
    return captures;
  }


  void Board::undoMove(void) {
    curPos = *(history.end() - 1);
    history.pop_back();
  }


  // Do the actual move and update BitBoards, and the Chess board itself
  int Board::makeMove(Move newMove) {
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

    // Set new position
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

      switch (movedPiece) {
      case PAWN:
	curPos.whitePieces |= mask[dest];
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
      if (curPos.square[dest].getPiece() != EMPTY) {
	curPos.whitePieces ^= mask[dest];
	curPos.whitePawns ^= mask[dest];
	curPos.whiteQueens ^= mask[dest];
	curPos.whiteKing ^= mask[dest];
	curPos.whiteRooks ^= mask[dest];
	curPos.whiteBishops ^= mask[dest];
	curPos.whiteKnights ^= mask[dest];
      }

      switch (movedPiece) {
      case PAWN:
	curPos.blackPieces |= mask[dest];
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

    return 0;
  }


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
  

  // Returns true if aMove leads to a check situation for the opposite king
  // (This function acts more as a potential check mate control mechanism.)
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


  // Returns true if a move from 'position' by 'moveLength'-squares
  // would lead out of the Chess board, horizontally. Otherwise returns false.
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


  void Board::nextTurn(void) {
    curTurn == WHITE ? curTurn = BLACK : curTurn = WHITE;
  }


  int Board::getTurn(void) {
    return curTurn;
  }


  void Board::setTurn(int color) {
    curTurn = color;
  }


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

  
  vector<BitBoardMove> Board::getMoves(void) {
    return allMoves;
  }
  

  void Board::setBestMove(Move bMove) {
    bestMove = bMove;
  }


  Move Board::getBestMove(void) {
    return bestMove;
  }


  Position Board::getBoard(void) {
    return curPos;
  }


  void Board::setBoard(Position newPos) {
    curPos = newPos;
  }

  
  int* Board::getSafetyBoard(void) {
    return safetyBoard;
  }


  int Board::getChecks(void) {
    return checks;
  }

  
  void Board::setCheckmate(int aColor) {
    checkmate = aColor;
  }


  int Board::getCheckmate(void) {
    return checkmate;
  }

}
