#include "rubiks.h++"
#include <thread>
#include <iostream> //used for printing the cube
#include <iomanip>  //used for printing the cube
#include <string>   //used for printing the cube
#include <random>   //used to scramble cube

//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
//            34                                         30            
//                                                                       
//      35    33                                         31    29      
//                                                                       
//36          32                                         24          28 
//                                                                       
//37    39                        4                            25    27
//                                                                       
//38                        3           5                            26 
//                     
//                    2                       6                     
//                  
//                          1           7
//               22                              10
//                                0                   
//               21    23                   9    11  
//
//               20          16       8          12   
//        
//                     19    17      15    13
//
//                           18      14
//
//                                                          
//                                                          
//                                                          
//                               40                                       
//                                                                        
//                         41          47                                   
//                                                                            
//                   42                      46                             
//                                                                           
//                         43          45                                   
//                                                                            
//                               44        
//xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


namespace PRETZEL
{
      
    //==================================================================
    //==================================================================
    void rubiks::print()
    {
    
    // this messy print function is derived from the diagram at the top
    // of this file by performing various regex operations

    // in retrospect it should have probably been implemented with
    // with std out formatting operations instead of strings of spaces

    std::cout << "            "<< square[34] 
                << "      Left                  Back         "
                << square[30] << "            "<< std::endl << std::endl 
                << "      "<< square[35] << "    "<< square[33] 
                << "                                         "<< square[31] 
                << "    "<< square[29] << "      "<< std::endl 
                << std::endl<< square[36] << "          "<< square[32] 
                << "                                         "<< square[24] 
                << "          "<< square[28] << " "<< std::endl << std::endl
                << square[37] << "    "<< square[39] 
                << "                        "<< square[4] 
                << "                            "<< square[25] << "    "
                << square[27]<< std::endl << std::endl<< square[38] 
                << "                        "<< square[3] << "           "
                << square[5] << "                            "<< square[26] 
                << " "<< std::endl << "                     "<< std::endl 
                << "                    "<< square[2] 
                << "          Up           "<< square[6] 
                << "                     "<< std::endl << "                  "
                << std::endl << "                          "<< square[1] 
                << "           "<< square[7]<< std::endl << "               "
                << square[22] << "                              "<< square[10]
                << std::endl << "                                "
                << square[0] << "                   "<< std::endl 
                << "               "<< square[21] << "    "<< square[23] 
                << "                   "<< square[9] << "    "<< square[11] 
                << "  "<< std::endl<< std::endl << "    Front      "
                << square[20] << "          "<< square[16] << "       "
                << square[8] << "          "<< square[12] << "       Right "
                << std::endl << "        "<< std::endl 
                << "                     "<< square[19] << "    "
                << square[17] << "      "<< square[15] << "    "<< square[13]
                << std::endl<< std::endl << "                           "
                << square[18] << "      "<< square[14]<< std::endl
                << std::endl << std::endl << std::endl << std::endl 
                << "                               "<< square[40] 
                << "                                       "<< std::endl 
                << std::endl << "                         "<< square[41] 
                << "          "<< square[47] 
                << "                                   "<< std::endl 
                << std::endl << "                   "<< square[42] 
                << "          Down        "<< square[46] 
                << "                             "<< std::endl << std::endl 
                << "                         "<< square[43] << "          "
                << square[45] << "                                   "
                << std::endl << std::endl << "                               "
                << square[44] << "        " 
                << std::endl;
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
    int rubiks::findSquare(SqType squareToFind)
    {
        for( int i = 0; i < NUM_SQ; ++i)
        {
            if( square[i] == squareToFind) return i;
        }
    }
    //==================================================================
    //==================================================================



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

        rotRecord.push_back( emptyRotPair ); 


        for (int i = 0; i < NUM_SQ; ++i) square[i] = i;
        
        //==============================================
        for(int i = 0; i < NUM_CORNER_CUBES; ++i){
            corners[i].cubeNum = i;

            if( i < 4) corners[i].orient = up;
            else corners[i].orient = down; 
            
        }

        for(int i = 0; i < NUM_EDGE_CUBES; ++i){
            corners[i].cubeNum = i;
            if( i < 4) edges[i].orient = up;
            else if(i < 6) edges[i].orient = right;
            else if(i < 8) edges[i].orient = left;
            else edges[i].orient = down;
        }

        //==============================================
        for(int i = 0; i < NUM_CUBES; ++i)
        {
            cubeNum[i] = i;
            if (i < 8) cubeOrient[i] = up;
            else if(i < 10) cubeOrient[i] = right;
            else if(i < 12) cubeOrient[i] = left;
            else cubeOrient[i] = down;
        }
        //==============================================
        for(int i = 0; i < NUM_CUBES; ++i)
        {
            allCubes[i].cubeNum = i;
            if (i < 8)      allCubes[i].orient = up;
            else if(i < 10) allCubes[i].orient = right;
            else if(i < 12) allCubes[i].orient = left;
            else allCubes[i].orient = down;
        }
   
    } 
    //==================================================================
    //==================================================================


         
    
    //==================================================================
    //==================================================================
    void rubiks::rotUcount()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight(  0,  6,  4,  2);
        rotToTheRight(  7,  5,  3,  1);
        rotToTheRight( 22,  8, 28, 34);
        rotToTheRight( 23,  9, 29, 35);
        rotToTheRight( 16, 10, 30, 36); 
    }
    //==================================================================
    //==================================================================
 
 
    //==================================================================
    //==================================================================
    void rubiks::rotCubeNumsToRight3( int a, int b, int c, int d)
    {
        int temp = allCubes[d].cubeNum;
            
        allCubes[d].cubeNum = allCubes[c].cubeNum;
        allCubes[c].cubeNum = allCubes[b].cubeNum;
        allCubes[b].cubeNum = allCubes[a].cubeNum;
        allCubes[a].cubeNum = temp;
        
    }
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::rotCubeNumsToRight( int a, int b, int c, int d)
    {
        int temp = cubeNum[d];
            
        cubeNum[d] = cubeNum[c];
        cubeNum[c] = cubeNum[b];
        cubeNum[b] = cubeNum[a];
        cubeNum[a] = temp;
        
    }
    //==================================================================
    //==================================================================


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
    void rubiks::rotUcountCubes3()
    {

        //test function
        rotCubeNumsToRight3(0,2,4,6);
        rotCubeNumsToRight3(1,3,5,7);

        Face tempCorner = allCubes[0].orient;
        Face tempEdge = allCubes[1].orient;

        // left -> front -> right -> back -> ...
        // contents of: 3  ->  0  ->  1  ->  2
        //rotation corners

        //front edge
        //if(edges[3].orient == front) edges[0].orient = right;
        //edges[0].orient = nextFaceYrotCount[ edges[3].orient];
        if(allCubes[7].orient == front) allCubes[1].orient = right;

        //front left corner
        //if(corners[3].orient == left) corners[0].orient = front;
        //else if(corners[3].orient == front) corners[0].orient = right;
        //corners[0].orient = nextFaceYrotCount[ corners[3].orient ];
        allCubes[0].orient = nextFaceYrotCount[ allCubes[6].orient ];

        //left edge
        //if(edges[2].orient == left) edges[3].orient = front;
        //edges[3].orient = nextFaceYrotCount[ edges[2].orient ];
        if(allCubes[5].orient == left) allCubes[7].orient = front;

        //back left corner
        //if(corners[2].orient == left) corners[3].orient = front;
        //else if(corners[2].orient == back) corners[3].orient = left;
        //corners[3].orient = nextFaceYrotCount[ corners[2].orient ];
        allCubes[6].orient = nextFaceYrotCount[ allCubes[4].orient ];

        //back edge
        //if(edges[1].orient == back) edges[2].orient = left;
        //edges[2].orient = nextFaceYrotCount[ edges[1].orient ];
        if(allCubes[3].orient == back) allCubes[5].orient = left;
       
        //back right corner 
        //if(corners[1].orient == right) corners[2].orient = back;
        //else if(corners[1].orient == back) corners[2].orient = left;
        //corners[2].orient = nextFaceYrotCount[ corners[1].orient ];
        allCubes[4].orient = nextFaceYrotCount[ allCubes[2].orient ];
    
        //right edge
        //if(tempEdge == right) edges[1].orient = back;
        //edges[1].orient = nextFaceYrotCount[ tempEdge ];
        if(tempEdge == right) allCubes[3].orient = back;
    
        //front right corner
        //if(tempCorner == right) corners[1].orient = back;
        //else if(tempCorner == front) corners[1].orient = right;
        //corners[1].orient = nextFaceYrotCount[ tempCorner];
        allCubes[0].orient = nextFaceYrotCount[ tempCorner ];

        
    } 
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::rotRcountCubes3()
    {

        //test function
    
        rotCubeNumsToRight3(0, 12, 14, 2);
        rotCubeNumsToRight3(8, 13, 9, 1);

        Face tempCorner = allCubes[0].orient;
        Face tempEdge = allCubes[1].orient;

        // up -> front -> down -> back 
        //rotation corners

        //up back corner
        //if(corners[1].orient == up) corners[0].orient = front;
        //else if(corners[1].orient == back) corners[0].orient = up;
        allCubes[0].orient = nextFaceXrotCount[ allCubes[2].orient];

        //back edge 
        //if(edges[5].orient == back) edges[0].orient = up;
        //edges[0].orient = nextFaceXrotCount[ edges[5].orient];
        if(allCubes[9].orient == back) allCubes[1].orient = up;

        //back down corner
        //if(corners[5].orient == down) corners[1].orient = back;
        //else if(corners[5].orient == back) corners[1].orient = up;
        //corners[1].orient = nextFaceXrotCount[ corners[5].orient];
        allCubes[2].orient = nextFaceXrotCount[ allCubes[14].orient];

        //down edge
        //if(corners[8].orient == down) edges[5].orient = back;
        //edges[5].orient = nextFaceXrotCount[ edges[8].orient];
        if(allCubes[13].orient == down) allCubes[9].orient = back;

        //front down corner
        //if(corners[4].orient == front) corners[5].orient = down;
        //else if(corners[4].orient == down) corners[5].orient = back;
        //corners[5].orient = nextFaceXrotCount[ corners[4].orient];
        allCubes[14].orient = nextFaceXrotCount[ allCubes[12].orient];

        //front edge
        //if(edges[4].orient == front) edges[8].orient = down;
        //edges[8].orient = nextFaceXrotCount[ edges[4].orient];
        if(allCubes[8].orient == front) allCubes[13].orient = down;

        //up front corner
        //if(tempCorner == up) corners[4].orient = front;
        //else if(tempCorner == front) corners[4].orient = down;
        //corners[4].orient = nextFaceXrotCount[ tempCorner];
        allCubes[12].orient = nextFaceXrotCount[ tempCorner];

        //up edge
        //if(tempEdge == up) edges[4].orient = front;
        //edges[4].orient = nextFaceXrotCount[ tempEdge];
        if(tempEdge == up) allCubes[8].orient = front;
        
    } 
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotUcountCubes2()
    {

        //test function
        rotCubeNumsToRight(0,2,4,6);
        rotCubeNumsToRight(1,3,5,7);

        Face tempCorner = cubeOrient[0];
        Face tempEdge = cubeOrient[1];

        // left -> front -> right -> back -> ...
        // contents of: 3  ->  0  ->  1  ->  2
        //rotation corners

        //front edge
        //if(edges[3].orient == front) edges[0].orient = right;
        //edges[0].orient = nextFaceYrotCount[ edges[3].orient];
        if(cubeOrient[7] == front) cubeOrient[1] = right;

        //front left corner
        //if(corners[3].orient == left) corners[0].orient = front;
        //else if(corners[3].orient == front) corners[0].orient = right;
        //corners[0].orient = nextFaceYrotCount[ corners[3].orient ];
        cubeOrient[0] = nextFaceYrotCount[ cubeOrient[6] ];

        //left edge
        //if(edges[2].orient == left) edges[3].orient = front;
        //edges[3].orient = nextFaceYrotCount[ edges[2].orient ];
        if(cubeOrient[5] == left) cubeOrient[7] = front;

        //back left corner
        //if(corners[2].orient == left) corners[3].orient = front;
        //else if(corners[2].orient == back) corners[3].orient = left;
        //corners[3].orient = nextFaceYrotCount[ corners[2].orient ];
        cubeOrient[6] = nextFaceYrotCount[ cubeOrient[4] ];

        //back edge
        //if(edges[1].orient == back) edges[2].orient = left;
        //edges[2].orient = nextFaceYrotCount[ edges[1].orient ];
        if(cubeOrient[3] == back) cubeOrient[5] = left;
       
        //back right corner 
        //if(corners[1].orient == right) corners[2].orient = back;
        //else if(corners[1].orient == back) corners[2].orient = left;
        //corners[2].orient = nextFaceYrotCount[ corners[1].orient ];
        cubeOrient[4] = nextFaceYrotCount[ cubeOrient[2] ];
    
        //right edge
        //if(tempEdge == right) edges[1].orient = back;
        //edges[1].orient = nextFaceYrotCount[ tempEdge ];
        if(tempEdge == right) cubeOrient[3] = back;
    
        //front right corner
        //if(tempCorner == right) corners[1].orient = back;
        //else if(tempCorner == front) corners[1].orient = right;
        //corners[1].orient = nextFaceYrotCount[ tempCorner];
        cubeOrient[0] = nextFaceYrotCount[ tempCorner ];

        
    } 
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::rotRcountCubes2()
    {

        //test function
        rotCornerNumsToRight( 1, 0, 4, 5);
        rotEdgeNumsToRight(0, 4, 8, 5);
    
        rotCubeNumsToRight(0, 12, 14, 2);
        rotCubeNumsToRight(8, 13, 9, 1);

        Face tempCorner = cubeOrient[0];
        Face tempEdge = cubeOrient[1];

        // up -> front -> down -> back 
        //rotation corners

        //up back corner
        //if(corners[1].orient == up) corners[0].orient = front;
        //else if(corners[1].orient == back) corners[0].orient = up;
        cubeOrient[0] = nextFaceXrotCount[ cubeOrient[2]];

        //back edge 
        //if(edges[5].orient == back) edges[0].orient = up;
        //edges[0].orient = nextFaceXrotCount[ edges[5].orient];
        if(cubeOrient[9] == back) cubeOrient[1] = up;

        //back down corner
        //if(corners[5].orient == down) corners[1].orient = back;
        //else if(corners[5].orient == back) corners[1].orient = up;
        //corners[1].orient = nextFaceXrotCount[ corners[5].orient];
        cubeOrient[2] = nextFaceXrotCount[ cubeOrient[14]];

        //down edge
        //if(corners[8].orient == down) edges[5].orient = back;
        //edges[5].orient = nextFaceXrotCount[ edges[8].orient];
        if(cubeOrient[13] == down) cubeOrient[9] = back;

        //front down corner
        //if(corners[4].orient == front) corners[5].orient = down;
        //else if(corners[4].orient == down) corners[5].orient = back;
        //corners[5].orient = nextFaceXrotCount[ corners[4].orient];
        cubeOrient[14] = nextFaceXrotCount[ cubeOrient[12]];

        //front edge
        //if(edges[4].orient == front) edges[8].orient = down;
        //edges[8].orient = nextFaceXrotCount[ edges[4].orient];
        if(cubeOrient[8] == front) cubeOrient[13] = down;

        //up front corner
        //if(tempCorner == up) corners[4].orient = front;
        //else if(tempCorner == front) corners[4].orient = down;
        //corners[4].orient = nextFaceXrotCount[ tempCorner];
        cubeOrient[12] = nextFaceXrotCount[ tempCorner];

        //up edge
        //if(tempEdge == up) edges[4].orient = front;
        //edges[4].orient = nextFaceXrotCount[ tempEdge];
        if(tempEdge == up) cubeOrient[8] = front;
        
    } 
    //==================================================================
    //==================================================================

    //==================================================================
    //==================================================================
    void rubiks::rotUcountCubes()
    {

        //test function
        rotCornerNumsToRight( 0, 1, 2, 3);
        rotEdgeNumsToRight(0, 1, 2, 3);

        Face tempCorner = corners[0].orient;
        Face tempEdge = edges[0].orient;

        // left -> front -> right -> back -> ...
        // contents of: 3  ->  0  ->  1  ->  2
        //rotation corners

        //front edge
        if(edges[3].orient == front) edges[0].orient = right;
        //edges[0].orient = nextFaceYrotCount[ edges[3].orient];

        //front left corner
        //if(corners[3].orient == left) corners[0].orient = front;
        //else if(corners[3].orient == front) corners[0].orient = right;
        corners[0].orient = nextFaceYrotCount[ corners[3].orient ];

        //left edge
        if(edges[2].orient == left) edges[3].orient = front;
        //edges[3].orient = nextFaceYrotCount[ edges[2].orient ];

        //back left corner
        //if(corners[2].orient == left) corners[3].orient = front;
        //else if(corners[2].orient == back) corners[3].orient = left;
        corners[3].orient = nextFaceYrotCount[ corners[2].orient ];

        //back edge
        if(edges[1].orient == back) edges[2].orient = left;
        //edges[2].orient = nextFaceYrotCount[ edges[1].orient ];
       
        //back right corner 
        //if(corners[1].orient == right) corners[2].orient = back;
        //else if(corners[1].orient == back) corners[2].orient = left;
        corners[2].orient = nextFaceYrotCount[ corners[1].orient ];
    
        //right edge
        if(tempEdge == right) edges[1].orient = back;
        //edges[1].orient = nextFaceYrotCount[ tempEdge ];
    
        //front right corner
        //if(tempCorner == right) corners[1].orient = back;
        //else if(tempCorner == front) corners[1].orient = right;
        corners[1].orient = nextFaceYrotCount[ tempCorner];

        
    } 
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::rotRcountCubes()
    {

        //test function
        rotCornerNumsToRight( 1, 0, 4, 5);
        rotEdgeNumsToRight(0, 4, 8, 5);

        Face tempCorner = corners[0].orient;
        Face tempEdge = edges[0].orient;

        // up -> front -> down -> back 
        //rotation corners

        //up back corner
        //if(corners[1].orient == up) corners[0].orient = front;
        //else if(corners[1].orient == back) corners[0].orient = up;
        corners[0].orient = nextFaceXrotCount[ corners[1].orient];

        //back edge 
        if(edges[5].orient == back) edges[0].orient = up;
        //edges[0].orient = nextFaceXrotCount[ edges[5].orient];

        //back down corner
        //if(corners[5].orient == down) corners[1].orient = back;
        //else if(corners[5].orient == back) corners[1].orient = up;
        corners[1].orient = nextFaceXrotCount[ corners[5].orient];

        //down edge
        if(corners[8].orient == down) edges[5].orient = back;
        //edges[5].orient = nextFaceXrotCount[ edges[8].orient];

        //front down corner
        //if(corners[4].orient == front) corners[5].orient = down;
        //else if(corners[4].orient == down) corners[5].orient = back;
        corners[5].orient = nextFaceXrotCount[ corners[4].orient];

        //front edge
        if(edges[4].orient == front) edges[8].orient = down;
        //edges[8].orient = nextFaceXrotCount[ edges[4].orient];

        //up front corner
        //if(tempCorner == up) corners[4].orient = front;
        //else if(tempCorner == front) corners[4].orient = down;
        corners[4].orient = nextFaceXrotCount[ tempCorner];

        //up edge
        if(tempEdge == up) edges[4].orient = front;
        //edges[4].orient = nextFaceXrotCount[ tempEdge];
        
    } 
    //==================================================================
    //==================================================================




    //==================================================================
    //==================================================================
    void rubiks::rotUclock()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight(  2,  4,  6,  0);
        rotToTheRight(  1,  3,  5,  7);
        rotToTheRight( 34, 28,  8, 22);
        rotToTheRight( 35, 29,  9, 23);
        rotToTheRight( 36, 30, 10, 16); 
    }
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::rotUtwice()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight(  0,  4);
        rotToTheRight(  2,  6);
        rotToTheRight(  1,  5);
        rotToTheRight(  3,  7);
        rotToTheRight( 22, 28);
        rotToTheRight(  8, 34);
        rotToTheRight( 23, 29);
        rotToTheRight(  9, 35);
        rotToTheRight( 16, 30);
        rotToTheRight( 10, 36);
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotDcount()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 44, 42, 40, 46 );
        rotToTheRight( 43, 41, 47, 45 );
        rotToTheRight( 18, 38, 24, 12 );
        rotToTheRight( 20, 32, 26, 14 );
        rotToTheRight( 19, 39, 25, 13 ); 
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotDclock()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 46, 40, 42, 44 );
        rotToTheRight( 45, 47, 41, 43 );
        rotToTheRight( 12, 24, 38, 18 );
        rotToTheRight( 14, 26, 32, 20 );
        rotToTheRight( 13, 25, 39, 19 ); 
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotDtwice()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 40, 44);
        rotToTheRight( 42, 46);
        rotToTheRight( 43, 47);
        rotToTheRight( 41, 45);
        rotToTheRight( 18, 24);
        rotToTheRight( 12, 38);
        rotToTheRight( 20, 26);
        rotToTheRight( 14, 32);
        rotToTheRight( 19, 25);
        rotToTheRight( 13, 39);
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotLcount()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 38, 36, 34, 32);
        rotToTheRight( 39, 37, 35, 33);
        rotToTheRight(  2, 30, 40, 20);
        rotToTheRight(  4, 24, 42, 22);
        rotToTheRight(  3, 31, 41, 21);
    }
    //==================================================================
    //==================================================================


    //==================================================================
    //==================================================================
    void rubiks::rotLclock()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 32, 34, 36, 38);
        rotToTheRight( 33, 35, 37, 39);
        rotToTheRight( 20, 40, 30,  2);
        rotToTheRight( 22, 42, 24,  4);
        rotToTheRight( 21, 41, 31,  3);
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotLtwice()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 36, 32);
        rotToTheRight( 34, 38);
        rotToTheRight( 35, 39);
        rotToTheRight( 37, 33);
        rotToTheRight(  2, 40);
        rotToTheRight( 30, 20);
        rotToTheRight(  4, 42);
        rotToTheRight( 22, 24);
        rotToTheRight(  3, 41);
        rotToTheRight( 21, 31);
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotRcount()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 12, 10,  8, 14);
        rotToTheRight( 11,  9, 15, 13);
        rotToTheRight(  6, 16, 44, 26);
        rotToTheRight(  7, 17, 45, 27);
        rotToTheRight(  0, 18, 46, 28);
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotRclock()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 14,  8, 10, 12);
        rotToTheRight( 13, 15,  9, 11);
        rotToTheRight( 26, 44, 16,  6);
        rotToTheRight( 27, 45, 17,  7);
        rotToTheRight( 28, 46, 18,  0);
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotRtwice()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight(  8, 12);
        rotToTheRight( 14, 10);
        rotToTheRight(  9, 13);
        rotToTheRight( 15, 11);
        rotToTheRight(  6, 44);
        rotToTheRight( 16, 26);
        rotToTheRight( 17, 27);
        rotToTheRight(  7, 45);
        rotToTheRight(  0, 46);
        rotToTheRight( 18, 28);
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotFcount()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 16, 22, 20, 18);
        rotToTheRight( 23, 21, 19, 17);
        rotToTheRight(  0, 36, 42, 14);
        rotToTheRight(  1, 37, 43, 15);
        rotToTheRight(  2, 38, 44,  8);
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotFclock()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 18, 20, 22, 16);
        rotToTheRight( 17, 19, 21, 23);
        rotToTheRight( 14, 42, 36,  0);
        rotToTheRight( 15, 43, 37,  1);
        rotToTheRight(  8, 44, 38,  2);
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotFtwice()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 16, 20);
        rotToTheRight( 22, 18);
        rotToTheRight( 23, 19);
        rotToTheRight( 21, 17);
        rotToTheRight(  0, 42);
        rotToTheRight( 14, 36);
        rotToTheRight( 37, 15);
        rotToTheRight(  1, 43);
        rotToTheRight(  8, 38);
        rotToTheRight(  2, 44);
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotBcount()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 24, 30, 28, 26);
        rotToTheRight( 25, 31, 29, 27);
        rotToTheRight(  4, 10, 46, 32);
        rotToTheRight(  5, 11, 47, 33);
        rotToTheRight(  6, 12, 40, 34);
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotBclock()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 26, 28, 30, 24);
        rotToTheRight( 27, 29, 31, 25);
        rotToTheRight( 32, 46, 10,  4);
        rotToTheRight( 33, 47, 11,  5);
        rotToTheRight( 34, 40, 12,  6);
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotBtwice()
    {
        //rotate face corners, then face edges, then adjacent face
        //northwest corners, then adjacent face north edges, then
        // adjacent face north east edges

        rotToTheRight( 30, 26);
        rotToTheRight( 24, 28);
        rotToTheRight( 29, 25);
        rotToTheRight( 31, 27);
        rotToTheRight(  4, 46);
        rotToTheRight( 32, 10);
        rotToTheRight(  5, 47);
        rotToTheRight( 33, 11);
        rotToTheRight(  6, 40);
        rotToTheRight( 12, 34);
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

    

    //==================================================================
    //==================================================================
    rubiks::Rot rubiks::invRot(Rot invThis)
    {
        Rot returnValue;

        if      (invThis == count) returnValue = clock;
        else if (invThis == clock) returnValue = count;
        else                           returnValue = twice;

        return returnValue;
    }
    //==================================================================
    //==================================================================




    //==================================================================
    //==================================================================
    void rubiks::inc( Face &faceToIncrement)
    {
         faceToIncrement = static_cast<Face>(faceToIncrement + 1);
    }
    //==================================================================
    //==================================================================




    //==================================================================
    //==================================================================
    void rubiks::inc( Rot &rotTypeToIncrement)
    {
        rotTypeToIncrement = static_cast<Rot>( rotTypeToIncrement +1);
    }
    //==================================================================
    //==================================================================




    //==================================================================
    //==================================================================
    void rubiks::rotToTheRight( SqType a, SqType b)
    {
        SqType temp = square[b];

        square[b] = square[a];
        square[a] = temp;
    } 
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::rotToTheRight( int a, int b, int c, int d)
    {
        int temp = square[d];

        square[d] = square[c];
        square[c] = square[b]; 
        square[b] = square[a];
        square[a] = temp;
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

}
