#include "rubiks.h++"
#include "printToCoord.h++"
#include <thread>
#include <iostream> //used for printing the cube
#include <iomanip>  //used for printing the cube
#include <random>   //used to scramble cube


// two structures for edges and corners, indices marked for both
// indices generally are numbered in a counter clockwise fashion
// looking down on the cube.
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//
//
//
//             2U                                    2
//
//       2U                                                1
//             6L                                    6
// 3U                                                            1
//                                2         
//             6D                                    6
// 7L                       2           1                        5
//      10D                                                9 
//                    3                       1                    
// 7D                                                            5 
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



namespace PRETZEL
{
    using std::cout;
    using std::endl;
    using std::setw;
      


    //==================================================================
    //==================================================================
    rubiks::rubiks()
    {
        std::pair< Face, Rot> emptyRotPair(emptyFace, emptyRot);

        //initiate rotation function pointers 
        rotFuncPtr[up][count]       = &rubiks::rotUcount;
        //rotFuncPtr[up][clock]       = &rubiks::rotUclock;
        //rotFuncPtr[up][twice]       = &rubiks::rotUtwice;
        //rotFuncPtr[down][count]     = &rubiks::rotDcount;
        //rotFuncPtr[down][clock]     = &rubiks::rotDclock;
        //rotFuncPtr[down][twice]     = &rubiks::rotDtwice;
        //rotFuncPtr[left][count]     = &rubiks::rotLcount;
        //rotFuncPtr[left][clock]     = &rubiks::rotLclock;
        //rotFuncPtr[left][twice]     = &rubiks::rotLtwice;
        rotFuncPtr[right][count]    = &rubiks::rotRcount;
        //rotFuncPtr[right][clock]    = &rubiks::rotRclock;
        //rotFuncPtr[right][twice]    = &rubiks::rotRtwice;
        //rotFuncPtr[front][count]    = &rubiks::rotFcount;
        //rotFuncPtr[front][clock]    = &rubiks::rotFclock;
        //rotFuncPtr[front][twice]    = &rubiks::rotFtwice;
        //rotFuncPtr[back][count]     = &rubiks::rotBcount;
        //rotFuncPtr[back][clock]     = &rubiks::rotBclock;
        //rotFuncPtr[back][twice]     = &rubiks::rotBtwice;

        //rotRecord.push_back( emptyRotPair ); 
        //rotRecord.push_back( emptyRotPair ); 
        
        //==============================================
        for(int i = 0; i < NUM_CORNER_CUBES; ++i){
            corners[i].cubeNum = i;

            if( i < 4) corners[i].orient = up;
            else corners[i].orient = down; 
            
        }

        for(int i = 0; i < NUM_EDGE_CUBES; ++i){
            edges[i].cubeNum = i;


            if( i < 4) edges[i].orient = up;
            else if(i < 6) edges[i].orient = right;
            else if(i < 8) edges[i].orient = left;
            else edges[i].orient = down;
        }

        //next face from current face rotations for x,y,z axis
        // clockwise rotations are the same as 3 counter clockwise rotations
        nextFaceXrotCount[up]    = front;
        nextFaceXrotCount[down]  = back;
        nextFaceXrotCount[left]  = left;
        nextFaceXrotCount[right] = right;
        nextFaceXrotCount[front] = down;
        nextFaceXrotCount[back]  = up;

        nextFaceXrotClock[up]    = nextFaceXrotCount[nextFaceXrotCount[front]];
        nextFaceXrotClock[down]  = nextFaceXrotCount[nextFaceXrotCount[back ]];
        nextFaceXrotClock[left]  = nextFaceXrotCount[nextFaceXrotCount[left ]];
        nextFaceXrotClock[right] = nextFaceXrotCount[nextFaceXrotCount[right]];
        nextFaceXrotClock[front] = nextFaceXrotCount[nextFaceXrotCount[down ]];
        nextFaceXrotClock[back]  = nextFaceXrotCount[nextFaceXrotCount[up   ]];

        nextFaceYrotCount[up]    = up;
        nextFaceYrotCount[down]  = down;
        nextFaceYrotCount[left]  = front;
        nextFaceYrotCount[right] = back;
        nextFaceYrotCount[front] = right;
        nextFaceYrotCount[back]  = left;

        nextFaceYrotClock[up]    = nextFaceYrotCount[nextFaceYrotCount[up   ]];
        nextFaceYrotClock[down]  = nextFaceYrotCount[nextFaceYrotCount[down ]];
        nextFaceYrotClock[left]  = nextFaceYrotCount[nextFaceYrotCount[front]];
        nextFaceYrotClock[right] = nextFaceYrotCount[nextFaceYrotCount[back ]];
        nextFaceYrotClock[front] = nextFaceYrotCount[nextFaceYrotCount[right]];
        nextFaceYrotClock[back]  = nextFaceYrotCount[nextFaceYrotCount[left ]];

        nextFaceZrotCount[up]    = left;
        nextFaceZrotCount[down]  = right;
        nextFaceZrotCount[left]  = down;
        nextFaceZrotCount[right] = up;
        nextFaceZrotCount[front] = front;
        nextFaceZrotCount[back]  = back;

        nextFaceZrotClock[up]    = nextFaceZrotCount[nextFaceZrotCount[left ]];
        nextFaceZrotClock[down]  = nextFaceZrotCount[nextFaceZrotCount[right]];
        nextFaceZrotClock[left]  = nextFaceZrotCount[nextFaceZrotCount[down ]];
        nextFaceZrotClock[right] = nextFaceZrotCount[nextFaceZrotCount[up   ]];
        nextFaceZrotClock[front] = nextFaceZrotCount[nextFaceZrotCount[front]];
        nextFaceZrotClock[back]  = nextFaceZrotCount[nextFaceZrotCount[back ]];

    } 
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::print()
    {
        PrintToCoord printBuffer; 
        const int NUM_STR_ROWS = 11;
        const int HOR_SPACE_WID = 5;
        const int BEGIN_COL = 2;
        const int SIDE_SQ_BEGIN_COL = 9;
        int rowOffset[ NUM_STR_ROWS];
        

        printBuffer.setWindow( NUM_STR_ROWS * HOR_SPACE_WID, 25);

        rowOffset[0] = 0;
        for(int i = 1; i < NUM_STR_ROWS; ++i)
        {
            rowOffset[i] = rowOffset[i-1] + HOR_SPACE_WID; 
        }

        
        //draw the left face 
        printBuffer.addStr( returnOrientChar( corners, 7), 
                            BEGIN_COL,
                            SIDE_SQ_BEGIN_COL,
                            PrintToCoord::alignRight );

        std::cout << printBuffer;
                            
                            
        //draw the back square
        //

    }
    //==================================================================
    //==================================================================

         
    //==================================================================
    //==================================================================
    void rubiks::printRecord()
    {
        std::string strFace; //will hold the face to string conversion
        std::string strRot;  //will hold the rotation to string conversion

        std::cout << std::left << std::setw(20) << "Face"
                  << std::left << std::setw(20) << "Rotation type"
                  << std::endl 
                  << std::left << std::setw(20) << "------------"
                  << std::left << std::setw(20) << "------------"
                  << std::endl;

        for( auto itRot = rotRecord.begin(); 
                  itRot != rotRecord.end(); 
                  ++itRot)
        {
            switch( (*itRot).first){
                case up:        strFace =  "up";
                                break;

                case down:      strFace =  "down";
                                break;

                case left:      strFace =  "left";
                                break;

                case right:     strFace =  "right";
                                break;

                case front:     strFace =  "front";
                                break;

                case back:      strFace =  "back";
                                break;

                case emptyFace: strFace = "EMPTY_FACE";
                                break;

                default:        break;
            }

            switch( (*itRot).second){
                case count:         strRot = "\'";
                                    break;

                case clock:         strRot = "";
                                    break;

                case twice:         strRot = "2x";
                                    break;

                case emptyRot:  strRot = "EMPTY_ROTTYPE";   
                                    break;

                default:            break;
            }
            
            std::cout << std::left << std::setw(20) << strFace 
                      << std::left << std::setw(20) << strRot
                      << std::endl;
        }
    }
    //==================================================================
    //==================================================================

   
 
    //==================================================================
    //==================================================================
    void rubiks::rotUcount()
    {

        //test function
        rotCornerNumsToRight( 0, 1, 2, 3);
        rotEdgeNumsToRight(0, 1, 2, 3);

        Face tempCorner = corners[0].orient;
        Face tempEdge = edges[0].orient;

        // left -> front -> right -> back -> ...

        //it's faster to have 1 conditional to determine next orientation,
        // however, we would need to 2 conditionals for corners, in which
        // case it is faster to do array lookups

        //front edge
        if(edges[3].orient == front) edges[0].orient = right;

        //front left corner
        corners[0].orient = nextFaceYrotCount[ corners[3].orient ];

        //left edge
        if(edges[2].orient == left) edges[3].orient = front;

        //back left corner
        corners[3].orient = nextFaceYrotCount[ corners[2].orient ];

        //back edge
        if(edges[1].orient == back) edges[2].orient = left;
       
        //back right corner 
        corners[2].orient = nextFaceYrotCount[ corners[1].orient ];
    
        //right edge
        if(tempEdge == right) edges[1].orient = back;
    
        //front right corner
        corners[1].orient = nextFaceYrotCount[ tempCorner];
        
    } 
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotUtwice()
    {
        rotUcount();
        rotUcount();
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotUclock()
    {
        rotUcount();
        rotUcount();
        rotUcount();
    }
    //==================================================================
    //==================================================================


    //return here
    //==================================================================
    //==================================================================
    void rubiks::rotDcount()
    {

        //test function
        rotCornerNumsToRight( 0, 1, 2, 3);
        rotEdgeNumsToRight(0, 1, 2, 3);

        Face tempCorner = corners[0].orient;
        Face tempEdge = edges[0].orient;

        // left -> front -> right -> back -> ...

        //it's faster to have 1 conditional to determine next orientation,
        // however, we would need to 2 conditionals for corners, in which
        // case it is faster to do array lookups

        //front edge
        if(edges[3].orient == front) edges[0].orient = right;

        //front left corner
        corners[0].orient = nextFaceYrotCount[ corners[3].orient ];

        //left edge
        if(edges[2].orient == left) edges[3].orient = front;

        //back left corner
        corners[3].orient = nextFaceYrotCount[ corners[2].orient ];

        //back edge
        if(edges[1].orient == back) edges[2].orient = left;
       
        //back right corner 
        corners[2].orient = nextFaceYrotCount[ corners[1].orient ];
    
        //right edge
        if(tempEdge == right) edges[1].orient = back;
    
        //front right corner
        corners[1].orient = nextFaceYrotCount[ tempCorner];
        
    } 
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotRcount()
    {

        //test function
        rotCornerNumsToRight( 1, 0, 4, 5);
        rotEdgeNumsToRight(0, 4, 8, 5);

        Face tempCorner = corners[0].orient;
        Face tempEdge = edges[0].orient;

        
        // up -> front -> down -> back 
        //it's faster to have 1 conditional to determine next orientation,
        // however, we would need to 2 conditionals for corners, in which
        // case it is faster to do array lookups

        //up back corner
        corners[0].orient = nextFaceXrotCount[ corners[1].orient];

        //back edge 
        if(edges[5].orient == back) edges[0].orient = up;

        //back down corner
        corners[1].orient = nextFaceXrotCount[ corners[5].orient];

        //down edge
        if(corners[8].orient == down) edges[5].orient = back;

        //front down corner
        corners[5].orient = nextFaceXrotCount[ corners[4].orient];

        //front edge
        if(edges[4].orient == front) edges[8].orient = down;

        //up front corner
        corners[4].orient = nextFaceXrotCount[ tempCorner];

        //up edge
        if(tempEdge == up) edges[4].orient = front;
        
    }
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::rotRtwice()
    {
        rotRcount();
        rotRcount();
    }
    //==================================================================
    //==================================================================

    ////==================================================================
    ////==================================================================
    //void rubiks::rotRcount()
    //{

    //    //test function
    //    rotCornerNumsToRight( 1, 0, 4, 5);
    //    rotEdgeNumsToRight(0, 4, 8, 5);

    //    Face tempCorner = corners[0].orient;
    //    Face tempEdge = edges[0].orient;

    //    
    //    // up -> front -> down -> back 
    //    //rotation corners

    //    //up back corner
    //    //if(corners[1].orient == up) corners[0].orient = front;
    //    //else if(corners[1].orient == back) corners[0].orient = up;
    //    corners[0].orient = nextFaceXrotCount[ corners[1].orient];

    //    //back edge 
    //    if(edges[5].orient == back) edges[0].orient = up;
    //    //edges[0].orient = nextFaceXrotCount[ edges[5].orient];

    //    //back down corner
    //    //if(corners[5].orient == down) corners[1].orient = back;
    //    //else if(corners[5].orient == back) corners[1].orient = up;
    //    corners[1].orient = nextFaceXrotCount[ corners[5].orient];

    //    //down edge
    //    if(corners[8].orient == down) edges[5].orient = back;
    //    //edges[5].orient = nextFaceXrotCount[ edges[8].orient];

    //    //front down corner
    //    //if(corners[4].orient == front) corners[5].orient = down;
    //    //else if(corners[4].orient == down) corners[5].orient = back;
    //    corners[5].orient = nextFaceXrotCount[ corners[4].orient];

    //    //front edge
    //    if(edges[4].orient == front) edges[8].orient = down;
    //    //edges[8].orient = nextFaceXrotCount[ edges[4].orient];

    //    //up front corner
    //    //if(tempCorner == up) corners[4].orient = front;
    //    //else if(tempCorner == front) corners[4].orient = down;
    //    corners[4].orient = nextFaceXrotCount[ tempCorner];

    //    //up edge
    //    if(tempEdge == up) edges[4].orient = front;
    //    //edges[4].orient = nextFaceXrotCount[ tempEdge];
    //    
    //} 
    ////==================================================================
    ////==================================================================






    // 1) travel to step to test (taking initial rotations)
    // 2) test the 18 rotations at that step (return success if true)
    // 3) if the current step being tested is the final rotation in
    //      the current iteration then remove the step.  If no more
    //      steps to be tested return failure
    // 4) repeat 3) until we're at a step with untested rotations
    // 5) iterate the step and start again from 1)

    // numSteps must be 1 or greater
    //==================================================================
    //==================================================================
    //bool rubiks::findHighestValueRoute( bool (rubiks::*matchFunc)(), 
    //                                    int numSteps)
    //{

    //    int initialStep = rotRecordFace.size(); 
    //    int finalStep = initialStep + numSteps;

    //    Face    currentFace = up;
    //    Rot currentRot = clock;
   
    //    while(true){ 
    //        //travel to desired step number
    //        travelToStep( finalStep );
    //       
    //        //printRecord(); 

    //        //begin first rotation
    //        rotUcount();

    //        //perform 1st->2nd rotation transform
    //        rotUcount();
    //        
    //        //perform 2nd->3rd rotation transform
    //        rotUcount();

    //        //perform 3rd->4th rotation transform ( up clock -> down counter)
    //        rotUcount();
    //        rotDcount();

    //        //perform 4th->5th rotation transform
    //        rotDcount();
    //        
    //        //perform 5th->6th rotation transform
    //        rotDcount();

    //        //perform 6th->7th rotation transform ( down clock -> left count) 
    //        rotDcount();    //this can be optimized
    //        rotLcount();    //this can be optimized

    //        //perform 7th->8th rotation transform
    //        rotLcount();
    //        
    //        //perform 8th->9th rotation transform
    //        rotLcount();

    //        //perform 9th->10th rot transform ( left clock -> right count) 
    //        rotLcount();    //this can be optimized
    //        rotRcount();    //this can be optimized

    //        //perform 10th->11th rotation transform
    //        rotRcount();

    //        //perform 11th->12th rotation transform
    //        rotRcount();

    //        //perform 12th->13th rot transform ( right clock -> front count) 
    //        rotRcount();   //can be optimized 
    //        rotFcount();   //can be optimized

    //        //perform 13th->14th rotation transform
    //        rotFcount();
    //        
    //        //perform 14th->15th rotation transform
    //        rotFcount();

    //        //perform 15th->16th rot transform ( front clock -> back count) 
    //        rotFcount();    
    //        rotBcount();  

    //        //perform 16th->17th rotation transform
    //        rotBcount();
    //        
    //        //perform 17th->18th rotation transform
    //        rotBcount();

    //        //return to original form at this step
    //        rotBcount();

    //        //remove steps till we get to a step where not all
    //        //of the combos have been tested yet
    //        while(isLastRotInStep() )
    //        {
    //            if( rotRecordFace.size() == (initialStep+1) ){
    //                popRotStep();
    //                return 0;
    //            } else{ 
    //                popRotStep( currentFace, currentRot);
    //            }
    //        }
    //        
    //        //try next step, this cannot be the last step (guaranteed above)
    //        moveToNextStep();

    //    }
    //    
    //}
    ////==================================================================
    ////==================================================================

    

    ////==================================================================
    ////==================================================================
    //rubiks::Rot rubiks::invRot(Rot invThis)
    //{
    //    Rot returnValue;

    //    if      (invThis == count) returnValue = clock;
    //    else if (invThis == clock) returnValue = count;
    //    else                           returnValue = twice;

    //    return returnValue;
    //}
    ////==================================================================
    ////==================================================================




    ////==================================================================
    ////==================================================================
    //void rubiks::inc( Face &faceToIncrement)
    //{
    //     faceToIncrement = static_cast<Face>(faceToIncrement + 1);
    //}
    ////==================================================================
    ////==================================================================




    ////==================================================================
    ////==================================================================
    //void rubiks::inc( Rot &rotTypeToIncrement)
    //{
    //    rotTypeToIncrement = static_cast<Rot>( rotTypeToIncrement +1);
    //}
    ////==================================================================
    ////==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotCornerNumsToRight( int a, int b, int c, int d)
    {
        int temp = corners[d].cubeNum;
            
        corners[d].cubeNum = corners[c].cubeNum;
        corners[c].cubeNum = corners[b].cubeNum;
        corners[b].cubeNum = corners[a].cubeNum;
        corners[a].cubeNum = temp;
        
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotEdgeNumsToRight( int a, int b, int c, int d)
    {
        int temp = edges[d].cubeNum;
            
        edges[d].cubeNum = edges[c].cubeNum;
        edges[c].cubeNum = edges[b].cubeNum;
        edges[b].cubeNum = edges[a].cubeNum;
        edges[a].cubeNum = temp;
        
    }
    //==================================================================
    //==================================================================



    ////==================================================================
    ////==================================================================
    //void rubiks::addRotStep( Face faceTypePush, Rot rotTypePush)
    //{
    ////    if(!rotRecordFace.empty() ){
    //        if( rotRecordFace.back() == faceTypePush){
    //            inc( faceTypePush);
    //            //faceTypePush = static_cast<Face>(faceTypePush + 1);
    //        }
    ////    }
    //    rotRecordFace.push_back(faceTypePush);    
    //    rotRecordType.push_back(rotTypePush);    
    //    (this->*rotFuncPtr[faceTypePush][rotTypePush])();
    //}
    ////==================================================================
    ////==================================================================


    ////==================================================================
    ////==================================================================
    //void rubiks::removeRotStep( Face& faceTypePop, Rot& rotTypePop)
    //{
    //    Face topFaceRot = rotRecordFace.back();
    //    Rot topTypeRotInv = invRot(rotRecordType.back());
    //    (this->*rotFuncPtr[ topFaceRot ][ topTypeRotInv])();
    //    faceTypePop = rotRecordFace.back();
    //    rotTypePop  = rotRecordType.back();
    //    rotRecordFace.pop_back();
    //    rotRecordType.pop_back();
    //}
    ////==================================================================
    ////==================================================================



    ////==================================================================
    ////==================================================================
    //void rubiks::pushRotStep( Face rotFacePush, Rot rotTypePush)
    //{
    //    rotRecordFace.push_back(rotFacePush);
    //    rotRecordType.push_back(rotTypePush);
    //    (this->*rotFuncPtr[ rotFacePush][ rotTypePush ])();
    //}
    ////==================================================================
    ////==================================================================



    ////==================================================================
    ////==================================================================
    //void rubiks::popRotStep( Face &rotFacePop, Rot &rotTypePop)
    //{
    //    //if(  rotRecordFace.back() != emptyFace){
    //        Rot invOfLastRot = invRot( rotRecordType.back() );
    //        rotFacePop = rotRecordFace.back();
    //        rotTypePop = rotRecordType.back();
    //        (this->*rotFuncPtr[ rotFacePop ][ invOfLastRot])();
    //    //}
    //    rotRecordFace.pop_back();
    //    rotRecordType.pop_back();
    //}
    ////==================================================================
    ////==================================================================



    ////==================================================================
    ////==================================================================
    //void rubiks::popRotStep()
    //{
    //    //if(  rotRecordFace.back() != emptyFace){
    //        Rot invOfLastRot = invRot( rotRecordType.back() );
    //        (this->*rotFuncPtr[ rotRecordFace.back() ][ invOfLastRot])();
    //        rotRecordFace.pop_back();
    //        rotRecordType.pop_back();
    //    //}
    //}
    ////==================================================================
    ////==================================================================



    ////==================================================================
    ////==================================================================
    //void rubiks::travelToStep(int finalStep )
    //{
    //    int currentStep = rotRecordFace.size();
    //    while( currentStep < finalStep){
    //        if( rotRecordFace.back() == FIRST_FACE){
    //            pushRotStep( SEC_FACE, FIRST_ROT);
    //        } else  {
    //            pushRotStep( FIRST_FACE, FIRST_ROT);
    //        }

    //        ++currentStep;
    //    }

    //}
    ////==================================================================
    ////==================================================================


    //


    ////==================================================================
    ////==================================================================
    //bool rubiks::isLastRotInStep()
    //{
    //    bool isLastRotOfSet;
    //    bool isLastRotByPreviousStep;
    //    bool thisStepSecToLast;
    //    bool prevFaceLast;
    //    int secToLastRecInd = rotRecordFace.size() -2;

    //    isLastRotOfSet = (rotRecordFace.back() == LAST_FACE)
    //                &&   (rotRecordType.back() == LAST_ROT);

    //    thisStepSecToLast = (rotRecordFace.back() == SEC_LAST_FACE)
    //                   &&   (rotRecordType.back() == LAST_ROT);

    //    prevFaceLast = (rotRecordFace[ secToLastRecInd ] == LAST_FACE);

    //    return isLastRotOfSet || (thisStepSecToLast && prevFaceLast);

    //    //return ((rotRecordFace.back() ==  && rotRecordType.back() == clock)
    //    //|| ((rotRecordFace.back() == front && rotRecordType.back() == clock) 
    //    //        && (rotRecordFace[ rotRecordFace.size() -2] == back)) );
    //}
    ////==================================================================
    ////==================================================================



    ////==================================================================
    ////==================================================================
    //void rubiks::moveToNextStep()
    //{
    //    Face currentRotFace = rotRecordFace.back();
    //    Rot currentRot = rotRecordType.back();

    //    //if( currentRotFace != emptyFace){
    //        popRotStep();
    //        if(currentRot == LAST_ROT){
    //            inc( currentRotFace);
    //            currentRot = FIRST_ROT;

    //            if( currentRotFace == rotRecordFace.back()){
    //                inc( currentRotFace);
    //            }
    //        } else {
    //            inc(currentRot);
    //        }
    //    //} else {
    //    //    currentRotFace = up;
    //    //    currentRot = count;
    //    //}
    //    pushRotStep( currentRotFace, currentRot);
    //}
    ////==================================================================
    ////==================================================================

    
    //==================================================================
    //==================================================================
    std::string rubiks::returnOrientChar(Cube cubeArray[], int cubeNum)
    {
        std::string orientStr;

        switch(cubeArray[cubeNum].orient){
            case up: orientStr = "U";
                     break;
            case down: orientStr = "D";
                     break;
            case left: orientStr = "L";
                     break;
            case right: orientStr = "U";
                     break;
            case front: orientStr = "F";
                     break;
            case back: orientStr = "B";
                     break;
        }

        return orientStr;
    }
    //==================================================================
    //==================================================================

}
