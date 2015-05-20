/* 
 * File:   hpFilterRows.cpp
 * Author: daniel
 *
 * Created on 9 de mayo de 2015, 15:55
 */

#include <cstdlib>
#include <vector>
#include "aquila/transform/Spectrogram.h"


/*
 * 
 */
std::vector<std::vector<double>> hpFilterRows(std::vector<std::vector<double>> zmlSpectrogram){
    int i,j;
    std::vector<std::vector<double>> hpfZMLSpectrogram(zmlSpectrogram.size(),std::vector<double>(zmlSpectrogram[0].size()));
    for (j=0;j<zmlSpectrogram[0].size();j++){
        hpfZMLSpectrogram[0][j] = zmlSpectrogram[0][j];
        for (i=1; i<zmlSpectrogram.size();i++){
            hpfZMLSpectrogram[i][j] = zmlSpectrogram[i][j]-0.02*hpfZMLSpectrogram[i-1][j];
        }
    }
    return hpfZMLSpectrogram;
}
