
#include <aquila/transform/Spectrogram.h>
#include <aquila/aquila.h>
#include <vector>

std::vector<std::vector<double>> computeLog(Aquila::Spectrogram spectrogram, int logEnable, std::ofstream& logFile) {
    if(logEnable == 1){logFile << "Running computeLog...";}
    int spectrogramLength = spectrogram.getFrameCount();
    int spectrogramHeight = spectrogram.getSpectrumSize();
    std::vector<std::vector<double>> logSpectrogram(spectrogramLength, std::vector<double>(spectrogramHeight/2));
    int i, j;
    for (i=0;i<spectrogramLength;i++){
        for (j=0;j<spectrogramHeight/2;j++){
            logSpectrogram[i][j] = abs(spectrogram.getPoint(i,j));
        }
    }
    
    return logSpectrogram;
}
