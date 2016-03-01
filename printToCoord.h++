#include <string>
#include <vector>
#ifndef __PRETZEL_PRINT_COORD__
#define __PRETZEL_PRINT_COORD__

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//  Author: Sergy Pretetsky
//
//  email: serpret.spam@gmail.com (yes this is a spam email, yes i do check it)
//
//  description: 
//              PrintToCoord is a class that makes it easier to print strings
//              directly to certain coordinates on the screen by using
//              a string buffer.
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx




//================= Class Modifiers ==================================
//----------------------------------------------
//  void setWindow(int cols, int rows)
//      Description:
//                  sets the window size of the string buffer which will
//                  get printed
//      Precondition:
//                  cols and rows are integers bigger than 0
//
//      Postconditoin:
//                  windows column and row size is set
//
//----------------------------------------------
//  void addStr( string str, int col, int row, AlignType alignment)
//      Description:
//                  adds the string to the col and row given to the 
//                  string buffer.  alignment allows you to allign 
//                  the string the to the left or right of the given
//                  row col coordinate.
//      Precondition:
//                  none
//      Postcondition:
//                  str is put into the string buffer at col, row
//                  coordinates
//====================================================================



//================  Class accessors ==================================
//----------------------------------------------
//  void print()
//      Description: 
//                  the string buffer is printed to the screen
//      Precondition:
//                  none
//      Postcondition:
//                  the string buffer is printed to the screen
//====================================================================



//================  Friend Functions =================================
//----------------------------------------------
//  friend ostream &operator<<( ostream &output,  PrintToCoord printMe)
//      Description:
//              the string buffer is sent to the ostream output
//      Precondition:
//              none
//      Postcondition:
//              the string buffer is sent to the ostream output
//====================================================================



namespace PRETZEL
{
    class PrintToCoord {
        public:
            enum AlignType{ alignLeft, alignRight};

            PrintToCoord() = default;
            ~PrintToCoord() = default;

            void setWindow( int cols, int rows);
            void addStr( std::string str, int col, int row, AlignType alignment);
            void print();
            friend std::ostream &operator<<( std::ostream &output, 
                                             PrintToCoord printMe);
        private:
            int windowRows{1};
            int windowCols{1};
            std::vector<std::string> strRows;
    };
}
#endif
