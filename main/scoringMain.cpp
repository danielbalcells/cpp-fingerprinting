/* 
 * File:   scoringMain.cpp
 * Author: daniel
 *
 * Created on 17 de mayo de 2015, 15:31
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <bitset>
#include <vector>
#include <unistd.h>
#include <time.h>

std::vector<float> featureMatching(std::unordered_multimap<std::bitset<48>,std::bitset<32>> sampleFeatures, std::unordered_multimap<std::bitset<48>,std::bitset<32>> candidateFeatures, int numIDs, int logEnable, std::ofstream& logFile);
std::unordered_multimap<std::bitset<48>,std::bitset<32>> loadMultimapFromFile(std::string fileName);
std::unordered_multimap<std::bitset<48>,std::bitset<32>> featureExtraction(std::string inputFileName, int filmID, int logEnable, std::ofstream& logFile);

void printFloatVector(std::vector<float>& inputVector, std::ofstream& printFile);

/*
 * Main function for the scoring workflow: receives the name of an audio file and a candidate features file, and saves
 * the scores computed for each candidate in the specified output file.
 */
int main(int argc, char** argv) {
    /*Get timestamp, for logs*/
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    
    /*Define command-line options here:
    One letter per allowed option. 
    Add a colon for options that accept arguments*/
    /*Allowed options:
     *      -s: Sample audio file name.
            -f: Features file name.
            -o: Output file name: specifies the path to the scores file.
            -i: Number of IDs to use.
            -l: Name of the log file. If this option is specified, the program runs
                in verbose mode and the prints are written to the log file.*/
    std::string commandLineOpts = "s:f:o:i:l:";
    int opt;
    
    /*Declare option argument variables here*/
    std::string sampleFileName, candidatesFileName, outputFileName, logFileName;
    int numIDs = 0;
    int logEnable = 0;
    std::ofstream logFile;
    
    while((opt = getopt(argc,argv,commandLineOpts.c_str())) != -1){
        switch(opt){
            case 's':
                sampleFileName = std::string(optarg);
                break;
            case 'f':
                candidatesFileName = std::string(optarg);
                break;
            case 'o':
                outputFileName = std::string(optarg);
                break;
            case 'i':
                numIDs = std::stoi(std::string(optarg));
                break;
            case 'l':
                logEnable = 1;
                logFileName = std::string(optarg);
                logFile.open(logFileName, std::ios::app);
                logFile << "Log for Recomovie Fingerprinting Scoring.\nRecomovie PAE - May 2015\n";
                logFile << std::string(asctime(timeinfo)) + "\n\n";
                break;
        }
    }
    
    std::unordered_multimap<std::bitset<48>,std::bitset<32>> sampleFeatures, candidateFeatures;
    
    sampleFeatures = featureExtraction(sampleFileName, 0, logEnable, logFile);
    if(logEnable == 1){ logFile << "Obtained fingerprints from sample.\n\n";}
    candidateFeatures = loadMultimapFromFile(candidatesFileName);
    if(logEnable == 1){ logFile << "Loaded candidate fingerprints.\n\n";}
 
    std::vector<float> scores = featureMatching(sampleFeatures, candidateFeatures, numIDs, logEnable, logFile);
    if(logEnable == 1){ logFile << "Obtained scores.\n\n";}
    std::ofstream scoresFile;
    scoresFile.open(outputFileName, std::ios::trunc);
    printFloatVector(scores, scoresFile);
    if(logEnable == 1){ logFile << "Printed scores to file " + outputFileName + ".\n\n";}
    
    if(logEnable == 1){logFile << "----------------------------------------------------\n\n\n";logFile.close();}
    return 0;
}

