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
    //r1.rotUcount();
    //r1.rotUcount();
    //r1.rotUcount();
    //r1.rotUcount(); // tested ok
    //r1.rotRcount();
    //r1.rotRcount();
    //r1.rotRcount();
    //r1.rotRcount(); // tested ok
    //r1.rotDcount();
    //r1.rotDcount();
    //r1.rotDcount();
    //r1.rotDcount(); //tested ok
    r1.print();

    auto startTime = std::chrono::high_resolution_clock::now();

    int numIterations = 10000000;
    for( int i = 0; i < numIterations; ++i)
    {
        r1.rotUcount();
        sum = sum + r1.returnCubeNumAtCorner(1);
        r1.rotRcount();
        sum = sum + r1.returnCubeNumAtCorner(1);
        r1.rotUcount();
        sum = sum + r1.returnCubeNumAtCorner(1);
        r1.rotUcount();
        sum = sum + r1.returnCubeNumAtCorner(1);
        r1.rotRcount();
        sum = sum + r1.returnCubeNumAtCorner(1);

    }
    
    auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
    long long elapsedMilliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
    std::cout << "Sum value: " << sum << std::endl;
    std::cout << "Elapsed milliseconds: " << elapsedMilliSeconds << std::endl;




    return EXIT_SUCCESS;
}
