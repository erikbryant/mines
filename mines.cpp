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

#if 0
  cout << "Size of Cell: " << sizeof( Cell ) << endl;
  cout << "/" << char(27) << "[1m" << "1 bold text" << char(27) << "[0m" << "/" << endl;
  cout << "/" << char(27) << "[2m" << "2 light text" << char(27) << "[0m" << "/" << endl;
  cout << "/" << char(27) << "[3m" << "3 normal text" << char(27) << "[0m" << "/" << endl;
  cout << "/" << char(27) << "[4m" << "4 underline text" << char(27) << "[0m" << "/" << endl;
  cout << "/" << char(27) << "[5m" << "5 normal text" << char(27) << "[0m" << "/" << endl;
  cout << "/" << char(27) << "[6m" << "6 normal text" << char(27) << "[0m" << "/" << endl;
  cout << "/" << char(27) << "[7m" << "7 inverse text" << char(27) << "[0m" << "/" << endl;
  cout << "/" << char(27) << "[8m" << "8 white on white text" << char(27) << "[0m" << "/" << endl;
  cout << "/" << char(27) << "[34m" << "34 blue on white text" << char(27) << "[0m" << "/" << endl;
  cout << "/" << char(27) << "[35m" << "35 purple on white text" << char(27) << "[0m" << "/" << endl;
  cout << "/" << char(27) << "[40m" << "40 black on black text" << char(27) << "[0m" << "/" << endl;
  cout << "/" << char(27) << "[41m" << "41 black on red text" << char(27) << "[0m" << "/" << endl;
#endif

  while ( !done ) {
    Board b;
    char boardName[255];
    bool useColor = false;
    bool gameOver = false;
    bool editMode = false;
    bool dirty    = false;
    bool mirror   = false;

    system( "ls boards" );
    cout << "Board name: ";
    cin >> boardName;
    b.readFile( boardName );

    unsigned int difficulty;
    do {
      cout << "Difficulty [0..2]: ";
      cin >> difficulty;
    } while ( difficulty > 3 );

    b.setDifficulty( difficulty );

    b.startNewGame();

    while ( !gameOver || dirty ) {
      char op;

      system( "clear" );
      b.print( useColor );
      if ( editMode ) {
        cout << "Editing";
        if ( mirror ) {
          cout << " (mirroring)";
        }
        cout << " [<arrows> ";
        for ( int i=1; i<=9; i++ ) {
          cout << char(27) << "[" << 40 + i << "m";
          cout << i;
          cout << char(27) << "[0m";
        }
        cout << "0 c xdIiOo s m f w p q]";
      } else {
        cout << "Your move [<arrows> r f ? h c e g n q]";
      }

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

      if ( editMode ) {
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
            b.setCell( op - '0', mirror );
            dirty = true;
            break;
          case ' ':
            b.setCell( 0, mirror );
            dirty = true;
            break;
          case 'c':
            useColor = !useColor;
            break;
          case 'd':
            b.deleteRow();
            dirty = true;
            break;
          case 'I':
            b.insertColLeft();
            dirty = true;
            break;
          case 'i':
            b.insertColRight();
            dirty = true;
            break;
          case 'O':
            b.insertRowAbove();
            dirty = true;
            break;
          case 'o':
            b.insertRowBelow();
            dirty = true;
            break;
          case 'x':
            b.deleteCol();
            dirty = true;
            break;
          case 's':
            char oldColor;
            char newColor;
            do {
              cout << "Replace old color # [1-9]: ";
              oldColor = readkbd();
            } while ( oldColor < '1' || oldColor > '9' );
            cout << endl;
            do {
              cout << "With new color # [1-9]: ";
              newColor = readkbd();
            } while ( newColor < '1' || newColor > '9' );
            cout << endl;
            b.replaceColor( oldColor - '0', newColor - '0' );
            dirty = true;
            break;
          case 'm':
            mirror = !mirror;
            break;
          case 'f':
            do {
              cout << "Enter color # [1-9]: ";
              op = readkbd();
            } while ( op < '1' || op > '9' );
            cout << endl;
            b.fillCells( op - '0', mirror );
            dirty = true;
            break;
          case 'w':
            char filename[255];
            cout << endl;
            cout << "Enter a file name: ";
            cin >> filename;
            b.writeFile( filename );
            dirty = false;
            break;
          case 'p':
            editMode = false;
            gameOver = false;
            b.startNewGame();
            break;
          case 'q':
            if ( dirty ) {
              cout << endl;
              do {
                cout << "You have unsaved changes. Are you sure you want to quit? [y/n] ";
                op = readkbd();
              } while ( op != 'y' && op != 'n' );
              if ( op == 'n' ) {
                continue;
              } else {
                cout << endl;
              }
            }
            dirty = false;
            gameOver = 1;
            done = 1;
            break;
        }
      } else {
        switch ( op ) {
          case 'r':
            if ( b.reveal() ) {
              cout << "You exposed a mine. You lose!" << endl;
              cout << "Do you want to undo that move? [y/n]";
              do {
                op = readkbd();
                if ( op == 'y' || op == 'Y' ) {
                  b.unreveal();
                  cout << "Undone. Enjoy the rest of your game." << endl;
                } else if ( op == 'n' ) {
                  gameOver = true;
                }
               } while ( op != 'y' && op != 'n' );
              cout << endl;
            }
            break;
          case 'f':
            b.toggleFlag();
            break;
          case '?':
            b.toggleQuestion();
            break;
          case 'h':
            b.hint();
            break;
          case 'c':
            useColor = !useColor;
            break;
          case 'e':
            editMode = true;
            break;
          case 'g':
            b.revealAll();
            cout << "You have given up." << endl;
            gameOver = true;
            break;
          case 'n':
            gameOver = true;
            break;
          case 'q':
            if ( dirty ) {
              cout << endl;
              do {
                cout << "You have unsaved changes. Are you sure you want to quit? [y/n] ";
                op = readkbd();
              } while ( op != 'y' && op != 'n' );
              if ( op == 'n' ) {
                continue;
              } else {
                cout << endl;
              }
            }
            dirty = false;
            gameOver = 1;
            done = 1;
            break;
          default:
            cout << "'" << op << "' is not a valid operation. Pick one of:" << endl;
            cout << "r   (Reveal)" << endl;
            cout << "f   (Flag)" << endl;
            cout << "?   (tentative marker)" << endl;
            cout << "h   (Hint)" << endl;
            cout << "c   (show Color vals)" << endl;
            cout << "e   (go into Edit mode)" << endl;
            cout << "g   (Give up on this board)" << endl;
            cout << "n   (New game)" << endl;
            cout << "q   (Quit)" << endl;
            break;
        }

        if ( b.isSolved() ) {
          system( "clear" );
          b.print( useColor );
          cout << "Congratulations! You have won!" << endl;
          gameOver = true;
        }
      }
    }

    unsigned int secs = b.stats.getElapsedSecs();
    unsigned int hours = secs / 3600;
    secs -= hours * 3600;
    unsigned int mins  = secs / 60;
    secs -= mins * 60;

    cout << "      Undos: " << b.stats.getUndos() << endl;
    cout << "      Hints: " << b.stats.getHints() << endl;
    cout << "      Flags: " << b.stats.getFlags() << endl;
    cout << "  Questions: " << b.stats.getQuestions() << endl;
    cout << "Wrong Flags: " << b.stats.getMarkWrong() << endl;
    cout << "       Time: " << hours << ":" << mins << ":" << secs << endl;
    cout << endl;
  }

  return 0;
}
