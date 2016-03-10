#include <iostream>
#include "rubiks.h++"
#include <chrono>
#include <vector>
#include <string>

using namespace PRETZEL;

void followAndPrintRecord( rubiks&, rubiks::RotRecordType );

int main()
{
    rubiks r1;
    int j = 2; 
    int sum{0};
    std::string clInput = "5";
    std::vector<std::pair<int,rubiks::RotRecordType>> topRotRecords;


    r1.rotBcount();
    r1.rotLclock();
    r1.rotLclock();
    r1.rotRcount();
    r1.rotDtwice();
    r1.rotUclock();
    r1.rotFclock();
    r1.rotBtwice();
    r1.rotUcount();
    r1.scramble(30);
    r1.print();

    auto startTime = std::chrono::high_resolution_clock::now();
    
    while( clInput != "exit"){
    topRotRecords = r1.findHighestValueRotRecs(7,std::stoi(clInput));
    r1.pushRot( topRotRecords[6].second);
    r1.print();
    r1.printRecord();
    std::cout << "Above value: " << topRotRecords[6].first << std::endl;
    std::cout << "exit? " ;
    std::cin >> clInput;
    }

    auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
    r1.print(); 
    long long elapsedMilliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count();
    followAndPrintRecord( r1, topRotRecords[6].second);
    std::cout << "Above value: " << topRotRecords[6].first << std::endl;
    std::cout << "Elapsed milliseconds: " << elapsedMilliSeconds << std::endl;


    return EXIT_SUCCESS;
}


void followAndPrintRecord( rubiks& rubiksToPrint , rubiks::RotRecordType recordToFollow )
{
    rubiksToPrint.pushRot( recordToFollow );
    rubiksToPrint.print();
    rubiksToPrint.printRecord();
    rubiksToPrint.popRot( recordToFollow.size() );
}
