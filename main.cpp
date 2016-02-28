#include <iostream>
#include "rubiks.h++"
#include <chrono>

using namespace PRETZEL;

int main()
{
    char threadTestStr[5];
    rubiks r1;
    rubiks::SqType j; 
    int sum{0};

    std::cout << "Input a random number between 0 and 47" << std::endl;
    std::cin >> j;


    //r1.scramble(40);

    //r1.print();
    

    auto startTime = std::chrono::high_resolution_clock::now();

    int numIterations = 10000000;
    //for( int i = 0; i < numIterations; ++i)
    //{
    //    r1.rotUcount();
    //    r1.rotRcount();
    //    if( sum & 0x1){
    //        r1.rotUcount();
    //        r1.rotUcount();
    //    } else {
    //        r1.rotUcount();
    //    }
    //    r1.rotRcount();

    //    //sum = sum + r1.findSquare(j);
    //    
    //}


    //for( int i = 0; i < numIterations; ++i)
    //{
    //    r1.rotUcountCubes();
    //    r1.rotRcountCubes();
    //    if( sum & 0x1){
    //        r1.rotUcountCubes();
    //        r1.rotUcountCubes();
    //    } else {
    //        r1.rotUcountCubes();
    //    }
    //    r1.rotRcountCubes();

    //    //sum = sum + r1.findSquare(j);
    //    
    //}


    for( int i = 0; i < numIterations; ++i)
    {
        r1.rotUcountCubes2();
        r1.rotRcountCubes2();
        if( sum & 0x1){
            r1.rotUcountCubes2();
            r1.rotUcountCubes2();
        } else {
            r1.rotUcountCubes2();
        }
        r1.rotRcountCubes2();

        //sum = sum + r1.findSquare(j);
        
    }
    //r1.rotUcountCubes();
    
    auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
    long long elapsedMilliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
    std::cout << "Elapsed milliseconds: " << elapsedMilliSeconds << std::endl;


    r1.print();


    return EXIT_SUCCESS;
}
