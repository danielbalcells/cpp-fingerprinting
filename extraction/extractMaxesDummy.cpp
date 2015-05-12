/* 
 * File:   extractMaxes.cpp
 * Author: daniel
 *
 * Created on 9 de mayo de 2015, 16:21
 */

#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>



/*
 * 
 */
std::vector<std::vector<int>> extractMaxesDummy(std::vector<std::vector<double>> hpfZMLSpectrogram, int logEnable, std::ofstream& logFile){
    if(logEnable == 1){logFile << "Running extractMaxesDummy...\n";}
    std::vector<std::vector<int>> maxes(8,std::vector<int>(2));
    maxes[0][0] = 1;
    maxes[0][1] = 200;
    maxes[1][0] = 2;
    maxes[1][1] = 250;
    maxes[2][0] = 3;
    maxes[2][1] = 30;
    maxes[3][0] = 13;
    maxes[3][1] = 67;
    maxes[4][0] = 40;
    maxes[4][1] = 34;
    maxes[5][0] = 56;
    maxes[5][1] = 150;
    maxes[6][0] = 60;
    maxes[6][1] = 130;
    maxes[7][0] = 70;
    maxes[7][1] = 120;
    return maxes;
}
