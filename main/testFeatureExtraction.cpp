#include <cstdlib>
#include <iostream> //Used for basic prints
#include <fstream>  //Used for log file handling
#include <unistd.h> //Used for option handling
#include <aquila/aquila.h> //Used for audio processing
#include <vector>
#include <unordered_map>
#include <time.h>
#include <exception>

//using namespace std;

/*
 * This function runs the controller for the feature extraction stage of the Recomovie
 * fingerprinting audio descriptor. It is the main function in the Test Extraction configuration,
 * providing a basic input and output for the controller so it can run independently from the
 * indexing and scoring workflows.
 */
std::unordered_multimap<std::bitset<48>,std::bitset<32>> featureExtraction(std::string inputFileName, int filmID, int logEnable, std::ofstream& logFile);
void saveMultimapToFile(std::unordered_multimap<std::bitset<48>,std::bitset<32>> map, std::string fileName, std::ios::openmode oMode);
std::unordered_multimap<std::bitset<48>,std::bitset<32>> loadMultimapFromFile(std::string fileName);

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
     *      -i: Input file name: specifies the path to the audio file to analyze.
            -o: Output file name: specifies the path to the feature file.
                If it already exists, the features are appended.
                If it does not exist, it is created.
            -I: ID of the film from which the audio has been extracted. Defaults to 0 (recorded sample).
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
                logFile.open(logFileName, std::ios::trunc);
                logFile << "Log for Recomovie Fingerprinting Feature Extraction Test.\nRecomovie PAE - May 2015\n";
                logFile << std::string(asctime(timeinfo)) + "\n\n";
                break;
        }
    }
    
    try{
        std::unordered_multimap<std::bitset<48>,std::bitset<32>> featureMultimap = featureExtraction(inputFileName, filmID, logEnable, logFile);
        
        saveMultimapToFile(featureMultimap,outputFileName, std::ios::trunc);
        //Used to test loadMultimapFromFile
//        featureMultimap = loadMultimapFromFile("features.txt");
//        saveMultimapToFile(featureMultimap,"featuresRead.txt");
        
        if(logEnable == 1){logFile << "----------------------------------------------------\n\n\n";logFile.close();}
        return 0;
    }
    catch(std::exception& e){
        if(logEnable == 1){logFile << "Unknown error.\n";}
    }
}

