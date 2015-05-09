#include <cstdlib>
#include <aquila/aquila.h>

/*
 * TODO: documentation
 */
Aquila::Spectrogram computeSpectrogram(Aquila::WaveFile inFile) {
    Aquila::FramesCollection frames;
    frames = Aquila::FramesCollection(inFile, 512, 256);
    Aquila::Spectrogram spectrogram = Aquila::Spectrogram(frames);
    
    return spectrogram;
}
