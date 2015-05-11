/* 
 * File:   subtractMean.cpp
 * Author: daniel
 *
 * Created on 9 de mayo de 2015, 15:53
 */

#include <cstdlib>
#include <vector>
#include "aquila/transform/Spectrogram.h"



/*
 * Subtracts the mean of the input log spectrogram to make it zero-mean
 */
std::vector<std::vector<double>> subtractMean(std::vector<std::vector<double>> logSpectrogram){
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
    
    //subtract mean
    for(i=0; i<spectrogramLength; i++){
        for(j=0; j<spectrogramHeight; j++){
            zmlSpectrogram[i][j] = logSpectrogram[i][j]-mean;
        }
    }
    
return zmlSpectrogram;
}
