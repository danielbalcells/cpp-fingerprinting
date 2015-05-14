#include <cstdlib>
#include <aquila/aquila.h>

/*
 * This function takes a WaveFile as input and computes its spectrogram.
 */

/*    
 * Computes the spectrogram of an audio file.
 */
Aquila::Spectrogram computeSpectrogram(Aquila::WaveFile inFile, int logEnable, std::ofstream& logFile) {
    if(logEnable == 1){
        logFile << "Running computeSpectrogram...";
    }
    Aquila::FramesCollection frames;
    frames = Aquila::FramesCollection(inFile, 512, 256);
    Aquila::Spectrogram spectrogram = Aquila::Spectrogram(frames);
    
    return spectrogram;
}
