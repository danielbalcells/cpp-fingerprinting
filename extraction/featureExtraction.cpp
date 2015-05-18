/* 
 * File:   extractionController.cpp
 * Author: daniel
 *
 * Created on 14 de mayo de 2015, 13:06
 */

#include <cstdlib>
#include <cstdlib>
#include <iostream> //Used for basic prints
#include <fstream>  //Used for log file handling
#include <unistd.h> //Used for option handling
#include <aquila/aquila.h> //Used for audio processing
#include <vector>
#include <unordered_map>
#include <time.h>
#include <exception>
using namespace std;

Aquila::WaveFile load8kHzfile(std::string inputFileName, int logEnable, std::ofstream& logFile);
Aquila::Spectrogram computeSpectrogram(Aquila::WaveFile inFile, int logEnable, std::ofstream& logFile);
std::vector<std::vector<double>> computeLog(Aquila::Spectrogram spectrogram, int logEnable, std::ofstream& logFile);
std::vector<std::vector<double>> subtractMean(std::vector<std::vector<double>>& logSpectrogram, int logEnable, std::ofstream& logFile);
std::vector<std::vector<double>> hpFilterRows(std::vector<std::vector<double>> zmlSpectrogram);
std::vector<std::vector<int>> extractMaxes(std::vector<std::vector<double>> hpfZMLSpectrogram);
std::vector<std::vector<int>> pairPoints(std::vector<std::vector<int>> maxes, int logEnable, std::ofstream& logFile);
std::unordered_multimap<std::bitset<48>,std::bitset<32>> formatMultimap(std::vector<std::vector<int>> pointPairs, int filmID, int logEnable, std::ofstream& logFile);


void printDoubleMatrix(std::vector<std::vector<double>>& inputMatrix, std::ofstream& printFile);
void printIntMatrix(std::vector<std::vector<int>>& inputMatrix, std::ofstream& printFile);
std::vector<std::vector<int>> extractMaxesDummy(std::vector<std::vector<double>> hpfZMLSpectrogram, int logEnable, std::ofstream& logFile); //Deboogie-woogie

/*
 * Returns the fingerprint descriptor of the input file specified by inputFileName and filmID
 */
std::unordered_multimap<std::bitset<48>,std::bitset<32>> featureExtraction(std::string inputFileName, int filmID, int logEnable, std::ofstream& logFile) {
    //Get 8kHz resampled input file DONE
    Aquila::WaveFile audio8kHz = load8kHzfile(inputFileName,logEnable,logFile);
    if(logEnable == 1){logFile << "Loaded file " << inputFileName << ", resampled to 8kHz.\n\n";}
       
    
    //Obtain spectrogram DONE
    Aquila::Spectrogram spectrogram = computeSpectrogram(audio8kHz, logEnable, logFile);
    if(logEnable == 1){logFile << " Done.\n\n";}
    
    //Obtain log spectrogram DONE
    std::vector<std::vector<double>> logSpectrogram = computeLog(spectrogram, logEnable, logFile);
    if(logEnable == 1){
        logFile << " Done.\n";
    
        //Plot log spectrogram to file
        std::string logSpect = "logs/logSpectrogram.txt";
        std::ofstream logSpectrogramFile;
        logSpectrogramFile.open(logSpect, std::ios::trunc);
        logFile << "Printing Log Spectrogram to file " + logSpect + "...";
        printDoubleMatrix(logSpectrogram,logSpectrogramFile);
        logFile << " Done.\n\n";
    }
        
//    //Obtain zero-mean log spectrogram DONE, CURRENTLY DISABLED
//    std::vector<std::vector<double>> zmlSpectrogram = subtractMean(logSpectrogram, logEnable, logFile);
//    if(logEnable == 1){
//        logFile << "Computed Zero-Mean Log Spectrogram.\n";
//        //Plot zero-mean log spectrogram to file
//        std::string zmlSpect = "logs/zmlSpectrogram.txt";
//        std::ofstream zmlSpectrogramFile;
//        zmlSpectrogramFile.open(zmlSpect, std::ios::app);
//        logFile << "Printing Log Spectrogram to file " + zmlSpect + "...";
//        printDoubleMatrix(zmlSpectrogram,zmlSpectrogramFile);
//        logFile << " Done.\n\n";
//    }
//    
////    //Filter spectrogram rows CURRENTLY DISABLED
////    /*std::vector<std::vector<double>> hpfZMLSpectrogram = hpFilterRows(zmlSpectrogram);
////    if(logEnable == 1){logFile << "Filtered Spectrogram rows.\n";}*/
////    
////    //Extract salient points from filtered spectrogram
////    std::vector<std::vector<int>> maxes = extractMaxes(hpfZMLSpectrogram);
////    if(logEnable == 1){logFile << "Extracted salient spectrogram points.\n";}
//    
    //Extract salient points from log spectrogram 
    std::vector<std::vector<int>> maxes = extractMaxesDummy(logSpectrogram,logEnable,logFile);
    if(0 == 1){ //PUNK
   
        logFile << "Extracted salient spectrogram points.\n";
        
        //Plot maxes to file
        std::string maxesName = "logs/sampleMaxes.txt";
        std::ofstream maxesFile;
        maxesFile.open(maxesName, std::ios::trunc);
        logFile << "Printing Maxes to file " + maxesName + "...";
        printIntMatrix(maxes,maxesFile);
        logFile << " Done.\n\n";
    }
    
    //Pair salient points DONE
    std::vector<std::vector<int>> pointPairs = pairPoints(maxes,logEnable, logFile);
    if(logEnable == 1){
        logFile << "Paired salient spectrogram points.\n";
        
        //Plot pairs to file
        std::string pairsName = "logs/samplePairs.txt";
        std::ofstream pairsFile;
        pairsFile.open(pairsName, std::ios::trunc);
        logFile << "Printing Pairs to file " + pairsName + "...";
        printIntMatrix(pointPairs,pairsFile);
        logFile << " Done.\n\n";
    }
    
    
    //Format points into unordered multimap DONE
    std::unordered_multimap<std::bitset<48>,std::bitset<32>> featureMultimap;
    featureMultimap = formatMultimap(pointPairs,filmID,logEnable,logFile);
    if(logEnable == 1){logFile << "Formatted multimap.\n\n";}
    
    return featureMultimap;
}
