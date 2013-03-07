//
// Copyright Erik Bryant (erikbryantology@gmail.com)
// GPLv2 http://www.gnu.org/licenses/gpl-2.0.html
//

#define CAP_NONE     0
#define CAP_BLANK    1
#define CAP_FLAG     2
#define CAP_QUESTION 3

class Cell {

  private:

    bool          cell;
    bool          mine;
    unsigned char cap;
    unsigned char neighbors;
    unsigned int  color;

  public:

    Cell() :
      cell(0),
      mine(0),
      cap(CAP_NONE),
      color(0)
    {
    }

    void clearCell()
    {
      mine = 0;
      if ( cell ) {
        cap   = CAP_BLANK;
      } else {
        color = 0;
        cap   = CAP_NONE;
      }
    }

    void setCell( bool isCell, unsigned int cellColor )
    {
      cell = isCell;
      if ( cell ) {
        mine  = 0;
        color = cellColor;
        cap   = CAP_BLANK;
      } else {
        mine  = 0;
        color = 0;
        cap   = CAP_NONE;
      }
    }

    void setHasMine( bool hasMine )
    {
      if ( cell ) {
        mine = hasMine;
      }
    }

    bool isCell()
    {
      return ( cell );
    }

    void setNeighbors( unsigned char n )
    {
      neighbors = n;
    }

    unsigned char getNeighbors()
    {
      return neighbors;
    }

    unsigned int getColor()
    {
      return ( color );
    }

    void setColor( unsigned int newColor )
    {
      if ( isCell() ) {
        color = newColor;
      }
    }

    unsigned char getCapType()
    {
      return cap;
    }

    bool hasMine()
    {
      return ( mine );
    }

    bool hasFlag()
    {
      return ( cap == CAP_FLAG || cap == CAP_QUESTION );
    }

    bool hasCap()
    {
      return ( cap != CAP_NONE );
    }

    void clearFlag()
    {
      cap = CAP_BLANK;
    }

    void toggleFlag()
    {
      switch ( cap ) {
        case CAP_NONE:
          break;
        case CAP_FLAG:
          cap = CAP_BLANK;
          break;
        case CAP_BLANK:
        case CAP_QUESTION:
          cap = CAP_FLAG;
          break;
        default:
          // TODO: Signal internal error
          break;
      }
    }

    void toggleQuestion()
    {
      switch ( cap ) {
        case CAP_NONE:
          break;
        case CAP_QUESTION:
          cap = CAP_BLANK;
          break;
        case CAP_FLAG:
        case CAP_BLANK:
          cap = CAP_QUESTION;
          break;
        default:
          // TODO: Signal internal error
          break;
      }
    }

    bool reveal()
    {
      if ( !cell ) {
        return false;
      }

      if ( cap != CAP_BLANK ) {
        return false;
      }

      cap = CAP_NONE;

      return mine;
    }

    void unreveal()
    {
      if ( cell && cap == CAP_NONE ) {
        if ( mine ) {
          cap = CAP_FLAG;
        } else {
          cap = CAP_BLANK;
        }
      }
    }

    bool isSolved()
    {
      if ( !cell ) {
        return true;
      }

      if ( mine ) {
        return ( cap == CAP_FLAG );
      } else {
        return ( cap == CAP_NONE );
      }

      // TODO: Error...should not have reached here
      return false;
    }

};

