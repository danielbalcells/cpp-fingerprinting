/* 
 * File:   load8kHzfile.cpp
 * Author: daniel
 *
 * Created on 9 de mayo de 2015, 12:59
 */
#include "aquila/source/WaveFile.h"
#include <fstream>  //Used for log file handling
#include <iostream> //Used for basic prints
#include <samplerate.h> //Used to resample the input at 8kHz
#include <sndfile.h>

/* Aquila::WaveFile load8kHzfile(std::string 44KinputFileName)
 * Returns the audio file specified by the input file name,
 * resampled at 8kHz
 * 
 * Input arguments:
 *      -std::string 44KinputFileName:      A string indicating the location of the 44.1kHz audio file.
 * 
 * Return value:
 *      Aquila::WaveFile 8KresampledAudio   An Aquila Wave File containing the input file resampled at 8kHz.
 */
Aquila::WaveFile load8kHzfile(std::string inputFileName, int logEnable, std::ofstream& logFile){
//    USING SOX (DISABLED))
//    std::string resampleFileName = "8kResample" + inputFileName;
//    std::string soxCommand = "sox " + inputFileName + " " + resampleFileName + " channels 1 rate 8k";
//    if(logEnable == 1){ logFile << "Resampling file to 8kHz. Command:\n" << soxCommand <<"\n";}
//    
//    system((char*)soxCommand.c_str());
//    
//    Aquila::WaveFile inputFile8KResample(resampleFileName, Aquila::StereoChannel::LEFT);
//    
//    return inputFile8KResample;
    
    //Open input file with sndfile to resample with samplerate (from libsamplerate library)
    SNDFILE* inputFile;
    SF_INFO inputFileInfo;
    inputFile = sf_open(((char*)inputFileName.c_str()), SFM_READ,&inputFileInfo);
    
    //Arrays to store original & resampled audio
    float inputArray [inputFileInfo.channels*inputFileInfo.frames];
    float resampledArray [(int)(inputFileInfo.channels*inputFileInfo.frames*8000/inputFileInfo.samplerate)];
    int read = sf_readf_float(inputFile,inputArray,inputFileInfo.frames);
    
    if(logEnable == 1){
        logFile << "\nRunning load8kHzfile...\n";
        logFile << "Input file name:\t" << inputFileName << "\n";
        logFile << "Input samples:\t" << std::to_string(inputFileInfo.frames) << "\n";
        logFile << "Input channels:\t" << std::to_string(inputFileInfo.channels) << "\n";
        logFile << "Input array length:\t" << std::to_string(inputFileInfo.channels*inputFileInfo.frames) << "\n";
        logFile << "Resample array length:\t" << std::to_string((int)(inputFileInfo.channels*inputFileInfo.frames*8000/inputFileInfo.samplerate)) << "\n";
        logFile << "Read samples:\t" << std::to_string(read) << "\n";
    }   
    //Resample signal
    SRC_DATA resampleStruct;
    resampleStruct.data_in = inputArray;   
    resampleStruct.data_out = resampledArray;
    resampleStruct.src_ratio = (8000/(double)inputFileInfo.samplerate);
    resampleStruct.input_frames = inputFileInfo.frames;
    resampleStruct.output_frames = (int)(inputFileInfo.frames*resampleStruct.src_ratio);
    
    int resampleReturn = src_simple(&resampleStruct, SRC_SINC_MEDIUM_QUALITY, inputFileInfo.channels);
    
    if (resampleReturn != 0 && logEnable == 1){//Error
        const char* resampleErrorText = src_strerror (resampleReturn) ;
        logFile << "Error during resampling: " << std::string(resampleErrorText) << "\n";
    }
    
    if(logEnable == 1){
        logFile << "Input samples used for resampling:\t" << std::to_string(resampleStruct.input_frames_used) << "\n";
        logFile << "Resample samples:\t" << std::to_string(resampleStruct.output_frames_gen) << "\n";
        logFile << "Resample ratio:\t" << std::to_string(resampleStruct.src_ratio) << "\n";
    }
        
    //Write resampled signal to WAV file.
    std::string resampleFileName = "8kResample" + inputFileName;
    SF_INFO outputFileInfo;
    outputFileInfo.channels = inputFileInfo.channels;
    outputFileInfo.samplerate = 8000;
    outputFileInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    SNDFILE* outputFile = sf_open(resampleFileName.c_str(), SFM_WRITE, &outputFileInfo);
    sf_writef_float(outputFile, resampleStruct.data_out, resampleStruct.output_frames_gen);
    sf_close(inputFile);
    sf_close(outputFile);
    
    if(logEnable == 1){
        logFile << "Writing 8kHz array to file " << resampleFileName << "\n";
        logFile << "Output samples:\t" << std::to_string(resampleStruct.output_frames) << "\n";
        logFile << "Output channels:\t" << std::to_string(outputFileInfo.channels) << "\n";
    }
    //Return resampled file as Aquila::WaveFile
    Aquila::WaveFile inputFile8KResample(resampleFileName, Aquila::StereoChannel::LEFT);
    return inputFile8KResample;
}