#ifndef __PRETZEL_RUBIKS__
#define __PRETZEL_RUBIKS__
#include <vector>


// two structures for edges and corners
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//                                2         
//                                                                       
//                          2           1  
//                     
//                    3          up           1                     
//                  
//                          3           0 
//                    7                       5
//                                0                   
//
//                    7                       5
//                                4 
//       front             11            8              right
//
//                                4                                      
//                                                          
//                                                          
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//  Author: Sergy Pretetsky

//  email: serpret.spam@gmail.com (yes this is a spam email, yes i do check it)

//  description: This class holds a rubiks cube combination and helps with
//               with solving the rubiks cube.
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

//int findSquare( SqType squareToFind)
//      precondition: squareToFind is a square (as shown in the diagram
//                  at the top of this file) within the rubiks cube.
//      postcondition: the location of the square is returned
//====================================================================



//======== Class Modifiers ===========================================
//all void rotXYYYYY() functions (ex: rotUclock() , rotDtwice() )
//      precondition: none
//      postcondition: the rubiks cube will rotate the squares correctly
//                  according to the rotation that was performed.  For
//                  example, rotUclock will rotate all of the cubes in
//                  the "up" face clockwise.  This means the contents
//                  of square 0 will be moved to 2, the contents of
//                  square 16 will be moved to square 36, the contents
//                  of square 8 will be moved to square 22, etc.

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
            static const int NUM_CORNER_CUBES{8};// = 8;
            static const int NUM_EDGE_CUBES{12};// = 12;

            //rubiks cube has 6 faces
            enum Face { up      = 0,
                        down    = 1,
                        left    = 2,
                        right   = 3,
                        front   = 4,
                        back    = 5,
                        numFaces,
                        emptyFace      };

            Face nextFaceXrotCount[numFaces];
            Face nextFaceXrotClock[numFaces];
            Face nextFaceYrotCount[numFaces];
            Face nextFaceYrotClock[numFaces];
            Face nextFaceZrotClock[numFaces];
            Face nextFaceZrotCount[numFaces];

            //each face can be rotated 90 degrees counter-clockwise or
            //clockwise, or 180 degrees which we call twice.
            enum Rot { count = 0,
                           twice = 1,
                           clock = 2,
                           numRots,
                           emptyRot  };


            

            typedef std::vector<std::pair< Face, Rot>> RotRecordType;

            rubiks();
            rubiks( const rubiks&) = default; 
            ~rubiks() = default;

            void printRecord();

            void rotUcount();
            void rotUtwice(); //this function is currently unoptimized
            void rotUclock(); //this function is currently unoptimized

            void rotDcount();
            void rotDtwice();
            void rotDclock();

            void rotLcount();
            void rotLtwice();
            void rotLclock();

            void rotRcount();
            void rotRtwice();
            void rotRclock();

            void rotFcount();
            void rotFtwice();
            void rotFclock();

            void rotBcount();
            void rotBtwice();
            void rotBclock();

            void scramble(int steps);
            //Rot invRot(Rot );
            //void inc( Face&);
            //void inc( Rot& );

        private:
            void (rubiks::*rotFuncPtr[numFaces][numRots])(void);
            void rotCornerNumsToRight( int, int, int, int); 
            void rotEdgeNumsToRight( int, int, int, int);           
            //void addRotStep( Face&, Rot&);
            //void removeRotStep( Face&, Rot&);
            //void pushRotStep( Face, Rot, vector<Face>&, vector<Rot>&);
            //void popRotStep( Face&, Rot&, vector<Face>&, vector<Rot>&);
            //void pushRotStep( Face, Rot );
            //void popRotStep( Face&, Rot&);
            //void popRotStep();
            //void travelToStep(int); //helper function for solveBreadthRecursion
            //bool isLastRotInStep(); //helper function for solveBreadthRecursion
            //void moveToNextStep();  //helper function for solveBreadthRecursion

            struct Cube{
                int cubeNum;
                Face orient;
            } ;

            Cube corners[NUM_CORNER_CUBES];
            Cube edges[NUM_EDGE_CUBES];

            RotRecordType rotRecord;

    };
}
#endif
