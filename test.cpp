#include "BP.h"
#include "NetworkVisualizer.h"
#include <iostream>
using namespace std;

int main(){


    // compile with: g++ -o cod BP.cpp NetworkVisualizer.cpp test.cpp -lsfml-graphics -lsfml-window -lsfml-system


    BackPropagation BP_Network({4, 3, 4, 5, 2});
    
   
    NetworkVisualizer visualizer(BP_Network);
    
    // Ejecutar
    visualizer.run();
    
}