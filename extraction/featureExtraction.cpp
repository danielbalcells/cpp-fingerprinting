#include <cstdlib>
#include <iostream> //Used for basic prints
#include <fstream>  //Used for log file handling
#include <unistd.h> //Used for option handling
#include "aquila/aquila.h" //Used for audio processing
#include "RecomovieSpectrogram.h"
using namespace std;

/*
 * This function is the controller for the feature extraction stage of the Recomovie
 * fingerprinting audio descriptor.
 */

RecomovieSpectrogram computeSpectrogram(Aquila::WaveFile inFile);
RecomovieSpectrogram computeLog(RecomovieSpectrogram spectrogram);
RecomovieSpectrogram subtractMean(RecomovieSpectrogram logSpectrogram);
RecomovieSpectrogram hpFilterRows(RecomovieSpectrogram zmlSpectrogram);
int** extractMaxes(RecomovieSpectrogram hpfZMLSpectrogram);

int main(int argc, char** argv) {

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
    ofstream logFile;
    
    while((opt = getopt(argc,argv,commandLineOpts.c_str())) != -1){
        switch(opt){
            case 'i':
                inputFileName = string(optarg);
                break;
            case 'o':
                outputFileName = string(optarg);
                break;
            case 'I':
                filmID = std::stoi(string(optarg));
                break;
            case 'l':
                logEnable = 1;
                logFileName = string(optarg);
                logFile.open(logFileName, ios::app);
                logFile << "Log for Recomovie Fingerprinting Feature Extraction.\nRecomovie\nPAE - May 2015\n";
                break;
        }
    }
    
    return 0;
}

