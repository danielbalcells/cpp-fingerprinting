/* 
 * File:   indexingMain.cpp
 * Author: daniel
 *
 * Created on 17 de mayo de 2015, 17:00
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <bitset>
#include <time.h>
#include <unistd.h>

std::unordered_multimap<std::bitset<48>,std::bitset<32>> featureExtraction(std::string inputFileName, int filmID, int logEnable, std::ofstream& logFile);
void saveMultimapToFile(std::unordered_multimap<std::bitset<48>,std::bitset<32>> map, std::string fileName, std::ios::openmode oMode);


/*
 * Main function for the indexing workflow: receives the name of an audio file and an ID, and saves its fingerprint features to the
 * specified output file.
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
     *      -i: Input audio file name.
            -o: Output file name: specifies the path to the features file.
            -I: ID to use.
            -l: Name of the log file. If this option is specified, the program runs
                in verbose mode and the prints are written to the log file.*/
    std::string commandLineOpts = "i:o:I:l:";
    int opt;
    
    /*Declare option argument variables here*/
    std::string inputFileName, outputFileName, logFileName;
    int filmID = 0;
    int logEnable = 0;
    std::ofstream logFile;
    
    while((opt = getopt(argc,argv,commandLineOpts.c_str())) != -1){
        switch(opt){
            case 'i':
                inputFileName = std::string(optarg);
                break;
            case 'o':
                outputFileName = std::string(optarg);
                break;
            case 'I':
                filmID = std::stoi(std::string(optarg));
                break;
            case 'l':
                logEnable = 1;
                logFileName = std::string(optarg);
                logFile.open(logFileName, std::ios::app);
                logFile << "Log for Recomovie Fingerprinting Indexing.\nRecomovie PAE - May 2015\n";
                logFile << std::string(asctime(timeinfo)) + "\n\n";
                break;
        }
    }
    
    std::unordered_multimap<std::bitset<48>,std::bitset<32>> featuresMap = featureExtraction(inputFileName, filmID, logEnable, logFile);
    if(logEnable == 1){logFile << "Obtained fingerprints for input file.\n\n";}
    
    saveMultimapToFile(featuresMap,outputFileName,std::ios::trunc);
    if(logEnable == 1){logFile << "Saved fingerprints to file " + outputFileName + ".\n\n";}
    
    if(logEnable == 1){logFile << "----------------------------------------------------\n\n\n";logFile.close();}
    return 0;
}

