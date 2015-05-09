/* 
 * File:   load8kHzfile.cpp
 * Author: daniel
 *
 * Created on 9 de mayo de 2015, 12:59
 */
#include "aquila/source/WaveFile.h"
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
Aquila::WaveFile load8kHzfile(std::string inputFileName){
    std::string resampleFileName = "8kResample" + inputFileName;
    std::string soxCommand = "sox " + inputFileName + " " + resampleFileName + " channels 1 rate 8k";
    system((char*)soxCommand.c_str());
    
    Aquila::WaveFile inputFile8KResample = Aquila::WaveFile(resampleFileName, Aquila::StereoChannel.LEFT);
    
    return inputFile8KResample;
}