//
// Copyright Erik Bryant (erikbryantology@gmail.com)
//

#include <vector>

#include "statistics.hpp"
#include "cell.hpp"

#define LEVEL_EASY   0
#define LEVEL_MEDIUM 1
#define LEVEL_HARD   2

class Board {

  public:

    Statistics    stats;

  private:

    char          name[255];
    int           rows;
    int           cols;
    unsigned int  difficulty;
    int           mines[3];
    bool          initialized;
    Cell          **cells;
    int           curR;
    int           curC;

  public:

    Board();

    void deinit();

    ~Board();

    void readFile( string filename );

    void startNewGame();

    unsigned char countNeighbors( int r, int c );

    int countFlags();

    void print( bool showColorVal );

    //
    // Take the cap off and reveal the cell underneath.
    // Returns True if there is a mine. False otherwise.
    //
    bool reveal();
    void unreveal();

    void revealAll();

    void toggleFlag();

    void toggleQuestion();

    void setDifficulty( unsigned int d );

    bool isSolved();

    void move( int r, int c );

    bool findUnmarkedMine( int *row, int *col );

    bool findWronglyFlaggedMine( int *row, int *col );

    bool findEmptyCellWithCap( int *row, int *col );

    void hint();

    //
    // Functions used by the editor
    //
    void setCell( unsigned int color, bool mirror );
    void fillCells( unsigned int color, bool mirror );
    void deleteRow();
    void deleteCol();
    void insertRowAbove();
    void insertRowBelow();
    void insertColLeft();
    void insertColRight();
    void writeFile( char *filename );
    void replaceColor( unsigned int oldColor, unsigned int newColor );

  private:

    bool reveal( int r, int c );

    void setCell( int r, int c, unsigned int color, bool mirror );

    void fillCellsHelper( int r, int c, unsigned int color, bool mirror );
};
