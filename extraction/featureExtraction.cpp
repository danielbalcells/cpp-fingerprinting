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
 * This function is the controller for the feature extraction stage of the Recomovie
 * fingerprinting audio descriptor.
 */
Aquila::WaveFile load8kHzfile(std::string inputFileName, int logEnable, std::ofstream& logFile);
Aquila::Spectrogram computeSpectrogram(Aquila::WaveFile inFile, int logEnable, std::ofstream& logFile);
std::vector<std::vector<double>> computeLog(Aquila::Spectrogram spectrogram, int logEnable, std::ofstream& logFile);
std::vector<std::vector<double>> subtractMean(std::vector<std::vector<double>>& logSpectrogram, int logEnable, std::ofstream& logFile);
std::vector<std::vector<double>> hpFilterRows(std::vector<std::vector<double>> zmlSpectrogram);
std::vector<std::vector<int>> extractMaxes(std::vector<std::vector<double>> hpfZMLSpectrogram);
std::vector<std::vector<int>> pairPoints(std::vector<std::vector<double>> maxes);
std::unordered_multimap<std::vector<int>,std::vector<int>> formatMultimap(std::vector<std::vector<int>> pointPairs);
void printDoubleMatrix(std::vector<std::vector<double>>& inputMatrix, std::ofstream& printFile);

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
                logFile.open(logFileName, std::ios::app);
                logFile << "Log for Recomovie Fingerprinting Feature Extraction.\nRecomovie PAE - May 2015\n";
                logFile << std::string(asctime(timeinfo)) + "\n\n";
                break;
        }
    }
    
    //Fun starts here
    
    //Load audio DONE
    try{
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
        std::string logSpect = "logSpectrogram.txt";
        std::ofstream logSpectrogramFile;
        logSpectrogramFile.open(logSpect, std::ios::app);
        logFile << "Printing Log Spectrogram to file " + logSpect + "...";
        printDoubleMatrix(logSpectrogram,logSpectrogramFile);
        logFile << " Done.\n\n";
    }
        
//    //Obtain zero-mean spectrogram DONE, CURRENTLY DISABLED
//    std::vector<std::vector<double>> zmlSpectrogram = subtractMean(logSpectrogram, logEnable, logFile);
//    if(logEnable == 1){
//        logFile << "Computed Zero-Mean Log Spectrogram.\n";
//        //Plot zero-mean log spectrogram to file
//        std::string zmlSpect = "zmlSpectrogram.txt";
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
    std::vector<std::vector<int>> maxes = extractMaxes(logSpectrogram);
    if(logEnable == 1){logFile << "Extracted salient spectrogram points.\n";}
//    
//    //Pair salient points DONE
//    std::vector<std::vector<int>> pointPairs = pairPoints(maxes);
//    if(logEnable == 1){logFile << "Paired points.\n";}
//    
//    //Format points into unordered multimap
//    std::unordered_multimap<std::vector<int>,std::vector<int>> featureMultimap = formatMultimap(pointPairs,filmID);
//    if(logEnable == 1){logFile << "Paired points.\n";}
       
    
    if(logEnable == 1){logFile << "----------------------------------------------------\n\n\n";}
    return 0;
    }
    catch(std::exception& e){
        if(logEnable == 1){logFile << "Loaded file " << inputFileName << ", resampled to 8kHz.\n\n";}
    }
}

