#include "rubiks.h++"
#include <thread>
#include <iostream> //used for printing the cube
#include <iomanip>  //used for printing the cube
#include <random>   //used to scramble cube


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
    using std::pair;
    using std::vector;
      


    //==================================================================
    //==================================================================
    rubiks::rubiks()
    {
        std::pair< Face, Rot> emptyRotPair(emptyFace, emptyRot);

        //initiate rotation function pointers 
        rotFuncPtr[up][count]       = &rubiks::rotUcount;
        rotFuncPtr[up][clock]       = &rubiks::rotUclock;
        rotFuncPtr[up][twice]       = &rubiks::rotUtwice;
        rotFuncPtr[down][count]     = &rubiks::rotDcount;
        rotFuncPtr[down][clock]     = &rubiks::rotDclock;
        rotFuncPtr[down][twice]     = &rubiks::rotDtwice;
        rotFuncPtr[left][count]     = &rubiks::rotLcount;
        rotFuncPtr[left][clock]     = &rubiks::rotLclock;
        rotFuncPtr[left][twice]     = &rubiks::rotLtwice;
        rotFuncPtr[right][count]    = &rubiks::rotRcount;
        rotFuncPtr[right][clock]    = &rubiks::rotRclock;
        rotFuncPtr[right][twice]    = &rubiks::rotRtwice;
        rotFuncPtr[front][count]    = &rubiks::rotFcount;
        rotFuncPtr[front][clock]    = &rubiks::rotFclock;
        rotFuncPtr[front][twice]    = &rubiks::rotFtwice;
        rotFuncPtr[back][count]     = &rubiks::rotBcount;
        rotFuncPtr[back][clock]     = &rubiks::rotBclock;
        rotFuncPtr[back][twice]     = &rubiks::rotBtwice;
        rotFuncPtr[emptyFace][emptyRot] = &rubiks::noOp;

        rotRecord.push_back( emptyRotPair ); 
        
        //==============================================
        for(int i = 0; i < NUM_CORNER_CUBES; ++i){
            corners[i].cubeNum = i;

            if( i < 4) corners[i].orient = up;
            else corners[i].orient = down; 

            cornerOrientSolutions[i] = corners[i].orient;            
        }

        for(int i = 0; i < NUM_EDGE_CUBES; ++i){
            edges[i].cubeNum = i;


            if( i < 4) edges[i].orient = up;
            else if(i < 6) edges[i].orient = right;
            else if(i < 8) edges[i].orient = left;
            else edges[i].orient = down;
        
            edgeOrientSolutions[i] = edges[i].orient;
            
        }

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
        const int HOR_SPACE = 6;
        const int VER_SPACE_UP = 3;
        const int VER_SPACE_DIAG = 2;
        const int BEGIN_COL = 2;
        const int END_COL = 80;// NUM_STR_ROWS * HOR_SPACE;
        const int BEGIN_COL_CENTER = (BEGIN_COL+END_COL) / 2 ;
        const int BEGIN_ROW_CENTER = 3;
        const int SIDE_SQ_BEGIN_ROW = 9;
        int rowOffset[ NUM_STR_ROWS];

        int currentRow;
        int currentCol;
        

        printBuffer.setWindow( END_COL +5, SIDE_SQ_BEGIN_ROW + 5*VER_SPACE_UP);

        rowOffset[0] = 0;
        for(int i = 1; i < NUM_STR_ROWS; ++i)
        {
            rowOffset[i] = rowOffset[i-1] + HOR_SPACE; 
        }

        
        //draw the left face 
        //bottom most point first going straight up
        currentCol = BEGIN_COL;
        currentRow = SIDE_SQ_BEGIN_ROW;
        addToPrintBufCube( printBuffer, corners , 7, currentCol, currentRow);

        currentCol = BEGIN_COL;
        currentRow = SIDE_SQ_BEGIN_ROW + VER_SPACE_UP ;
        addToPrintBufCube( printBuffer, edges , 7, currentCol, currentRow );

        currentCol = BEGIN_COL;
        currentRow = SIDE_SQ_BEGIN_ROW + 2*VER_SPACE_UP ;
        addToPrintBufCube( printBuffer, corners , 3, currentCol, currentRow );

        //now the edge over at 10 going up to edge 2
        currentCol = BEGIN_COL + HOR_SPACE;
        currentRow = SIDE_SQ_BEGIN_ROW + VER_SPACE_DIAG ;
        addToPrintBufCube( printBuffer, edges , 10, currentCol, currentRow );

        currentCol = BEGIN_COL + HOR_SPACE;
        currentRow = SIDE_SQ_BEGIN_ROW + 2*VER_SPACE_UP + VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, edges , 2, currentCol, currentRow );

        //now the corner at 6 going straight up to edge6 and corner 2
        currentCol = BEGIN_COL + 2*HOR_SPACE;
        currentRow = SIDE_SQ_BEGIN_ROW + 0*VER_SPACE_UP + 2*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, corners , 6, currentCol, currentRow );

        currentCol = BEGIN_COL + 2*HOR_SPACE;
        currentRow = SIDE_SQ_BEGIN_ROW + 1*VER_SPACE_UP + 2*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, edges , 6, currentCol, currentRow );

        currentCol = BEGIN_COL + 2*HOR_SPACE;
        currentRow = SIDE_SQ_BEGIN_ROW + 2*VER_SPACE_UP + 2*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, corners , 2, currentCol, currentRow );

        //draw the back face
        //bottom most point first going straight up
        currentCol = END_COL;
        currentRow = SIDE_SQ_BEGIN_ROW;
        addToPrintBufCube( printBuffer, corners , 5, currentCol, currentRow);

        currentCol = END_COL;
        currentRow = SIDE_SQ_BEGIN_ROW + VER_SPACE_UP ;
        addToPrintBufCube( printBuffer, edges , 5, currentCol, currentRow );

        currentCol = END_COL;
        currentRow = SIDE_SQ_BEGIN_ROW + 2*VER_SPACE_UP ;
        addToPrintBufCube( printBuffer, corners , 1, currentCol, currentRow );

        //now the edge over at 9 going up to edge 1 
        currentCol = END_COL - HOR_SPACE;
        currentRow = SIDE_SQ_BEGIN_ROW + VER_SPACE_DIAG ;
        addToPrintBufCube( printBuffer, edges , 9, currentCol, currentRow );

        currentCol = END_COL - HOR_SPACE;
        currentRow = SIDE_SQ_BEGIN_ROW + 2*VER_SPACE_UP + VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, edges , 1, currentCol, currentRow );

        //now the corner at 6 going straight up to edge6 and corner 2
        currentCol = END_COL - 2*HOR_SPACE;
        currentRow = SIDE_SQ_BEGIN_ROW + 0*VER_SPACE_UP + 2*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, corners , 6, currentCol, currentRow );

        currentCol = END_COL - 2*HOR_SPACE;
        currentRow = SIDE_SQ_BEGIN_ROW + 1*VER_SPACE_UP + 2*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, edges , 6, currentCol, currentRow );

        currentCol = END_COL - 2*HOR_SPACE;
        currentRow = SIDE_SQ_BEGIN_ROW + 2*VER_SPACE_UP + 2*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, corners , 2, currentCol, currentRow );

        //now lets draw the center cube, start with the right face 
        //bottom most point first going straight up
        currentCol = BEGIN_COL_CENTER;
        currentRow = BEGIN_ROW_CENTER;
        addToPrintBufCube( printBuffer, corners , 4, currentCol, currentRow);

        currentCol = BEGIN_COL_CENTER;
        currentRow = BEGIN_ROW_CENTER + VER_SPACE_UP ;
        addToPrintBufCube( printBuffer, edges , 4, currentCol, currentRow );

        currentCol = BEGIN_COL_CENTER;
        currentRow = BEGIN_ROW_CENTER + 2*VER_SPACE_UP ;
        addToPrintBufCube( printBuffer, corners , 0, currentCol, currentRow );

        currentCol = BEGIN_COL_CENTER;
        currentRow = BEGIN_ROW_CENTER + 2*VER_SPACE_UP + 4*VER_SPACE_DIAG ;
        addToPrintBufCube( printBuffer, corners , 2, currentCol, currentRow );

        //now the edge over at 8 going up to edge 0 
        currentCol = BEGIN_COL_CENTER + HOR_SPACE;
        currentRow = BEGIN_ROW_CENTER + VER_SPACE_DIAG ;
        addToPrintBufCube( printBuffer, edges , 8, currentCol, currentRow );

        currentCol = BEGIN_COL_CENTER + HOR_SPACE;
        currentRow = BEGIN_ROW_CENTER + 2*VER_SPACE_UP + VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, edges , 0, currentCol, currentRow );

        currentCol = BEGIN_COL_CENTER + HOR_SPACE;
        currentRow = BEGIN_ROW_CENTER + 2*VER_SPACE_UP + 3*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, edges , 1, currentCol, currentRow );

        //now the corner at 5 going straight up to edge5 and corner1 
        currentCol = BEGIN_COL_CENTER + 2*HOR_SPACE;
        currentRow = BEGIN_ROW_CENTER + 0*VER_SPACE_UP + 2*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, corners , 5, currentCol, currentRow );

        currentCol = BEGIN_COL_CENTER + 2*HOR_SPACE;
        currentRow = BEGIN_ROW_CENTER + 1*VER_SPACE_UP + 2*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, edges , 5, currentCol, currentRow );

        currentCol = BEGIN_COL_CENTER + 2*HOR_SPACE;
        currentRow = BEGIN_ROW_CENTER + 2*VER_SPACE_UP + 2*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, corners , 1, currentCol, currentRow );

        // now the front face
        //now the edge over at 11 going up to edge3 
        currentCol = BEGIN_COL_CENTER - HOR_SPACE;
        currentRow = BEGIN_ROW_CENTER + VER_SPACE_DIAG ;
        addToPrintBufCube( printBuffer, edges , 11, currentCol, currentRow );

        currentCol = BEGIN_COL_CENTER - HOR_SPACE;
        currentRow = BEGIN_ROW_CENTER + 2*VER_SPACE_UP + VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, edges , 3, currentCol, currentRow );

        currentCol = BEGIN_COL_CENTER - HOR_SPACE;
        currentRow = BEGIN_ROW_CENTER + 2*VER_SPACE_UP + 3*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, edges , 2, currentCol, currentRow );

        //now the corner at 7 going straight up to edge7 and corner3 
        currentCol = BEGIN_COL_CENTER - 2*HOR_SPACE;
        currentRow = BEGIN_ROW_CENTER + 0*VER_SPACE_UP + 2*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, corners , 7, currentCol, currentRow );

        currentCol = BEGIN_COL_CENTER - 2*HOR_SPACE;
        currentRow = BEGIN_ROW_CENTER + 1*VER_SPACE_UP + 2*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, edges , 7, currentCol, currentRow );

        currentCol = BEGIN_COL_CENTER - 2*HOR_SPACE;
        currentRow = BEGIN_ROW_CENTER + 2*VER_SPACE_UP + 2*VER_SPACE_DIAG;
        addToPrintBufCube( printBuffer, corners , 3, currentCol, currentRow );

        std::cout << printBuffer;

    }
    //==================================================================
    //==================================================================

         
    //==================================================================
    //==================================================================
    void rubiks::printRecord() const
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

        rotCornerNumsToRight( 0, 1, 2, 3);
        rotEdgeNumsToRight(0, 1, 2, 3);
        rotCornerOrientsToRight(nextFaceYrotCount, 0, 1, 2, 3);
        rotEdgeOrientsToRight(nextFaceYrotCount,0, 1, 2, 3);
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


    //==================================================================
    //==================================================================
    void rubiks::rotDcount()
    {

        //test function
        rotCornerNumsToRight( 7,  6, 5, 4);
        rotEdgeNumsToRight(  11, 10, 9, 8 );
        rotCornerOrientsToRight(nextFaceYrotClock,  7,  6, 5, 4);
        rotEdgeOrientsToRight(nextFaceYrotClock,   11, 10, 9, 8);

        
    } 
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::rotDtwice()
    {
        rotDcount();
        rotDcount();
    }
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::rotDclock()
    {
        rotDcount();
        rotDcount();
        rotDcount();
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotLcount()
    {

        rotCornerNumsToRight( 3, 2, 6, 7);
        rotEdgeNumsToRight(   2, 6, 10, 7 );
        rotCornerOrientsToRight(nextFaceXrotClock, 3, 2, 6, 7 );
        rotEdgeOrientsToRight(nextFaceXrotClock,   2, 6, 10, 7 );
        
    } 
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotLtwice()
    {
        rotLcount();
        rotLcount();
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotLclock()
    {
        rotLcount();
        rotLcount();
        rotLcount();
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotRcount()
    {

        rotCornerNumsToRight( 1, 0, 4, 5);
        rotEdgeNumsToRight(0, 4, 8, 5);
        rotCornerOrientsToRight(nextFaceXrotCount, 1, 0, 4, 5);
        rotEdgeOrientsToRight(nextFaceXrotCount, 0, 4, 8, 5);
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



    //==================================================================
    //==================================================================
    void rubiks::rotRclock()
    {
        rotRcount();
        rotRcount();
        rotRcount();
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotFcount()
    {
        rotCornerNumsToRight( 0, 3, 7, 4 );
        rotEdgeNumsToRight(   3, 7, 11, 4 );
        rotCornerOrientsToRight(nextFaceZrotCount, 0, 3, 7, 4 );
        rotEdgeOrientsToRight(  nextFaceZrotCount, 3, 7, 11, 4);
    }
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::rotFtwice()
    {
        rotFcount();
        rotFcount();
    }
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::rotFclock()
    {
        rotFcount();
        rotFcount();
        rotFcount();
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotBcount()
    {
        
        rotCornerNumsToRight( 1, 5, 6, 2);
        rotEdgeNumsToRight(   5, 9, 6, 1);
        rotCornerOrientsToRight(nextFaceZrotClock, 1, 5, 6, 2);
        rotEdgeOrientsToRight(  nextFaceZrotClock, 5, 9, 6, 1);
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotBtwice()
    {
        rotBcount();
        rotBcount();
    }
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::rotBclock()
    {
        rotBcount();
        rotBcount();
        rotBcount();
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::pushRot( pair<Face, Rot> rotPair)
    {
        (this->*rotFuncPtr[ rotPair.first][ rotPair.second])();
        rotRecord.push_back( pair<Face, Rot>(rotPair.first, rotPair.second ));
    } 
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::pushRot( RotRecordType rotRecToAdd )
    {
        for(    auto iter = rotRecToAdd.begin(); 
                iter != rotRecToAdd.end();
                ++iter                          )
        {
            pushRot( *iter );
        }
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::popRot()
    {
        Rot invRot;
        switch( rotRecord.back().second)
        {
            case count: invRot = clock; break;
            case twice: invRot = twice; break;
            case clock: invRot = count; break;
            default: break;
        }
        (this->*rotFuncPtr[ rotRecord.back().first][ invRot])();
        rotRecord.pop_back();
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::popRot(int numPops)
    {
        for (int i = 0; i < numPops; ++i){ popRot(); }
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    vector<pair<int,rubiks::RotRecordType>> 
        rubiks::findHighestValueRotRecs(int numRotRecsToKeep, int numSteps )
    {
        vector<pair<int,RotRecordType>> highestValueRotRecs;
        RotRecordType emptyRotRecord;
        emptyRotRecord.push_back( pair<Face, Rot>(emptyFace, emptyRot));
        vector<RotRecordType> highestRotRecs;
        vector<int>           highestValues;
        highestValues.resize(numRotRecsToKeep, 0);
        highestRotRecs.resize(numRotRecsToKeep, emptyRotRecord );

        tempRotRec.clear();
        tempRotRec.push_back( pair<Face, Rot>(emptyFace, emptyRot));

        recurseHighestValue( &rubiks::valuePositionOrient,
                             numSteps,
                             0,
                             highestRotRecs,
                             highestValues,
                             numRotRecsToKeep);

        auto iterVal = highestValues.begin();
        for( auto iterRot = highestRotRecs.begin(); 
            iterRot != highestRotRecs.end();
            ++iterRot)  
        {
            if( (*iterRot).front().first == emptyFace){
                (*iterRot).erase( (*iterRot).begin());
            }
            highestValueRotRecs.push_back( 
                                pair<int, RotRecordType>(*iterVal, *iterRot));
            ++iterVal;
        }

        return highestValueRotRecs;
    }
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    int rubiks::valueExactSolution()
    {
        for(int i = 0; i < NUM_CORNER_CUBES; ++i) {
            if( corners[i].cubeNum != i) return 0;
        }
    
        for(int i  = 0; i < NUM_EDGE_CUBES; ++i) {
            if( edges[i].cubeNum != i) return 0;
        }
        
        for(int i = 0; i < NUM_CORNER_CUBES; ++i) {
            if( corners[i].orient != cornerOrientSolutions[i]) return 1;
        }

        for(int i  = 0; i < NUM_EDGE_CUBES; ++i) {
            if( edges[i].orient != edgeOrientSolutions[i]) return 1;
        }
            
        return 2;
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    int rubiks::valuePositionOrient()
    {
        int value = 0;
        for(int i = 0; i < NUM_CORNER_CUBES; ++i) {
            if( corners[i].cubeNum == i){ 
                value = value +2;
                if(corners[i].orient == cornerOrientSolutions[i]) {
                    value = value +1;
                }
            }
        }

        for(int i  = 0; i < NUM_EDGE_CUBES; ++i) {
            if( edges[i].cubeNum == i) {
                value = value +2;
                if( edges[i].orient == edgeOrientSolutions[i]){
                    value = value +1;
                }
            }
        }
        

        return value;
    }
    //==================================================================
    //==================================================================
    

    //==================================================================
    //==================================================================
    void rubiks::scramble(int steps)
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<> disFace(0,5); 
        std::uniform_int_distribution<> disType(0,2); 

        for(int i = 0; i < steps; ++i)
        {
            (this->*rotFuncPtr[disFace(mt)][disType(mt)])(); 
        }

    }
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    int rubiks::returnCubeNumAtCorner(int cubeInd) const
    {
        return corners[ cubeInd].cubeNum;
    }
    //==================================================================
    //==================================================================



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


    //==================================================================
    //==================================================================
    void rubiks::rotCornerOrientsToRight( Face nextFaceLookup[], 
                                          int a, int b, int c, int d)
    {
        Face temp = nextFaceLookup[corners[d].orient];
            
        corners[d].orient = nextFaceLookup[corners[c].orient];
        corners[c].orient = nextFaceLookup[corners[b].orient];
        corners[b].orient = nextFaceLookup[corners[a].orient];
        corners[a].orient = temp;
        
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotEdgeOrientsToRight( Face nextFaceLookup[], 
                                        int a, int b, int c, int d)
    {
        Face temp = nextFaceLookup[edges[d].orient];
            
        edges[d].orient = nextFaceLookup[edges[c].orient];
        edges[c].orient = nextFaceLookup[edges[b].orient];
        edges[b].orient = nextFaceLookup[edges[a].orient];
        edges[a].orient = temp;
        
    }
    //==================================================================
    //==================================================================



    
    //==================================================================
    //==================================================================
    std::string rubiks::returnOrientStr(const Cube cubeArray[], 
                                        int cubeInd            ) const
    {
        std::string orientStr;

        switch(cubeArray[cubeInd].orient){
            case up: orientStr = "U";
                     break;
            case down: orientStr = "D";
                     break;
            case left: orientStr = "L";
                     break;
            case right: orientStr = "R";
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



    //==================================================================
    //==================================================================
    void rubiks::addToPrintBufCube( PrintToCoord &stringBuffer, 
                                    const Cube cubeArray[], 
                                    int cubeInd, 
                                    int col, 
                                    int row             ) const
    {
        std::string cubeNumStr = std::to_string(cubeArray[cubeInd].cubeNum);
        stringBuffer.addStr( cubeNumStr, 
                            col,
                            row,
                            PrintToCoord::alignRight );
        stringBuffer.addStr( "-",
                             col+1,
                             row,
                             PrintToCoord::alignLeft);
        stringBuffer.addStr( returnOrientStr( cubeArray, cubeInd),
                             col+2,
                             row,
                             PrintToCoord::alignLeft);
    }
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::recurseHighestValue(  int (rubiks::*valueFunc)(),
                                        int stepsToTake,
                                        int curStepNum,
                                        std::vector<RotRecordType> &topRotRecs,
                                        std::vector<int> &topValues,
                                        int numRotRecsToKeep             )
    {
        int nextStepNum = curStepNum +1;
        int value;
        int i = 0;

        if( stepsToTake == curStepNum){
        //if last step go through the rotations running the valueFunction
        // and checking against the topValues
            
            if( tempRotRec.back().first != up){
            rotUcount();
            recurseValueLastStep( (this->*valueFunc)(), up, count, topRotRecs, 
                              topValues, numRotRecsToKeep);
            rotUcount();
            recurseValueLastStep( (this->*valueFunc)(), up, twice, topRotRecs, 
                              topValues, numRotRecsToKeep);
            rotUcount();
            recurseValueLastStep( (this->*valueFunc)(), up, clock, topRotRecs, 
                              topValues, numRotRecsToKeep);
            rotUcount();
            }

            if( tempRotRec.back().first != down){
            rotDcount();
            recurseValueLastStep( (this->*valueFunc)(), down, count, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotDcount();
            recurseValueLastStep( (this->*valueFunc)(), down, twice, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotDcount();
            recurseValueLastStep( (this->*valueFunc)(), down, clock, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotDcount();
            }

            if( tempRotRec.back().first != left){
            rotLcount();
            recurseValueLastStep( (this->*valueFunc)(), left, count, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotLcount();
            recurseValueLastStep( (this->*valueFunc)(), left, twice, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotLcount();
            recurseValueLastStep( (this->*valueFunc)(), left, clock, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotLcount();
            }

            if( tempRotRec.back().first != right){
            rotRcount();
            recurseValueLastStep( (this->*valueFunc)(), right, count, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotRcount();
            recurseValueLastStep( (this->*valueFunc)(), right, twice, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotRcount();
            recurseValueLastStep( (this->*valueFunc)(), right, clock, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotRcount();
            }
            if( tempRotRec.back().first != front){
            rotFcount();
            recurseValueLastStep( (this->*valueFunc)(), front, count, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotFcount();
            recurseValueLastStep( (this->*valueFunc)(), front, twice, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotFcount();
            recurseValueLastStep( (this->*valueFunc)(), front, clock, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotFcount();
            }
            if( tempRotRec.back().first != back){
            rotBcount();
            recurseValueLastStep( (this->*valueFunc)(), back, count, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotBcount();
            recurseValueLastStep( (this->*valueFunc)(), back, twice, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotBcount();
            recurseValueLastStep( (this->*valueFunc)(), back, clock, 
                                    topRotRecs, topValues, numRotRecsToKeep);
            rotBcount();
            }

            recurseValueCurStep( (this->*valueFunc)(), topRotRecs, 
                                 topValues, numRotRecsToKeep);

        } else {

        //if not last step go through rotations and recursively call 
        // this same function (same recursive cycle). If previous iteration
        // rotated the same face then don't do that face.  At the end perform
        // a value check for this current rotation

            if( tempRotRec.back().first != up){
               recurseNextStep( &rubiks::rotUcount, up, valueFunc, 
                                stepsToTake, nextStepNum, topRotRecs, 
                                topValues, numRotRecsToKeep);
            }

            if( tempRotRec.back().first != down){
               recurseNextStep( &rubiks::rotDcount, down, valueFunc, 
                                stepsToTake, nextStepNum, topRotRecs, 
                                topValues, numRotRecsToKeep);
            }

            if( tempRotRec.back().first != left){
               recurseNextStep( &rubiks::rotLcount, left, valueFunc, 
                                stepsToTake, nextStepNum, topRotRecs, 
                                topValues, numRotRecsToKeep);
            }

            if( tempRotRec.back().first != right){
               recurseNextStep( &rubiks::rotRcount, right, valueFunc, 
                                stepsToTake, nextStepNum, topRotRecs, 
                                topValues, numRotRecsToKeep);
            }

            if( tempRotRec.back().first != front){
               recurseNextStep( &rubiks::rotFcount, front, valueFunc, 
                                stepsToTake, nextStepNum, topRotRecs, 
                                topValues, numRotRecsToKeep);
            }

            if( tempRotRec.back().first != back){
               recurseNextStep( &rubiks::rotBcount, back, valueFunc, 
                                stepsToTake, nextStepNum, topRotRecs, 
                                topValues, numRotRecsToKeep);
            }

            recurseValueCurStep( (this->*valueFunc)(), topRotRecs, 
                                 topValues, numRotRecsToKeep);
        }
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::recurseNextStep( void (rubiks::*rotFunc)(),
                                  Face currentFace,
                                  int (rubiks::*valueFunc)(),
                                  int stepsToTake,
                                  int curStepNum,
                                  std::vector<RotRecordType> &topRotRecs,
                                  std::vector<int> &topValues,
                                  int numRotRecsToKeep)
    {
        pair<Face, Rot> firstRotation ( currentFace, count);
        tempRotRec.push_back( firstRotation);

        (this->*rotFunc)();
        recurseHighestValue( valueFunc, stepsToTake, curStepNum, 
                             topRotRecs, topValues, numRotRecsToKeep);
        tempRotRec.back().second = twice;

        (this->*rotFunc)();
        recurseHighestValue( valueFunc, stepsToTake, curStepNum, 
                             topRotRecs, topValues, numRotRecsToKeep);
        tempRotRec.back().second = clock;

        (this->*rotFunc)();
        recurseHighestValue( valueFunc, stepsToTake, curStepNum, 
                             topRotRecs, topValues, numRotRecsToKeep);
        tempRotRec.pop_back();
        (this->*rotFunc)();
    }
    //==================================================================
    //==================================================================
                                

    //==================================================================
    //==================================================================
    void rubiks::recurseValueLastStep( int value,
                                  Face curFace,
                                  Rot  curRot,
                                  std::vector<RotRecordType> &topRotRecs,
                                  std::vector<int> &topValues,
                                  int numRotRecsToKeep)
    {
        if( value > topValues.front()){
            int i = 1;
            while( (value > topValues[i]) && (i < numRotRecsToKeep)){
                topValues[i-1] = topValues[i];
                topRotRecs[i-1] = topRotRecs[i];
                ++i;
            }
            topValues[i-1] = value;
            topRotRecs[i-1] = tempRotRec;
            pair<Face, Rot> curRotPair(curFace, curRot);
            topRotRecs[i-1].push_back( curRotPair);
        }
    }        
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::recurseValueCurStep( int value,
                                  std::vector<RotRecordType> &topRotRecs,
                                  std::vector<int> &topValues,
                                  int numRotRecsToKeep)
    {
        if( value > topValues.front()){
            int i = 1;
            while( (value > topValues[i]) && (i < numRotRecsToKeep)){
                topValues[i-1] = topValues[i];
                topRotRecs[i-1] = topRotRecs[i];
                ++i;
            }
            topValues[i-1] = value;
            topRotRecs[i-1] = tempRotRec;
        }
    }        
    //==================================================================
    //==================================================================

    //==================================================================
    //==================================================================
    void rubiks::noOp()
    {
    }
    //==================================================================
    //==================================================================
}
