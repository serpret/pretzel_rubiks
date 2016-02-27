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
    bool rubiks::findHighestValueRoute( bool (rubiks::*matchFunc)(), 
                                        int numSteps)
    {

        int initialStep = rotRecordFace.size(); 
        int finalStep = initialStep + numSteps;

        Face    currentFace = up;
        Rot currentRot = clock;
   
        while(true){ 
            //travel to desired step number
            travelToStep( finalStep );
           
            //printRecord(); 

            //begin first rotation
            rotUcount();

            //perform 1st->2nd rotation transform
            rotUcount();
            
            //perform 2nd->3rd rotation transform
            rotUcount();

            //perform 3rd->4th rotation transform ( up clock -> down counter)
            rotUcount();
            rotDcount();

            //perform 4th->5th rotation transform
            rotDcount();
            
            //perform 5th->6th rotation transform
            rotDcount();

            //perform 6th->7th rotation transform ( down clock -> left count) 
            rotDcount();    //this can be optimized
            rotLcount();    //this can be optimized

            //perform 7th->8th rotation transform
            rotLcount();
            
            //perform 8th->9th rotation transform
            rotLcount();

            //perform 9th->10th rot transform ( left clock -> right count) 
            rotLcount();    //this can be optimized
            rotRcount();    //this can be optimized

            //perform 10th->11th rotation transform
            rotRcount();

            //perform 11th->12th rotation transform
            rotRcount();

            //perform 12th->13th rot transform ( right clock -> front count) 
            rotRcount();   //can be optimized 
            rotFcount();   //can be optimized

            //perform 13th->14th rotation transform
            rotFcount();
            
            //perform 14th->15th rotation transform
            rotFcount();

            //perform 15th->16th rot transform ( front clock -> back count) 
            rotFcount();    
            rotBcount();  

            //perform 16th->17th rotation transform
            rotBcount();
            
            //perform 17th->18th rotation transform
            rotBcount();

            //return to original form at this step
            rotBcount();

            //remove steps till we get to a step where not all
            //of the combos have been tested yet
            while(isLastRotInStep() )
            {
                if( rotRecordFace.size() == (initialStep+1) ){
                    popRotStep();
                    return 0;
                } else{ 
                    popRotStep( currentFace, currentRot);
                }
            }
            
            //try next step, this cannot be the last step (guaranteed above)
            moveToNextStep();

        }
        
    }
    //==================================================================
    //==================================================================

    

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



    //==================================================================
    //==================================================================
    void rubiks::travelToStep(int finalStep )
    {
        int currentStep = rotRecordFace.size();
        while( currentStep < finalStep){
            if( rotRecordFace.back() == FIRST_FACE){
                pushRotStep( SEC_FACE, FIRST_ROT);
            } else  {
                pushRotStep( FIRST_FACE, FIRST_ROT);
            }

            ++currentStep;
        }

    }
    //==================================================================
    //==================================================================


    


    //==================================================================
    //==================================================================
    bool rubiks::isLastRotInStep()
    {
        bool isLastRotOfSet;
        bool isLastRotByPreviousStep;
        bool thisStepSecToLast;
        bool prevFaceLast;
        int secToLastRecInd = rotRecordFace.size() -2;

        isLastRotOfSet = (rotRecordFace.back() == LAST_FACE)
                    &&   (rotRecordType.back() == LAST_ROT);

        thisStepSecToLast = (rotRecordFace.back() == SEC_LAST_FACE)
                       &&   (rotRecordType.back() == LAST_ROT);

        prevFaceLast = (rotRecordFace[ secToLastRecInd ] == LAST_FACE);

        return isLastRotOfSet || (thisStepSecToLast && prevFaceLast);

        //return ((rotRecordFace.back() ==  && rotRecordType.back() == clock)
        //|| ((rotRecordFace.back() == front && rotRecordType.back() == clock) 
        //        && (rotRecordFace[ rotRecordFace.size() -2] == back)) );
    }
    //==================================================================
    //==================================================================



    //==================================================================
    //==================================================================
    void rubiks::moveToNextStep()
    {
        Face currentRotFace = rotRecordFace.back();
        Rot currentRot = rotRecordType.back();

        //if( currentRotFace != emptyFace){
            popRotStep();
            if(currentRot == LAST_ROT){
                inc( currentRotFace);
                currentRot = FIRST_ROT;

                if( currentRotFace == rotRecordFace.back()){
                    inc( currentRotFace);
                }
            } else {
                inc(currentRot);
            }
        //} else {
        //    currentRotFace = up;
        //    currentRot = count;
        //}
        pushRotStep( currentRotFace, currentRot);
    }
    //==================================================================
    //==================================================================

}
