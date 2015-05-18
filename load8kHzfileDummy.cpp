#include "aquila/aquila.h"
#include <fstream>  //Used for log file handling
#include <iostream> //Used for basic prints
#include <samplerate.h> //Used to resample the input at 8kHz
#include <sndfile.h>


Aquila::WaveFile load8kHzfileDummy(std::string inputFileName, int logEnable, std::ofstream& logFile){
    
    Aquila::WaveFile inFile("/Users/enric/NetBeansProjects/Fingerprinting/" + inputFileName,Aquila::StereoChannel::LEFT);
    if(logEnable == 1){
        logFile << "\nRunning load8kHzfile...\n";
        logFile << "Input file name:\t" << inputFileName << "\n";
        logFile << "Input file path:\t" << inFile.getFilename() << "\n";
        logFile << "Input samples:\t" << std::to_string(inFile.getWaveSize()/inFile.getBytesPerSample()) << "\n";
        logFile << "Input channels:\t" << std::to_string(inFile.getChannelsNum()) << "\n";
        logFile << "Input array length (1 channel):\t" << std::to_string(inFile.getWaveSize()/inFile.getBytesPerSample()) << "\n";
        //logFile << "Resample array length:\t" << std::to_string((int)(inputFileInfo.channels*inputFileInfo.frames*8000/inputFileInfo.samplerate)) << "\n";
        //logFile << "Read samples:\t" << std::to_string(read) << "\n";
    }
    inFile.save(inFile, "data/loadedFile.wav");
    return inFile;
}
