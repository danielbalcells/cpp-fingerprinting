#include <cstdlib>
#include "aquila/source/WaveFile.h"
#include "aquila/transform/Spectrogram.h"
#include "RecomovieSpectrogram.h"

/*
 * This function takes a WaveFile as input and computes its spectrogram.
 */

/*    
 * Computes the spectrogram of an audio file.
 */
RecomovieSpectrogram computeSpectrogram(Aquila::WaveFile inFile) {
    Aquila::FramesCollection frames;
    frames = Aquila::FramesCollection(inFile, 512, 256);
    RecomovieSpectrogram spectrogram = RecomovieSpectrogram(frames);
    
    return spectrogram;
}
