// textchess.cc - source file for the textchess program
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


// Standard C++ stuff
#include <iostream>
#include <string>

// Agoris stuff
#include <agoris/Board.hh>
#include <agoris/Game.hh>

// Local configuration
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

using namespace std;       // Standard C++ namespace
using namespace brd;       // Agoris namespace, defined in Board.hh

// Prototypes
void print(Position curPos);
void printMove(Move aMove);


int main() {
  int x1 = 0, x2 = 0, y1 = 0, y2 = 0;

  // Agoris objects
  Location from, to;
  Move myMove, compMove;
  Game myChessGame;

  cout << (string)PACKAGE << " " << (string)VERSION << endl;
  cout << "Written by Andreas Bauer, Copyright (c) 2001" << endl;
  cout << "(To end the game enter 100 as X1 coordinate.)" << endl;

  // Set maximum search time for a move in seconds
  myChessGame.setMaxTime(30);

  // Play the game until 666 was entered for the x1 coordinate
  while (x1 != 100) {

    // Print current board.
    // myChessGame.getBoard() returns the actual board layout and the
    // position of all pieces. We use that Position-object in our print(Position)
    // function to display the actual game onto the user's screen.
    print(myChessGame.getBoard());

#ifdef DEBUG
    // Print current board score
    cout << "White score: " << myChessGame.eval() << endl;
    myChessGame.nextTurn();
    cout << "Black score: " << myChessGame.eval() << endl;
    myChessGame.nextTurn();
#endif

    // Get player to enter a move
    cout << "X1: "; cin >> x1; cout << "Y1: "; cin >> y1;
    cout << "X2: "; cin >> x2; cout << "Y2: "; cin >> y2;
    from.x = x1; from.y = y1;
    to.x = x2; to.y = y2;

    // Set the actual move on the board
    myMove.setSource(from);
    myMove.setDest(to);
  
    // Check if move is valid, i.e. if we're in check and don't react to it
    if (myChessGame.isValidMove(myMove)) {
      myChessGame.makeMove(myMove);                   // Make move
      myChessGame.nextTurn();                         // Next turn
    }
    else {
      cout << "Invalid move. Choose again!" << endl;
      continue;
    }

    // Let computer make his move:
    // Use algorithm 1) alpha-beta pruning, or 0) minimax
    // Use search depth of 5 (with a depth of 3 you usually get much faster results, but not as precise!)
    compMove = myChessGame.calculateMove(1, 3);
    
    // Check for check mate
    if (myChessGame.getCheckmate() == BLACK) {
      cout << "Computer is check mate. Congratulation, you won the game." << endl;
      return 0;
    }
    else {
      myChessGame.makeMove(compMove);                 // Make move
      myChessGame.nextTurn();                         // Next turn
      printMove(myChessGame.getBestMove());           // Print the computers move to the screen
    }
  }
  
  return 0;
}


// Print current game position (the board) on the screen
void print(Position curPos) {
  cout << endl;
  
  for (int rowCount = 0, i = 0; i < 64; i++, rowCount++) {
    if (rowCount == 8) {
      cout << " " << ( 7 - ((64 - i) / 8) ) << endl;
      rowCount = 0;
    }
    
    if (curPos.square[i].getColor() == WHITE)
      cout << "W-";
    else if (curPos.square[i].getColor() == BLACK)
      cout << "B-";
    else {
      if (i < 10)
	cout << "[ 0" << i;
      else
	cout << "[ " << i;
    }
    
    switch(curPos.square[i].getPiece()) {
    case PAWN:
      cout << "PAWN";
      break;
    case ROOK:
      cout << "ROOK";
      break;
    case BISHOP:
      cout << "BISH";
      break;
    case KNIGHT:
      cout << "KNIG";
      break;
    case QUEEN:
      cout << "QUEE";
      break;
    case KING:
      cout << "KING";
      break;
    default:
      cout << " ]";
      break;
    }
    cout << " ";
  }
  cout << " 7" << endl;
  cout << "0      1      2      3      4      5      6      7" << endl;
}


// Print a move to the screen
void printMove(Move aMove) {
  cout << aMove.source().x << ":" << aMove.source().y << " - " << aMove.dest().x << ":" << aMove.dest().y << endl;
}
