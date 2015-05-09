#include <cstdlib>
#include "aquila/source/WaveFile.h"
#include "aquila/transform/Spectrogram.h"

/*
 * Computes the spectrogram of an audio file.
 */
Aquila::RecomovieSpectrogram computeSpectrogram(Aquila::WaveFile inFile) {
    Aquila::FramesCollection frames;
    frames = Aquila::FramesCollection(inFile, 512, 256);
    Aquila::RecomovieSpectrogram spectrogram = Aquila::RecomovieSpectrogram(frames);
    
    return spectrogram;
}
