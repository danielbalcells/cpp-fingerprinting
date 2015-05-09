#include <cstdlib>
#include "aquila/source/WaveFile.h"
#include "aquila/transform/Spectrogram.h"


/*
 * This function takes a WaveFile as input and computes its spectrogram.
 */

/*    
 * Computes the spectrogram of an audio file.
 */
Aquila::Spectrogram computeSpectrogram(Aquila::WaveFile inFile) {
    Aquila::FramesCollection frames;
    frames = Aquila::FramesCollection(inFile, 512, 256);
    Aquila::Spectrogram spectrogram = Aquila::Spectrogram(frames);
    
    return spectrogram;
}
