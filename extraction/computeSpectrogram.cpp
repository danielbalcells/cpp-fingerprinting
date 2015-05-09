#include <cstdlib>
#include "aquila/source/WaveFile.h"
#include "aquila/transform/Spectrogram.h"
//#include "RecomovieSpectrogram.h"

/*
<<<<<<< HEAD
 * This function takes a WaveFile as input and computes its spectrogram.
 */
Aquila::Spectrogram computeSpectrogram(Aquila::WaveFile inFile) {
    
=======
 * Computes the spectrogram of an audio file.
 */
RecomovieSpectrogram computeSpectrogram(Aquila::WaveFile inFile) {
>>>>>>> 58fc18f9f825978ee2870ed922512ed82fd33e6f
    Aquila::FramesCollection frames;
    frames = Aquila::FramesCollection(inFile, 512, 256);
    RecomovieSpectrogram spectrogram = RecomovieSpectrogram(frames);
    
    return spectrogram;
}
