/* 
 * File:   subtractMean.cpp
 * Author: daniel
 *
 * Created on 9 de mayo de 2015, 15:53
 */

#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>
#include "aquila/transform/Spectrogram.h"



/*
 * Subtracts the mean of the input log spectrogram to make it zero-mean
 */
std::vector<std::vector<double>> subtractMean(std::vector<std::vector<double>>& logSpectrogram, int logEnable, std::ofstream& logFile){
    if(logEnable == 1){logFile << "Running subtractMean...\n";}
    double mean = 0;
    
    int spectrogramLength = logSpectrogram.size();
    int spectrogramHeight = logSpectrogram[0].size();
    std::vector<std::vector<double>> zmlSpectrogram(spectrogramLength,std::vector<double>(spectrogramHeight));
    
    //find mean
    int i,j;
    for(i=0; i<spectrogramLength; i++){
        for(j=0; j<spectrogramHeight; j++){
            mean += logSpectrogram[i][j];
        }
    }
    mean = mean/(spectrogramLength*spectrogramHeight);
    if(logEnable == 1){logFile << "Spectrogram mean is " + std::to_string(mean) + ".\nSubtracting mean...";}
    
    //subtract mean
    for(i=0; i<spectrogramLength; i++){
        for(j=0; j<spectrogramHeight; j++){
            zmlSpectrogram[i][j] = logSpectrogram[i][j]-mean;
        }
    }
    
    if(logEnable == 1){
        logFile << " Done.\n";
        //Optionally, find mean again
        for(i=0; i<spectrogramLength; i++){
            for(j=0; j<spectrogramHeight; j++){
                mean += zmlSpectrogram[i][j];
            }
        }
        mean = mean/(spectrogramLength*spectrogramHeight);
        logFile << "New spectrogram mean is " + std::to_string(mean) + ".\n";
    }
    
return zmlSpectrogram;
}
