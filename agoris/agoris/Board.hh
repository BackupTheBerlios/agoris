// Board.hh - source file for the Agoris program
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

#ifndef __BOARD_H_
#define __BOARD_H_

#include <vector>
#include <math.h>
#include "Square.hh"

#define COL(x)  (x & 7)
#define ROW(x)  (x >> 3)
#define LG(x)   (log(x)/log(2))

#define WHITE 1
#ifndef EMPTY
#define EMPTY 6
#endif
#define BLACK 0


namespace brd {

  // I am on a 32-bit machine...
  typedef u_int64_t BitBoard;
  
  class BitBoardMove {
  public:
    BitBoard source, dest;
    int score;
    BitBoardMove();
  };
  
  class Location {
  public:
    int x, y;
    Location();
  };

  class Move {
  private:
    Location from;
    Location to;

  public:
    Location source(void);
    Location dest(void);
    void setSource(Location);
    void setDest(Location);
  };
  
  class Position {
  public:
    Square square[64];
    BitBoard blackPieces;
    BitBoard blackPawns;
    BitBoard blackRooks;
    BitBoard blackKnights;
    BitBoard blackBishops;
    BitBoard blackQueens;
    BitBoard blackKing;
    BitBoard whitePieces;
    BitBoard whitePawns;
    BitBoard whiteRooks;
    BitBoard whiteKnights;
    BitBoard whiteBishops;
    BitBoard whiteQueens;
    BitBoard whiteKing;
  };

  const char pieceChar[6] = { 'P', 'N', 'B', 'R', 'Q', 'K' };

  const unsigned int initColor[64] = {
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK, BLACK,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE
  };

  const unsigned int initPiece[64] = {
    3, 1, 2, 4, 5, 2, 1, 3,
    0, 0, 0, 0, 0, 0, 0, 0,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    6, 6, 6, 6, 6, 6, 6, 6,
    0, 0, 0, 0, 0, 0, 0, 0,
    3, 1, 2, 4, 5, 2, 1, 3
  };

  // Move possibilities
  const int rookPosLeft[7] =  { -1, -2, -3, -4, -5, -6, -7 };
  const int rookPosRight[7] = { 1, 2, 3, 4, 5, 6, 7 };
  const int rookPosDown[7] = { 8, 16, 24, 32, 40, 48, 56 };
  const int rookPosUp[7] = { -8, -16, -24, -32, -40, -48, -56 };
  const int knightPos[8] = { -17, -15, -6, -10, 17, 15, 6, 10 };
  const int bishPosLUp[7] = { -9, -18, -27, -36, -45, -54, -63 };
  const int bishPosLDown[7] = { 7, 14, 21, 28, 35, 42, 49 };
  const int bishPosRUp[7] = { -7, -14, -21, -28, -35, -42, -49 };
  const int bishPosRDown[7] = { 9 , 18, 27, 36, 45, 54, 63 };
  const int kingPos[8] = { 1, -1, 8, -8, 9, -9, 7, -7 };
   
  const int mailbox[120] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
    -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
    -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
    -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
    -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
    -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
    -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
    -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
  };
  
  const int mailbox64[64] = {
    21, 22, 23, 24, 25, 26, 27, 28,
    31, 32, 33, 34, 35, 36, 37, 38,
    41, 42, 43, 44, 45, 46, 47, 48,
    51, 52, 53, 54, 55, 56, 57, 58,
    61, 62, 63, 64, 65, 66, 67, 68,
    71, 72, 73, 74, 75, 76, 77, 78,
    81, 82, 83, 84, 85, 86, 87, 88,
    91, 92, 93, 94, 95, 96, 97, 98
  };
  
  class Board {
  private:
    int checkmate;
    unsigned int curTurn;
    BitBoard mask[64];
    Move bestMove;
    vector<BitBoardMove> allMoves;
    Position curPos;
    vector<Position> history;

    // Some sort of an evaluation
    int safetyBoard[64];
    int checks;

  protected:
    bool outOfBoundary(int, int);
    
  public:
    Board();
    void genMoves(void);
    vector<BitBoardMove> genPawnMoves(int);
    vector<BitBoardMove> genPawnCaptures(int);
    vector<BitBoardMove> genRookMoves(int);
    vector<BitBoardMove> genRookCaptures(int);
    vector<BitBoardMove> genKnightMoves(int);
    vector<BitBoardMove> genKnightCaptures(int);
    vector<BitBoardMove> genBishopMoves(int);
    vector<BitBoardMove> genBishopCaptures(int);
    vector<BitBoardMove> genQueenMoves(int);
    vector<BitBoardMove> genQueenCaptures(int);
    vector<BitBoardMove> genKingMoves(int);
    vector<BitBoardMove> genKingCaptures(int);
    int makeMove(Move);
    bool isValidMove(Move);
    bool isCheckSituation(Move);
    void undoMove(void);
    vector<BitBoardMove> getMoves(void);
    void doArrayMove(int);
    Move getArrayMove(int);
    void setBestMove(Move);
    Move getBestMove(void);
    void nextTurn(void);
    int getTurn(void);
    void setTurn(int);
    Position getBoard(void);
    void setBoard(Position);
    int* getSafetyBoard(void);
    int getChecks(void);
    void setCheckmate(int);
    int getCheckmate(void);
    void printBitBoard(BitBoard);
  };
  
}

#endif
