#ifndef __PRETZEL_RUBIKS__
#define __PRETZEL_RUBIKS__
#include <vector>
#include "string"
#include "printToCoord.h++"

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//  Author: Sergy Pretetsky
//
//  email: serpret.spam@gmail.com (yes this is a spam email, yes i do check it)
//
//  description: This class holds a rubiks cube combination and helps with
//               with solving the rubiks cube.
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx



//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// Below is a representation of the rubiks cube and how it is internally
// represented.  Corners and edges are stored in separate data structures
// which is why many corners and edges have the same index.  Orientation
// of the cube is represnted as a letter after the index.  The middle shape 
// is the cube from an isometric view, the two side angled squares are
// the left face projected outward and the back face projected outward.
// Some of the corners/edges are represented twice on the main cube and on the
// projected faces.
//U = up
//D = down
//L = left
//R = right
//F = front
//B = back
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//
//  left        2-U                                     2-U    back          
//                                                                       
//        2-U                       2-U                       1-U        
//              6-L                                     6-L              
//  3-U                       2-U         1-U                       1-U  
//                                                                       
//              6-D     3-U          up         1-U     6-D              
//  7-L                                                             5-R  
//       10-D                 3-U         0-U                 9-D        
//                      7-L                     5-R                      
//  7-D                             0-U                             5-D  
//                                                                       
//                      7-D                     5-D                      
//                                  4-R                                  
//                           11-D         8-D                            
//                                                                       
//            front                 4-D            right 
//
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx




//======== Constructors and Destructors ==============================
// rubiks()
//      preconditions: none
//      postcondition: a rubiks object is created in solved form 
//
// all other destructors and constructors defaulted
//====================================================================



//======== Class Access functions ====================================
//void print()
//      preconditions: none
//      postcondition: prints the current rubiks cube out to standard 
//                  out in the format seen at the top of this file

//void printRecord()
//      precondition: none
//      postcondition: prints the current rotation steps found in
//                      the rotation record to screen

//====================================================================



//======== Class Modifiers ===========================================
//all void rotXYYYYY() functions (ex: rotUclock() , rotDtwice() )
//      precondition: none
//      postcondition: the rubiks cube will rotate the cubes correctly
//                  according to the rotation that was performed.  For
//                  example, rotUclock will rotate all of the cubes in
//                  the "up" face clockwise. 

//void scramble(int numSteps)
//      precondition: none
//      postcondition: the cube has numSteps random rotations 
//                  performed on it.
//====================================================================



//======== Other/ Helper Functions ===================================
//Rot invRot( Rot)
//      Precondition: Rot is a valid rotation type (clock, count, or
//                  twice).
//      Postcondition: a rotation is returned to inverse the effects
//                  of the rotation supplied as the paramter (if clock
//                  rotation is given in paramter count is returned)

//void inc( Face& );
//      Precondition: none
//      Postcondition: Face is incremented to the next face, this 
//                  function doesn't check if the face is the last 
//                  face, so it can return invalid faces if, for 
//                  example, an empty face is input as a parameter.

//void inc( Rot&)
//      Precondition: none
//      Postcondition: Rot is incremented to the next rotation, this
//                  function doesn't check if the rotation is the last
//                  rotation, so it can return invalid rotations if, 
//                  for example, an empty rotation is input as a param

//====================================================================



namespace PRETZEL
{
    class rubiks 
    {

        public:
            static const int NUM_CORNER_CUBES{8};
            static const int NUM_EDGE_CUBES{12};

            //rubiks cube has 6 faces
            enum Face { up      = 0,
                        down    = 1,
                        left    = 2,
                        right   = 3,
                        front   = 4,
                        back    = 5,
                        numFaces,
                        emptyFace      };

            //each face can be rotated 90 degrees counter-clockwise or
            //clockwise, or 180 degrees which we call twice.
            enum Rot { count = 0,
                       twice = 1,
                       clock = 2,
                       numRots,
                       emptyRot  };

            struct Cube{
                int cubeNum;
                Face orient;
            } ;

            Face nextFaceXrotCount[numFaces];
            Face nextFaceXrotClock[numFaces];
            Face nextFaceYrotCount[numFaces];
            Face nextFaceYrotClock[numFaces];
            Face nextFaceZrotClock[numFaces];
            Face nextFaceZrotCount[numFaces];

            typedef std::vector<std::pair< Face, Rot>> RotRecordType;

            rubiks();
            rubiks( const rubiks&) = default; 
            ~rubiks() = default;

            void print() ;
            void printRecord() const;

            void rotUcount();
            void rotUtwice(); //this function is currently unoptimized
            void rotUclock(); //this function is currently unoptimized

            void rotDcount();
            void rotDtwice(); //this function is currently unoptimized
            void rotDclock(); //this function is currently unoptimized

            void rotLcount(); 
            void rotLtwice(); //this function is currently unoptimized
            void rotLclock(); //this function is currently unoptimized

            void rotRcount(); 
            void rotRtwice(); //this function is currently unoptimized
            void rotRclock(); //this function is currently unoptimized

            void rotFcount(); 
            void rotFtwice(); //this function is currently unoptimized
            void rotFclock(); //this function is currently unoptimized

            void rotBcount(); 
            void rotBtwice(); //this function is currently unoptimized
            void rotBclock(); //this function is currently unoptimized

            std::vector<std::pair<int,RotRecordType>> 
                findHighestValueRotRecs( int numRotRecsToKeep, int numSteps);

            void recurseHighestValue( int (rubiks::*valueFunc)(),
                                      int stepsToTake,
                                      int curStepNum,
                                      std::vector<RotRecordType> &topRotRecs,
                                      std::vector<int> &topValues,
                                      int numRotRecsToKeep);

            int valueExactSolution();

            void scramble(int steps);

            int returnCubeNumAtCorner(int) const;
            //void inc( Face&);
            //void inc( Rot& );

        private:
            void (rubiks::*rotFuncPtr[numFaces][numRots])(void);
            void rotCornerNumsToRight(  int, int, int, int); 
            void rotEdgeNumsToRight(    int, int, int, int);           
            void rotCornerOrientsToRight(   Face[],   int, int, int, int);
            void rotEdgeOrientsToRight(     Face[],   int, int, int, int);
            std::string returnOrientStr( const Cube[], int) const;
            void addToPrintBufCube( PrintToCoord&, const Cube[], 
                                    int, int, int) const;
            //void recurseHighestValue( int (rubiks::*valueFunc)(),
            //                          int stepsToTake,
            //                          int curStepNum,
            //                          std::vector<RotRecordType> &topRotRecs,
            //                          std::vector<int> &topValues,

            void recurseNextStep( void (rubiks::*rotFunc)(),
                                  Face currentFace,
                                  int (rubiks::*valueFunc)(),
                                  int stepsToTake,
                                  int curStepNum,
                                  std::vector<RotRecordType> &topRotRecs,
                                  std::vector<int> &topValues,
                                  int numRotRecsToKeep);
            void recurseValueLastStep( int value,
                                  Face curFace,
                                  Rot curRot,
                                  std::vector<RotRecordType> &topRotRecs,
                                  std::vector<int> &topValues,
                                  int numRotRecsToKeep);
            void recurseValueCurStep( int value,
                                  std::vector<RotRecordType> &topRotRecs,
                                  std::vector<int> &topValues,
                                  int numRotRecsToKeep);
                                  

            Cube corners[NUM_CORNER_CUBES];
            Cube edges[NUM_EDGE_CUBES];

            Face cornerOrientSolutions[NUM_CORNER_CUBES];
            Face edgeOrientSolutions[NUM_EDGE_CUBES];

            RotRecordType rotRecord;

    };
}
#endif
