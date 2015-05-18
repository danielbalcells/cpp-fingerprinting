/* 
 * File:   getScores.cpp
 * Author: daniel
 *
 * Created on 17 de mayo de 2015, 1:57
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>



/*
 * 
 */
std::vector<float> getScores(std::vector<std::vector<int>> modes, int numIDs, int logEnable, std::ofstream& logFile) {
    if(logEnable==1){logFile << "Running getScores...\n";}
    std::vector<float> scores(numIDs);
    
    for(int i = 0; i < numIDs; i++){
        if(modes[i][0] == 0){
            scores[i] = 0;
        }
        else if(modes[i][1] == 0){
            scores[i] = ((float)modes[i][0]-1)/(float)modes[i][0];
        }
        else{
            scores[i] = ((float)modes[i][0]-modes[i][1])/(float)modes[i][0];
        }
    }
    return scores;
}
