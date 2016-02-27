#include <iostream>
#include "rubiks.h++"
#include <chrono>

using namespace PRETZEL;

int main()
{
    char threadTestStr[5];
    rubiks r1;

    // sanity check, the series of rotations below should
    // leave me with the same cube i started with
    //r1.rotUcount();
    //r1.rotUclock();
    //r1.rotUtwice();
    //r1.rotUtwice();
    //r1.rotDcount();
    //r1.rotDclock();
    //r1.rotDtwice();
    //r1.rotDtwice();
    //r1.rotLcount();
    //r1.rotLclock();
    //r1.rotLtwice();
    //r1.rotLtwice();
    //r1.rotRcount();
    //r1.rotRclock();
    //r1.rotRtwice();
    //r1.rotRtwice();
    //r1.rotFcount();
    //r1.rotFclock();
    //r1.rotFtwice();
    //r1.rotFtwice();
    //r1.rotBcount();
    //r1.rotBclock();
    //r1.rotBtwice();
    //r1.rotBtwice();


    // more testing
    r1.rotDcount();
    r1.rotLtwice();
    r1.rotDclock();
    r1.rotBclock();
    r1.rotRclock();
    //r1.rotUtwice();
    //r1.rotFcount();
    //r1.rotUtwice();
    //r1.addRotStep( tempFace, tempType);
    //tempFace = PRETETSKY::front;
    //tempType = PRETETSKY::clock;
    //r1.addRotStep( tempFace, tempType);
    //r1.print();
    r1.print();
    //r1.scramble(40);
    

    auto startTime = std::chrono::high_resolution_clock::now();
    //r1.solveComplete(7);

    
    //for(int i = 1; !r1.solveBreadthRecursion( &rubiks::isMatchComplete, i); ++i){

    //    std::cout << "Step " << i << " no solutions found" << std::endl;
    //}


    
    auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
    long long elapsedMilliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
    std::cout << "Elapsed milliseconds: " << elapsedMilliSeconds << std::endl;


    r1.print();
    //r1.print();
    r1.printRecord();


    //r1.scramble(5);

    return EXIT_SUCCESS;
}
