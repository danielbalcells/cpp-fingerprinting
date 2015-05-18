/* 
 * File:   main.cpp
 * Author: daniel
 *
 * Created on 7 de mayo de 2015, 11:27
 */

#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <unistd.h>
#include <fstream>
#include <iostream>


std::vector<float> featureMatching(std::unordered_multimap<std::bitset<48>,std::bitset<32>> sampleFeatures, std::unordered_multimap<std::bitset<48>,std::bitset<32>> candidateFeatures, int numIDs, int logEnable, std::ofstream& logFile);
std::unordered_multimap<std::bitset<48>,std::bitset<32>> formatMultimap(std::vector<std::vector<int>> pointPairs, int filmID, int logEnable, std::ofstream& logFile);
void saveMultimapToFile(std::unordered_multimap<std::bitset<48>,std::bitset<32>> map, std::string fileName, std::ios::openmode oMode);
std::unordered_multimap<std::bitset<48>,std::bitset<32>> loadMultimapFromFile(std::string fileName);

void printIntMatrix(std::vector<std::vector<int>>& inputMatrix, std::ofstream& printFile);

/*
 * This function runs the controller for the matching stage of the Recomovie fingerprinting audio descriptor.
 * It is the main function of the Test Matching configuration, providing basic input and output for the controller so
 * it can run independently from the scoring workflow.
 */
int main(int argc, char** argv) {
    /*Get timestamp, for logs*/
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime (&rawtime);
    int numIDs = 3; //number of candidates + 1, as sample always has ID 0
    
    /*Define command-line options here:
    One letter per allowed option. 
    Add a colon for options that accept arguments*/
    /*Allowed options:
     *      
            -l: Name of the log file. If this option is specified, the program runs
                in verbose mode and the prints are written to the log file.*/
    std::string commandLineOpts = "l:";
    int opt;
    
    /*Declare option argument variables here*/
    std::string logFileName;
    int logEnable = 0;
    std::ofstream logFile;
    
    while((opt = getopt(argc,argv,commandLineOpts.c_str())) != -1){
        switch(opt){
            case 'l':
                logEnable = 1;
                logFileName = std::string(optarg);
                logFile.open(logFileName, std::ios::app);
                logFile << "Log for Recomovie Fingerprinting Matching Test.\nRecomovie PAE - May 2015\n";
                logFile << std::string(asctime(timeinfo)) + "\n\n";
                break;
        }
    }
    
    //create test multimaps
    std::unordered_multimap<std::bitset<48>,std::bitset<32>> sampleFeatures, candidateFeatures, candidateFeaturesID1, candidateFeaturesID2;
    
    std::vector<std::vector<int>> samplePointPairs(4,std::vector<int>(4));
    samplePointPairs[0][0] = 150;
    samplePointPairs[0][1] = 165;
    samplePointPairs[0][2] = 31;
    samplePointPairs[0][3] = 40;
    samplePointPairs[1][0] = 163;
    samplePointPairs[1][1] = 120;
    samplePointPairs[1][2] = 13;
    samplePointPairs[1][3] = 46;
    samplePointPairs[2][0] = 90;
    samplePointPairs[2][1] = 118;
    samplePointPairs[2][2] = 21;
    samplePointPairs[2][3] = 90;
    samplePointPairs[3][0] = 99;
    samplePointPairs[3][1] = 124;
    samplePointPairs[3][2] = 28;
    samplePointPairs[3][3] = 100;
    sampleFeatures = formatMultimap(samplePointPairs,0,0,logFile);
    if(logEnable != 0){logFile << "Created sample feature multimap.\n\n";}
    
    std::vector<std::vector<int>> candidatePointPairsID1(5,std::vector<int>(4));
    candidatePointPairsID1[0][0] = 150;
    candidatePointPairsID1[0][1] = 165;
    candidatePointPairsID1[0][2] = 31;
    candidatePointPairsID1[0][3] = 712;
    candidatePointPairsID1[1][0] = 164;
    candidatePointPairsID1[1][1] = 120;
    candidatePointPairsID1[1][2] = 13;
    candidatePointPairsID1[1][3] = 713;
    candidatePointPairsID1[2][0] = 90;
    candidatePointPairsID1[2][1] = 118;
    candidatePointPairsID1[2][2] = 733;
    candidatePointPairsID1[2][3] = 90;
    candidatePointPairsID1[3][0] = 99;
    candidatePointPairsID1[3][1] = 124;
    candidatePointPairsID1[3][2] = 28;
    candidatePointPairsID1[3][3] = 900;
    candidatePointPairsID1[4][0] = 90;
    candidatePointPairsID1[4][1] = 118;
    candidatePointPairsID1[4][2] = 21;
    candidatePointPairsID1[4][3] = 890;
    candidateFeaturesID1 = formatMultimap(candidatePointPairsID1,1,0,logFile);
    
    std::vector<std::vector<int>> candidatePointPairsID2(5,std::vector<int>(4));
    candidatePointPairsID2[0][0] = 150;
    candidatePointPairsID2[0][1] = 165;
    candidatePointPairsID2[0][2] = 31;
    candidatePointPairsID2[0][3] = 711;
    candidatePointPairsID2[1][0] = 164;
    candidatePointPairsID2[1][1] = 150;
    candidatePointPairsID2[1][2] = 13;
    candidatePointPairsID2[1][3] = 713;
    candidatePointPairsID2[2][0] = 90;
    candidatePointPairsID2[2][1] = 118;
    candidatePointPairsID2[2][2] = 732;
    candidatePointPairsID2[2][3] = 90;
    candidatePointPairsID2[3][0] = 99;
    candidatePointPairsID2[3][1] = 124;
    candidatePointPairsID2[3][2] = 28;
    candidatePointPairsID2[3][3] = 900;
    candidatePointPairsID2[4][0] = 90;
    candidatePointPairsID2[4][1] = 118;
    candidatePointPairsID2[4][2] = 21;
    candidatePointPairsID2[4][3] = 898;
    candidateFeaturesID2 = formatMultimap(candidatePointPairsID2,2,0,logFile);
    if(logEnable == 1){
        //Plot pairs to file
        std::string pairsName = "logs/candidatePairs.txt";
        std::ofstream pairsFile;
        pairsFile.open(pairsName, std::ios::trunc);
        logFile << "Printing Pairs to file " + pairsName + "...";
        printIntMatrix(candidatePointPairsID1,pairsFile);
        printIntMatrix(candidatePointPairsID2,pairsFile);
        logFile << " Done.\n\n";
    }
    
    candidateFeatures = std::unordered_multimap<std::bitset<48>,std::bitset<32>>(candidateFeaturesID1);
    candidateFeatures.insert(candidateFeaturesID2.begin(),candidateFeaturesID2.end());
    saveMultimapToFile(candidateFeatures,"candidatesProva.txt", std::ios::trunc);
    if(logEnable != 0){logFile << "Created candidate feature multimap.\n\n";}
    try{
        std::vector<float> scores = featureMatching(sampleFeatures, candidateFeatures, numIDs, logEnable, logFile);
        if(logEnable == 1){logFile << "----------------------------------------------------\n\n\n";logFile.close();}
        return 0;
    }
    catch(std::exception& e){
        if(logEnable == 1){logFile << "Unknown error.\n";}
    }
    
}

