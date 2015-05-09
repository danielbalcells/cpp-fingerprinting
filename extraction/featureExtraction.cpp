#include <cstdlib>
#include <iostream> //Used for basic prints
#include <fstream>  //Used for log file handling
#include <unistd.h> //Used for option handling
#include <aquila/aquila.h> //Used for audio processing
#include <vector>
#include <unordered_map>

//using namespace std;

/*
 * This function is the controller for the feature extraction stage of the Recomovie
 * fingerprinting audio descriptor.
 */
Aquila::WaveFile load8kHzfile(std::string inputFileName44K);
Aquila::Spectrogram computeSpectrogram(Aquila::WaveFile inFile);
std::vector<std::vector<double>> computeLog(Aquila::Spectrogram spectrogram);
std::vector<std::vector<double>> subtractMean(std::vector<std::vector<double>> logSpectrogram);
std::vector<std::vector<double>> hpFilterRows(std::vector<std::vector<double>> zmlSpectrogram);
std::vector<std::vector<int>> extractMaxes(std::vector<std::vector<double>> hpfZMLSpectrogram);
std::vector<std::vector<int>> pairPoints(std::vector<std::vector<double>> maxes);
std::unordered_multimap<std::vector<int>,std::vector<int>> formatMultimap(std::vector<std::vector<int>> pointPairs);

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
                logFile << "Log for Recomovie Fingerprinting Feature Extraction.\nRecomovie\nPAE - May 2015\n";
                break;
        }
    }
    
    //Fun starts here
    
    //Load audio DONE
    Aquila::WaveFile audio8kHz = load8kHzfile(inputFileName);
    if(logEnable == 1){logFile << "Loaded file " << inputFileName << ", resampled to 8kHz.\n";}
    
    //Obtain spectrogram DONE
    Aquila::Spectrogram spectrogram = computeSpectrogram(audio8kHz);
    if(logEnable == 1){logFile << "Computed Spectrogram.\n";}
    
    //Obtain log spectrogram DONE
    std::vector<std::vector<double>> logSpectrogram = computeLog(spectrogram);
    if(logEnable == 1){logFile << "Computed Log Spectrogram.\n";}
    
    //Obtain zero-mean spectrogram DONE
    std::vector<std::vector<double>> zmlSpectrogram = subtractMean(logSpectrogram);
    if(logEnable == 1){logFile << "Computed Zero-Mean Log Spectrogram.\n";}
    
    //Filter spectrogram rows
    std::vector<std::vector<double>> hpfZMLSpectrogram = hpFilterRows(zmlSpectrogram);
    if(logEnable == 1){logFile << "Filtered Spectrogram rows.\n";}
    
    //Extract salient points from filtered spectrogram
    std::vector<std::vector<int>> maxes = extractMaxes(hpfZMLSpectrogram);
    if(logEnable == 1){logFile << "Extracted salient spectrogram points.\n";}
    
    //Pair salient points DONE
    std::vector<std::vector<int>> pointPairs = pairPoints(maxes);
    if(logEnable == 1){logFile << "Paired points.\n";}
    
    //Format points into unordered multimap
    std::unordered_multimap<std::vector<int>,std::vector<int>> featureMultimap = formatMultimap(pointPairs,filmID);
    if(logEnable == 1){logFile << "Paired points.\n";}
    
    
    return 0;
}

