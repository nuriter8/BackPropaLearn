#include "BP.h"
#include "NetworkVisualizer.h"
#include <iostream>
using namespace std;

int main(){


    BackPropagation BP_Network({4, 3, 4, 5, 2});
    
    vector<size_t> architecture = {4, 2, 2, 3, 4, 5, 2};
    NetworkVisualizer visualizer(architecture);
    
    // Ejecutar
    visualizer.run();
    
}