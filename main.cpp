#include <iostream>
#include "rubiks.h++"
#include <chrono>

using namespace PRETZEL;

int main()
{
    rubiks r1;
    int j = 2; 
    int sum{0};

    r1.print();
    r1.rotUcount();
    r1.print();
    r1.rotRcount();
    r1.print();

    auto startTime = std::chrono::high_resolution_clock::now();

    int numIterations = 10000000;
    for( int i = 0; i < numIterations; ++i)
    {
        r1.rotUcount();
        r1.rotRcount();
        if( sum & 0x1){
            r1.rotUcount();
            r1.rotUcount();
        } else {
            r1.rotUcount();
        }
        r1.rotRcount();

    }
    
    auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
    long long elapsedMilliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
    std::cout << "Elapsed milliseconds: " << elapsedMilliSeconds << std::endl;




    return EXIT_SUCCESS;
}
