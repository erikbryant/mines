//
// Copyright Erik Bryant (erikbryantology@gmail.com)
// GPLv2 http://www.gnu.org/licenses/gpl-2.0.html
//

#include <iostream>

using namespace std;

#include <time.h>

class Statistics {

  private:

    unsigned int level;
    unsigned int hints;
    unsigned int flags;
    unsigned int questions;
    unsigned int undos;
    unsigned int wronglyMarkedMines;
    time_t       startTime;

  public:

    Statistics() :
      level(0),
      hints(0),
      flags(0),
      questions(0),
      undos(0),
      wronglyMarkedMines(0),
      startTime(0)
    {
    }

    void reset()
    {
      level = 0;
      hints = 0;
      flags = 0;
      questions = 0;
      undos = 0;
      wronglyMarkedMines = 0;
      startTimer();
    }

    void setLevel( int l )
    {
      level = l;
    }

    unsigned int getLevel()
    {
      return level;
    }

    void startTimer()
    {
      startTime = time( NULL );
    }

    void usedHint()
    {
      hints++;
    }

    void usedFlag()
    {
      flags++;
    }

    void usedQuestion()
    {
      questions++;
    }

    void usedUndo()
    {
      undos++;
    }

    void usedMarkWrong()
    {
      wronglyMarkedMines++;
    }

    unsigned int getHints()
    {
      return ( hints );
    }

    unsigned int getFlags()
    {
      return ( flags );
    }

    unsigned int getQuestions()
    {
      return ( questions );
    }

    unsigned int getUndos()
    {
      return ( undos );
    }

    unsigned int getMarkWrong()
    {
      return ( wronglyMarkedMines );
    }

    unsigned int getElapsedSecs()
    {
      return ( time( NULL ) - startTime );
    }

};

