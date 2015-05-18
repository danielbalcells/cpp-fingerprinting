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
Aquila::WaveFile load8kHzfileDummy(std::string inputFileName, int logEnable, std::ofstream& logFile);
Aquila::Spectrogram computeSpectrogram(Aquila::WaveFile inFile, int logEnable, std::ofstream& logFile);
std::vector<std::vector<double>> computeLog(Aquila::Spectrogram spectrogram, int logEnable, std::ofstream& logFile);
std::vector<std::vector<double>> subtractMean(std::vector<std::vector<double>>& logSpectrogram, int logEnable, std::ofstream& logFile);
std::vector<std::vector<double>> hpFilterRows(std::vector<std::vector<double>> zmlSpectrogram);
std::vector<std::vector<int>> extractMaxes(std::vector<std::vector<double>> hpfZMLSpectrogram, int logEnable, std::ofstream& logFile);
std::vector<std::vector<int>> pairPoints(std::vector<std::vector<int>> maxes, int logEnable, std::ofstream& logFile);
std::unordered_multimap<std::bitset<48>,std::bitset<32>> formatMultimap(std::vector<std::vector<int>> pointPairs, int filmID, int logEnable, std::ofstream& logFile);


void printDoubleMatrix(std::vector<std::vector<double>>& inputMatrix, std::ofstream& printFile);
void printIntMatrix(std::vector<std::vector<int>>& inputMatrix, std::ofstream& printFile);
std::vector<std::vector<int>> extractMaxesDummy(std::vector<std::vector<double>> hpfZMLSpectrogram, int logEnable, std::ofstream& logFile); //Deboogie-woogie

int main(int argc, char** argv) {
    /*Get timestamp, for logs*/
    std::cout << "Hola\n";
    std::string command = "pwd";
    std::system((char*)command.c_str());
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
    //Aquila::WaveFile audio8kHz = load8kHzfile(inputFileName,logEnable,logFile);
    Aquila::WaveFile audio8kHz = load8kHzfileDummy(inputFileName,logEnable,logFile);
    if(logEnable == 1){logFile << "Loaded file " << inputFileName << ", resampled to 8kHz.\n\n";}
       
    
    //Obtain spectrogram DONE
    Aquila::Spectrogram spectrogram = computeSpectrogram(audio8kHz, logEnable, logFile);
    if(logEnable == 1){
        logFile << " Done.\n";
        
        //Plot spectrogram to file
        std::string spect = "Spectrogram.txt";
        std::ofstream spectrogramFile;
        spectrogramFile.open(spect, std::ios::trunc);
        int spectrogramLength = spectrogram.getFrameCount();
        int spectrogramHeight = spectrogram.getSpectrumSize();
        std::vector<std::vector<double>> spectrogramVector(spectrogramLength, std::vector<double>(spectrogramHeight));
        int i, j;
        for (i=0;i<spectrogramLength;i++){
            for (j=0;j<spectrogramHeight;j++){
                spectrogramVector[i][j] = abs(spectrogram.getPoint(i,j));
            }
        }
        logFile << "Printing Spectrogram to file " + spect + "...";
        printDoubleMatrix(spectrogramVector, spectrogramFile);
        logFile << " Done.\n\n";
    }
    
    //Obtain log spectrogram DONE
    std::vector<std::vector<double>> logSpectrogram = computeLog(spectrogram, logEnable, logFile);
    if(logEnable == 1){
        logFile << " Done.\n";
    
        //Plot log spectrogram to file
        std::string logSpect = "logSpectrogram.txt";
        std::ofstream logSpectrogramFile;
        logSpectrogramFile.open(logSpect, std::ios::trunc);
        logFile << "Printing Log Spectrogram to file " + logSpect + "...";
        printDoubleMatrix(logSpectrogram,logSpectrogramFile);
        logFile << " Done.\n\n";
    }
        
    //Obtain zero-mean log spectrogram DONE, CURRENTLY DISABLED
    std::vector<std::vector<double>> zmlSpectrogram = subtractMean(logSpectrogram, logEnable, logFile);
    if(logEnable == 1){
        logFile << "Computed Zero-Mean Log Spectrogram.\n";
        //Plot zero-mean log spectrogram to file
        std::string zmlSpect = "zmlSpectrogram.txt";
        std::ofstream zmlSpectrogramFile;
        zmlSpectrogramFile.open(zmlSpect, std::ios::app);
        logFile << "Printing Log Spectrogram to file " + zmlSpect + "...";
        printDoubleMatrix(zmlSpectrogram,zmlSpectrogramFile);
        logFile << " Done.\n\n";
    }
//    
////    //Filter spectrogram rows CURRENTLY DISABLED
////    /*std::vector<std::vector<double>> hpfZMLSpectrogram = hpFilterRows(zmlSpectrogram);
////    if(logEnable == 1){logFile << "Filtered Spectrogram rows.\n";}*/
////    
    //Extract salient points from log spectrogram
    std::vector<std::vector<int>> maxes = extractMaxes(zmlSpectrogram, logEnable, logFile);
    if(logEnable == 1){logFile << "Extracted salient spectrogram points.\n\n";}
    
//    //Extract salient points from log spectrogram DUMMY
//    std::vector<std::vector<int>> maxes = extractMaxesDummy(logSpectrogram,logEnable,logFile);
    if(logEnable == 1){
        logFile << "Extracted salient spectrogram points.\n";
        
        //Plot maxes to file
        std::string maxesName = "maxes.txt";
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
        std::string pairsName = "pairs.txt";
        std::ofstream pairsFile;
        pairsFile.open(pairsName, std::ios::trunc);
        logFile << "Printing Pairs to file " + pairsName + "...";
        printIntMatrix(pointPairs,pairsFile);
        logFile << " Done.\n\n";
    }
    
    
    //Format points into unordered multimap
    std::unordered_multimap<std::bitset<48>,std::bitset<32>> featureMultimap;
    featureMultimap = formatMultimap(pointPairs,filmID,logEnable,logFile);
    if(logEnable == 1){logFile << "Formatted multimap.\n\n";}
       
    
    if(logEnable == 1){logFile << "----------------------------------------------------\n\n\n";logFile.close();}
    return 0;
    
    
}

