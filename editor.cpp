//
// Copyright Erik Bryant (erikbryantology@gmail.com)
// GPLv2 http://www.gnu.org/licenses/gpl-2.0.html
//

#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <string.h>

#include "board.hpp"

using namespace std;

//
// Copied from the web.
// Write your own!
//
char readkbd( void ) {
  char ch;
  struct termios oldt;
  struct termios newt;
  tcgetattr( STDIN_FILENO, &oldt );

  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
  
  return ch;
}

int main( int argc, char **argv ) {
  bool done = 0;

  while ( !done ) {
    Board b;
    char boardName[255];
    bool useColor = true;
    bool gameOver = false;

    system( "ls boards" );
    cout << "Board name: ";
    cin >> boardName;
    b.readFile( boardName );

    while ( !gameOver ) {
      char op;

      system( "clear" );
      b.print( useColor );
      cout << "Your move [<arrows> ";
      for ( int i=1; i<=9; i++ ) {
        cout << char(27) << "[" << 40 + i << "m";
        cout << i;
        cout << char(27) << "[0m";
      }
      cout << "0 c xdIiOo w q]";

      op = readkbd();
      if ( op == 27 ) {
        op = readkbd();
        if ( op == '[' ) {
          op = readkbd();
          switch ( op ) {
            case 'A':
              b.move( -1, 0 );
              break;
            case 'B':
              b.move( 1, 0 );
              break;
            case 'C':
              b.move( 0, 1 );
              break;
            case 'D':
              b.move( 0, -1 );
              break;
          }
          continue;
        }
      }
      cout << endl;
      switch ( op ) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
          b.setCell( op - '0' );
          break;
        case ' ':
          b.setCell( 0 );
          break;
        case 'c':
          useColor = !useColor;
          break;
        case 'd':
          break;
        case 'I':
          break;
        case 'i':
          break;
        case 'O':
          break;
        case 'o':
          break;
        case 'x':
          break;
        case 'w':
          char filename[255];
          cout << endl;
          cout << "Enter a file name: ";
          cin >> filename;
          b.writeFile( filename );
          break;
        case 'q':
          gameOver = 1;
          done = 1;
          break;
      }
    }
  }

  return 0;
}
