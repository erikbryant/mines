//
// Copyright Erik Bryant (erikbryantology@gmail.com)
// GPLv2 http://www.gnu.org/licenses/gpl-2.0.html
//

//
// TODO:
// * Make a board iterator that walks all cells
// ** Give it flags to ignore non-cells
// * Allow the boards to have multi-word names (change the read to a getline call)
// * Add a strings file for internationalization
// * If there are too few available spaces to put mines into
//   it will get into an infinite loop. Write a function to count
//   number of available cells.
//

#include <iostream>
#include <fstream>
#include "board.hpp"

//
// Convert these to something more C++ style.
//
#include <string.h>   // strlen
#include <stdlib.h>   // srand, rand

using namespace std;

Board::Board() :
  rows(0),
  cols(0),
  difficulty(LEVEL_EASY),
  initialized(0),
  cells(NULL),
  curR(0),
  curC(0)
{
  srand( time(NULL) );
}

void Board::deinit()
{
  if ( initialized ) {
    for ( int r=0; r<rows; r++ ) {
      delete cells[r];
    }
    delete cells;
    cells = NULL;
    rows = 0;
    cols = 0;
    difficulty = LEVEL_EASY;
    curR = 0;
    curC = 0;
    initialized = 0;
  }
}

Board::~Board()
{
  deinit();
}

void Board::readFile( char *filename )
{
  if ( initialized ) {
    deinit();
  }

  ifstream boardFile;
  char *rowBuff = new char[1024];

  boardFile.open( filename );

  boardFile >> name;
  boardFile >> mines[0];
  boardFile >> mines[1];
  boardFile >> mines[2];

  //
  // Read the board layout.
  // TODO: Replace the hardcoded array
  // with something expandable. Vector?
  //
#ifdef ARRAY
  rows = 0;
  cells = new Cell*[999];
  while ( 1 ) {
    boardFile >> rowBuff;
    if ( boardFile.eof() ) { break; }
    cols = strlen( rowBuff );
    cells[rows] = new Cell[cols];
    for ( int c=0; c<cols; c++ ) {
      if ( rowBuff[c] == '0' ) {
        cells[rows][c].setCell( 0, 0 );
      } else {
        cells[rows][c].setCell( 1, rowBuff[c] - '0' );
      }
    }

    rows++;
  }
#else
  rows = 0;
  cells = new vector< Cell >*[999];
  while ( 1 ) {
    boardFile >> rowBuff;
    if ( boardFile.eof() ) { break; }
    cols = strlen( rowBuff );
    cells[rows] = new vector< Cell >(cols);
    for ( int c=0; c<cols; c++ ) {
      if ( rowBuff[c] == '0' ) {
        cells[rows][c].setCell( 0, 0 );
      } else {
        cells[rows][c].setCell( 1, rowBuff[c] - '0' );
      }
    }

    rows++;
  }
#endif

  boardFile.close();
  delete rowBuff;

  initialized = 1;
}

void Board::startNewGame()
{
  // Put the caps back on, clear old mines
  for ( int r=0; r<rows; r++ ) {
    for ( int c=0; c<cols; c++ ) {
      cells[r][c].clearCell();
    }
  }

  // Place the mines
  int placed = 0;

  while ( placed < mines[difficulty] ) {
    int r = rand() % rows;
    int c = rand() % cols;
    if ( cells[r][c].isCell() && !cells[r][c].hasMine() ) {
      cells[r][c].setHasMine( true );
      placed++;
    }
  }

  // Initialize the neighbor count
  for ( int r=0; r<rows; r++ ) {
    for ( int c=0; c<cols; c++ ) {
      cells[r][c].setNeighbors( countNeighbors( r, c ) );
    }
  }

  stats.reset();
}

unsigned char Board::countNeighbors( int r, int c )
{
  if ( r < 0 || c < 0 || r >= rows || c >= cols ) {
    return -1;
  }

  unsigned char neighbors = 0;

  int minR = r - 1;
  int maxR = r + 1;
  int minC = c - 1;
  int maxC = c + 1;

  //
  // Clamp row range to be between [0..rows-1]
  //
  minR = max( 0, minR );
  maxR = min( maxR, rows-1 );

  //
  // Clamp col range to be between [0..cols-1]
  //
  minC = max( 0, minC );
  maxC = min( maxC, cols-1 );

  for ( int i=minR; i<=maxR; i++ ) {
    for ( int j=minC; j<=maxC; j++ ) {
      if ( cells[i][j].hasMine() ) {
        neighbors++;
      }
    }
  }

  // We included ourselves in that counting. Undo that.
  neighbors -= cells[r][c].hasMine() ? 1 : 0;

  return ( neighbors );
}

int Board::countFlags()
{
  int flags = 0;

  for ( int r=0; r<rows; r++ ) {
    for ( int c=0; c<cols; c++ ) {
      if ( cells[r][c].hasFlag() ) {
        flags++;
      }
    }
  }

  return flags;
}

void Board::print( bool showColorVal )
{
  for ( int r=0; r<rows; r++ ) {
    for ( int c=0; c<cols; c++ ) {
      if ( r == curR && c == curC ) {
        cout << char(27) << "[7m";
      }

      if ( cells[r][c].isCell() ) {
        cout << char(27) << "[" << 40 + cells[r][c].getColor() << "m";
        switch ( cells[r][c].getCapType() ) {
          case CAP_FLAG:
            cout << char(27) << "[1m";
            cout << 'F';
            break;
          case CAP_QUESTION:
            cout << char(27) << "[1m";
            cout << '?';
            break;
          case CAP_BLANK:
            if ( showColorVal ) {
              cout << cells[r][c].getColor();
            } else {
              cout << char(27) << "[1m";
              cout << '#';
            }
            break;
          case CAP_NONE:
            if ( cells[r][c].hasMine() ) {
              cout << char(27) << "[1m";
              cout << '*';
            } else {
              unsigned char neighbors = cells[r][c].getNeighbors();
              if ( neighbors > 0 ) {
                // cout tries to display the character rather than the number
                cout << (int) neighbors;
              } else {
                cout << ' ';
              }
            }
            break;
          default:
            cout << '~';
            break;
        }
        cout << char(27) << "[0m";
      } else {
        cout << ' ';
      }

      if ( r == curR && c == curC ) {
        cout << char(27) << "[0m";
      }
    }
    cout << endl;
  }

  cout << "Marked: " << countFlags() << " of " << mines[difficulty] << endl;
}

#if 0
void Board::print( bool showColorVal )
{
  for ( int r=0; r<rows; r++ ) {
    for ( int c=0; c<cols; c++ ) {
      if ( r == curR && c == curC ) {
        cout << char(27) << "[7m";
      }
      if ( cells[r][c].isCell() ) {
        cout << char(27) << "[" << 40 + cells[r][c].color << "m";
        switch (cells[r][c].cap) {
          case CAP_NONE:
            if ( cells[r][c].hasMine() ) {
              cout << char(27) << "[1m";
              cout << 'X';
            } else {
              int neighbors = countNeighbors( r, c );
              if ( neighbors > 0 ) {
                cout << neighbors;
              } else {
                cout << ' ';
              }
            }
            break;
          case CAP_BLANK:
            if ( showColorVal ) {
              cout << char( cells[r][c].color + '0' );
            } else {
              cout << char(27) << "[1m";
              cout << "#";
            }
            break;
          case CAP_FLAG:
            cout << char(27) << "[1m";
            cout << "X";
            break;
          case CAP_QUESTION:
            cout << char(27) << "[1m";
            cout << "?";
            break;
          default:
            cout << char(27) << "[1m";
            cout << "~";
            break;
        }
        cout << char(27) << "[0m";
      } else {
        cout << " ";
      }
      if ( r == curR && c == curC ) {
        cout << char(27) << "[0m";
      }
    }
    cout << endl;
  }

  cout << "Marked: " << countFlags() << " of " << mines[difficulty] << endl;
}
#endif

//
// Take the cap off and reveal the cell underneath.
// Returns True if there is a mine. False otherwise.
//
bool Board::reveal()
{
  return ( reveal( curR, curC ) );
}

bool Board::reveal( int r, int c )
{
  if ( r < 0 || c < 0 || r >= rows || c >= cols ) {
    return false;
  }

  if ( !cells[r][c].isCell() ) {
    return false;
  }

  if ( cells[r][c].hasFlag() ) {
    return false;
  }

  if ( !cells[r][c].hasCap() ) {
    return false;
  }

  if ( cells[r][c].reveal() ) {
    return true;
  }

  // If this cell has no neighbors, flood reveal.
  if ( countNeighbors( r, c ) == 0 ) {
    reveal( r-1, c-1 );
    reveal( r-1, c   );
    reveal( r-1, c+1 );
    reveal( r,   c-1 );
    reveal( r,   c+1 );
    reveal( r+1, c-1 );
    reveal( r+1, c   );
    reveal( r+1, c+1 );
  }

  return false;
}

void Board::unreveal()
{
  stats.usedUndo();
  cells[curR][curC].unreveal();
}

void Board::revealAll()
{
  for ( int r=0; r<rows; r++ ) {
    for ( int c=0; c<cols; c++ ) {
        cells[r][c].reveal();
    }
  }
}

void Board::toggleFlag()
{
  cells[curR][curC].toggleFlag();
  if ( cells[curR][curC].hasFlag() ) {
    stats.usedFlag();
    if ( !cells[curR][curC].hasMine() ) {
      stats.usedMarkWrong();
    }
  }
}

void Board::toggleQuestion()
{
  cells[curR][curC].toggleQuestion();
  if ( cells[curR][curC].hasFlag() ) {
    stats.usedQuestion();
  }
}

void Board::setDifficulty( unsigned int d )
{
  difficulty = d;
  stats.setLevel( d );
}

bool Board::isSolved()
{
  for ( int r=0; r<rows; r++ ) {
    for ( int c=0; c<cols; c++ ) {
      if ( !cells[r][c].isSolved() ) {
        return ( false );
      }
    }
  }

  return ( true );
}

void Board::move( int r, int c )
{
  curR += r;
  curC += c;

  //
  // Clamp row range to be between [0..rows-1]
  //
  curR = max( 0, curR );
  curR = min( curR, rows-1 );

  //
  // Clamp col range to be between [0..cols-1]
  //
  curC = max( 0, curC );
  curC = min( curC, cols-1 );
}

bool Board::findUnmarkedMine( int *row, int *col )
{
  for ( int r=0; r<rows; r++ ) {
    for ( int c=0; c<cols; c++ ) {
      if ( cells[r][c].isCell() && cells[r][c].hasMine() && !cells[r][c].hasFlag() ) {
        *row = r;
        *col = c;
        return true;
      }
    }
  }

  return false;
}

bool Board::findWronglyFlaggedMine( int *row, int *col )
{
  for ( int r=0; r<rows; r++ ) {
    for ( int c=0; c<cols; c++ ) {
      if ( cells[r][c].isCell() && !cells[r][c].hasMine() && cells[r][c].hasFlag() ) {
        *row = r;
        *col = c;
        return true;
      }
    }
  }

  return false;
}

bool Board::findEmptyCellWithCap( int *row, int *col )
{
  for ( int r=0; r<rows; r++ ) {
    for ( int c=0; c<cols; c++ ) {
      if ( cells[r][c].isCell() && !cells[r][c].hasMine() && cells[r][c].hasCap() && !cells[r][c].hasFlag() ) {
        *row = r;
        *col = c;
        return true;
      }
    }
  }

  return false;
}

void Board::hint()
{
  int mineR = 0;
  int mineC = 0;

  stats.usedHint();

  //
  // Find a cell that is not a mine, but
  // that still has a cap on it.
  //
  if ( findEmptyCellWithCap( &mineR, &mineC ) ) {
    curR = mineR;
    curC = mineC;
    reveal( curR, curC );
    cout << "The cursor has been moved to a cell with no mine and it has been auto-revealed." << endl;
    return;
  }

  //
  // See if there are any un-found mines.
  // If so, move the cursor to one of them.
  //
  if ( findUnmarkedMine( &mineR, &mineC ) ) {
    curR = mineR;
    curC = mineC;
    cells[curR][curC].toggleFlag();
    cout << "The cursor has been moved to a cell that has a mine. It has been auto-marked." << endl;
    return;
  }

  //
  // See if there are any marked mines that
  // are incorrectly marked. If so, move to
  // one of them.
  //
  if ( findWronglyFlaggedMine( &mineR, &mineC ) ) {
    curR = mineR;
    curC = mineC;
    cells[curR][curC].clearFlag();
    reveal( curR, curC );
    cout << "The cursor has been moved to a cell that was incorrectly flagged as a mine and it has been auto-revealed." << endl;
    return;
  }

  //
  // If we got here, this is probably a bug.
  //
  cout << "Could not think of a hint to give you. This is probably a bug." << endl;
  return;
}

//
// -----------------------------------------------------
// Functions used by the editor
// -----------------------------------------------------
//

void Board::setCell( unsigned int color, bool mirror )
{
  setCell( curR, curC, color, mirror );
}

void Board::setCell( int r, int c, unsigned int color, bool mirror )
{
  cells[r][c].setCell( color != 0, color );
  if ( mirror ) {
    int middle = ( cols / 2 ) - 1;
    int delta = middle - c;
    int mirrorCol = middle + delta;
    cells[r][mirrorCol].setCell( color != 0, color );
  }
}

void Board::fillCells( unsigned int color, bool mirror )
{
  fillCellsHelper( curR, curC, color, mirror );
}

void Board::fillCellsHelper( int r, int c, unsigned int color, bool mirror )
{
  if ( r < 0 || r >= rows || c < 0 || c >= cols ) {
    return;
  }

  if ( cells[r][c].getColor() != 0 ) {
    return;
  }

  setCell( r, c, color, mirror );

  fillCellsHelper( r-1, c,   color, mirror );
  fillCellsHelper( r,   c-1, color, mirror );
  fillCellsHelper( r,   c+1, color, mirror );
  fillCellsHelper( r+1, c,   color, mirror );
}

void Board::writeFile( char *filename )
{
  ofstream boardFile;

  boardFile.open( filename );

  boardFile << name << endl;
  boardFile << mines[0] << endl;
  boardFile << mines[1] << endl;
  boardFile << mines[2] << endl;

  for ( int r=0; r<rows; r++ ) {
    for ( int c=0; c<cols; c++ ) {
      boardFile << cells[r][c].getColor();
    }
    boardFile << endl;
  }

  boardFile.close();
}

void Board::deleteRow()
{
  if ( curR < 0 || curR >= rows ) {
    return;
  }

  // Always leave something behind to support a cursor position
  if ( rows <= 1 ) {
    return;
  }

  delete cells[curR];

  // Slide the lower rows up to fill the gap
  for ( int r=curR+1; r<rows; r++ ) {
    cells[r-1] = cells[r];
    cells[r] = NULL;
  }

  rows--;

  if ( curR >= rows ) {
    curR = rows - 1;
  }
}

void Board::deleteCol()
{
  if ( curC < 0 || curC >= cols ) {
    return;
  }

  // Always leave something behind to support a cursor position
  if ( cols <= 1 ) {
    return;
  }

  for ( int r=0; r<rows; r++ ) {
    for ( int c=curC+1; c<cols; c++ ) {
      cells[r][c-1] = cells[r][c];
    }
  }

  cols--;

  if ( curC >= cols ) {
    curC = cols - 1;
  }
}

void Board::insertRowAbove()
{
}

void Board::insertRowBelow()
{
}

void Board::insertColLeft()
{
}

void Board::insertColRight()
{
}

void Board::replaceColor( unsigned int oldColor, unsigned int newColor )
{
  for ( int r=0; r<rows; r++ ) {
    for ( int c=0; c<cols; c++ ) {
      if ( cells[r][c].getColor() == oldColor ) {
        cells[r][c].setColor( newColor );
      }
    }
  }
}

