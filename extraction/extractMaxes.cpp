/* 
 * File:   extractMaxes.cpp
 * Author: daniel
 *
 * Created on 9 de mayo de 2015, 16:21
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <valarray>
#include <math.h>

/*
 * 
 */

std::vector<double> locMax(std::vector<double>);
std::vector<double> spread(std::vector<double> x, int sd);
//std::vector<double> locMax(std::vector<double> x, std::vector<double> E);   //NOT USED IN OUR VERSION. ORIGINALLY 
                                                                              //USED IN THE BACKWARDS PRUNING STAGE

template <typename T>
std::vector<int> sort_indexes(const std::vector<T> &v) {

  // initialize original index locations
  std::vector<int> idx(v.size());
  for (int i = 0; i != idx.size(); ++i) idx[i] = i;

  // sort indexes based on comparing values in v
  std::sort(idx.begin(), idx.end(),
       [&v](int i1, int i2) {return v[i1] < v[i2];});

  return idx;
}

//template <class BidirectionalIterator>
//  void reverse (BidirectionalIterator first, BidirectionalIterator last)
//{
//  while ((first!=last)&&(first!=--last)) {
//    std::iter_swap (first,last);
//    ++first;
//  }
//}

std::vector<std::vector<int>> extractMaxes(std::vector<std::vector<double>> hpfZMLSpectrogram, int logEnable, std::ofstream& logFile){
    //INITIALIZE RETURN VECTOR OF VECTORS WITH MAXES
    if (logEnable==1){
        logFile << "Running extractMaxes..\n";
    }
    std::vector<std::vector<int>> maxes;
    //std::vector<std::vector<int>> maxes2;
    //THE COMMENTS IN LOWER-CASE ARE THE ONES IN THE ORIGINAL MATLAB CODE.
    //THE ONES IN UPPER-CASE HAVE BEEN ADDED BY A MEMBER OF THE RECOMOVIE®
    //CODING SQUAD. PEACE
        
    //THE ORIGINAL MATLAB CODE SETS THE PROCEDURE TO OBTAIN THE PAIRED MAXES FROM
    //THE VERY FIRST STAGE (READING AUDIO FILE). THIS FUNCTION BEGINS WHERE A SPECTROGRAM
    //HAS ALREADY BEEN CALCULATED AND IS LOGARITHMIC AND ZERO MEAN. THIS FUNCTION 
    //DOESN'T APPLY THE HPF TO THE ROWS OF THE SPECTROGRAM.
    
    //N is the target hashes-per-sec (approximately; default 5)
    int N = 5;
    int nFft = hpfZMLSpectrogram[0].size();
    
    //The scheme relies on just a few landmarks being common to both
    //query and reference items.  The greater the density of landmarks,
    //the more like this is to occur (meaning that shorter and noisier
    //queries can be tolerated), but the greater the load on the
    //database holding the hashes.
    //    
    //The factors influencing the number of landmarks returned are:
    // A.  The number of local maxima found, which in turn depends on 
    //   A.1 The spreading width applied to the masking skirt from each
    //       found peak (gaussian half-width in frequency bins).  A
    //       larger value means fewer peaks found.
    int f_sd = 30;
    //
    //   A.2 The decay rate of the masking skirt behind each peak
    //       (proportion per frame).  A value closer to one means fewer
    //       peaks found.
    //a_dec = 0.998;
    int a_dec = 1-0.01*(N/35);
    //0.999 -> 2.5
    //0.998 -> 5 hash/sec
    //0.997 -> 10 hash/sec
    //0.996 -> 14 hash/sec
    //0.995 -> 18
    //0.994 -> 22
    //0.993 -> 27
    //0.992 -> 30
    //0.991 -> 33
    //0.990 -> 37
    //0.98  -> 67
    //0.97  -> 97
    
    //    A.3 The maximum number of peaks allowed for each frame.  In
    //       practice, this is rarely reached, since most peaks fall
    //       below the masking skirt
    int maxpksperframe = 5;
    //
    //THIS HPF SECTION IS NOT IMPLEMENTED
    //   A.4 The high-pass filter applied to the log-magnitude
    //       envelope, which is parameterized by the position of the
    //       single real pole.  A pole close to +1.0 results in a
    //       relatively flat high-pass filter that just removes very
    //       slowly varying parts; a pole closer to -1.0 introduces
    //       increasingly extreme emphasis of rapid variations, which
    //       leads to more peaks initially.
    //NOT USED int hpf_pole = 0.98;
    //
    // B. The number of pairs made with each peak.  All maxes within a
    //    "target region" following the seed max are made into pairs,
    //    so the larger this region is (in time and frequency), the
    //    more maxes there will be.  The target region is defined by a
    //    freqency half-width (in bins)
    int targetdf = 31;  // +/- 50 bins in freq (LIMITED TO -32..31 IN LANDMARK2HASH)
    //
    //    .. and a time duration (maximum look ahead)
    int targetdt = 63;  // (LIMITED TO <64 IN LANDMARK2HASH)
    //
    //    The actual frequency and time differences are quantized and
    //    packed into the final hash; if they exceed the limited size
    //    described above, the hashes become irreversible (aliased);
    //    however, in most cases they still work (since they are
    //    handled the same way for query and reference).
    
    //Estimate for how many maxes we keep - < 30/sec (to preallocate array)
    //int maxespersec = 30;
    //
    
    //int nmaxkeep = round(maxespersec * audioDuration); //UNUSED IN THE C++ VERSION
    //maxes = zeros(3,nmaxkeep); //NOT NEEDED AS WE WILL ONLY RETURN THE LIST OF MAXES, UNPAIRED
    int nmaxes = 0;
    //int maxix = 0;


    int nFrames = hpfZMLSpectrogram.size();
    if (logEnable==1){
        logFile << "Spectrogram has " << std::to_string(nFrames) << " frames\n";
    }
/* 
 * BLOC 1
 * AGAFA ELS 10 PRIMERS FRAMES DE L'ESPECTROGRAMA (O TOTS SI EL NUMERO DE FRAMES ES <10)
 * I TROBA ELS MAXIMS EN HORITZONTAL: PER CADA FREQÜÈNCIA ÉS QUEDA AMB EL PUNT MES GRAN ENTRE ELS
 * 10 PRIMERS FRAMES.
 */
    //MINIMUM IS 10 EXCEPT IF THE NUMBER OF DFTs IN THE SPECTROGRAM IS LESS.
    int minimum;
    //NO HE SAPIGUT FERHO AMB LA FUNCIO MIN() NO SE PERQUE CONY.
    //int minimum = std::min(10,hpfZMLSpectrogram.size());
    if (10 < hpfZMLSpectrogram.size())
        minimum = 10;
    else
        minimum = hpfZMLSpectrogram.size();
    
    //A CONTINUACIÓ TROBA AQUESTS MÀXIMS I ELS VA POSANT A FIRSTMAXESVECTOR.
    // initial threshold envelope based on peaks in first 10 frames
    std::vector<double> firstMaxesVector;
    std::vector<double> spectrogramRow(minimum);
    //if(logEnable == 1){logFile << "Finding max value for each fbin along first 10 spectrogram frames.\n";}
    for (int j=0; j<nFft; j++){
        for (int i=0; i<minimum; i++){
            spectrogramRow[i] = hpfZMLSpectrogram[i][j];
        }
        firstMaxesVector.push_back(*std::max_element(std::begin(spectrogramRow), std::end(spectrogramRow)));
        //if(logEnable == 1){logFile << "\tFbin #" + std::to_string(j) + ":\t" + std::to_string(*std::max_element(std::begin(spectrogramRow), std::end(spectrogramRow))) +"\n";}
    }
    std::vector<double> sthresh;
    sthresh = spread(firstMaxesVector, f_sd);
    
    
    //EXTRACT MAXES OF THE SPECTROGRAM
    int thisFrameIndex = 0; 
    for (thisFrameIndex=0; thisFrameIndex<nFrames; thisFrameIndex++){
        std::vector<double> thisFrame = hpfZMLSpectrogram[thisFrameIndex];
        std::vector<double> sdiff(thisFrame.size());
        for (int i=0; i<thisFrame.size();i++){
            sdiff[i] = std::fmax(0,thisFrame[i]-(0.5*sthresh[i]));
        }
        logFile << "This are sdiff, sthresh , (thisFrame[i]-sthresh[i]) and thisFrame[i]:\n";
        for (int i=0; i<sdiff.size(); i++){
            logFile << std::to_string(sdiff[i]) << "      ";
            logFile << std::to_string(sthresh[i]) << "      ";
            logFile << std::to_string(thisFrame[i]-sthresh[i]) << "      ";
            logFile << std::to_string(thisFrame[i]) << "\n";
        }
        sdiff = locMax(sdiff);
        std::vector<double> thisFrameLocMax;
        thisFrameLocMax = locMax(thisFrame);
        logFile << "This are thisFrame locMaxes:\n";
        for (int i=0; i<thisFrame.size(); i++){
            logFile << std::to_string(thisFrameLocMax[i]) << "\n";
        }
        //MAKE SURE LAST BIN IS ALWAYS 0. NOT NEEDED IN C++ AS FIRST INDEX IS 0.
        //sdiff[thisFrame.size()-1] = 0;
        std::vector<int> sortedIndexs = sort_indexes(sdiff);
        std::vector<int> peaks(sdiff.size());
        for (int i=0; sdiff[sortedIndexs[i]]>0; i++){
            peaks[i]=sortedIndexs[i];
            logFile << "TOLA";
            logFile << std::to_string(peaks[i]) << "\n";
        }
        int nmaxthistime = 0;
        int thisPeakIndex;
        std::vector<int> thisMax(2);
        for (int i=0; i<peaks.size(); i++){
            thisPeakIndex = peaks[i];
            if (nmaxthistime < maxpksperframe){
                if (thisFrame[i]>sthresh[i]){
                    thisMax[0] = thisFrameIndex;
                    thisMax[1] = thisPeakIndex;
                    //logFile << "FI= " << std::to_string(thisFrameIndex) <<"   ";
                    //logFile << "PI= " << std::to_string(thisPeakIndex) << "\n";
                    //maxes[nmaxes][1]=thisFrameIndex;
                    //maxes[nmaxes][2]=thisPeakIndex;
                    maxes.push_back(thisMax);
                    nmaxthistime++;
                    nmaxes++;
                    std::vector<double> eww(sthresh.size());
                    //eww = exp(-0.5*(([1:length(sthresh)]'- p)/f_sd).^2);
                    for(int i=0; i<sthresh.size(); i++){
                        //eww = exp(-0.5*(([1:length(sthresh)]'- p)/f_sd).^2);
                        eww[i] = exp(-0.5*(pow(((i-thisPeakIndex)/f_sd),2))); //DONDE ES MEJOR DECLARARLO???
                    }
                    for (int i=0; i<sthresh.size(); i++){
                        sthresh[i] = std::fmax(sthresh[i],thisFrame[thisPeakIndex]*eww[i]);
                    }
                }
            }
        }
        for (int i=0; i<sthresh.size(); i++){
            sthresh[i] = a_dec*sthresh[i];
        }
    }
    return maxes;
    
    //BACKWARDS PRUNING OF MAXES
    std::vector<std::vector<int>> maxes2;
    int nmaxes2 = 0;
    int whichMax = nmaxes2;
    std::vector<double> lastFrame = hpfZMLSpectrogram[nFrames-1];
    sthresh = spread(lastFrame,f_sd);
    int thisFrameIndex2;
    int thisPeakIndex2;
    double thisPeakValue;
    for (int thisFrameIndex2=nFrames-1; thisFrameIndex2>=0; thisFrameIndex2--){
        std::vector<double> thisFrame2 = hpfZMLSpectrogram[thisFrameIndex2];
        while (whichMax>0 && maxes[whichMax][1]==thisFrameIndex2){
            thisPeakIndex2 = maxes[whichMax][1];
            thisPeakValue = hpfZMLSpectrogram[thisFrameIndex2][thisPeakIndex2];
            if (thisPeakValue>=sthresh[thisPeakIndex2]){
                //Keep this one
                maxes2[nmaxes2][1] = thisFrameIndex2;
                maxes2[nmaxes2][2] = thisPeakIndex2;
                nmaxes2++;
                std::vector<double> eww(sthresh.size());
                //eww = exp(-0.5*(([1:length(sthresh)]'- p)/f_sd).^2);
                for(int i=0; i<sthresh.size(); i++){
                    //eww = exp(-0.5*(([1:length(sthresh)]'- p)/f_sd).^2);
                    eww[i] = exp(-0.5*(pow((double)((double)(i-thisPeakIndex2)/(double)f_sd),2))); //DONDE ES MEJOR DECLARARLO???
                }
                for (int i=0; i<sthresh.size(); i++){
                    sthresh[i] = std::fmax(sthresh[i],thisFrame2[thisPeakIndex2]*eww[i]);
                }
            }
            whichMax--;
        }
        for (int i=0; i<sthresh.size(); i++){
            sthresh[i] = a_dec*sthresh[i];
        }
    }
    
    std::reverse(maxes2.begin(), maxes2.end());
    
    return maxes2;
}

//RETURNS A VECTOR ONLY WITH THE VALUES THAT ARE LOCAL MAXIMA OF X AND 0s IN THE
//REST OF POSITIONS.
std::vector<double> locMax(std::vector<double> x){
    int i=0;
    std::vector<double> nbrVector;
    std::valarray<double> yValArray;
    std::valarray<double> xValArray(x.data(), x.size());
    std::vector<double> y;
    nbrVector.push_back(1);
    for (i=1;i<x.size();i++){
        nbrVector.push_back(x[i]>=x[i-1]);
    }
    std::vector<double> nbrAux;
    nbrAux.assign(std::begin(nbrVector)+1,std::end(nbrVector));
    nbrAux.push_back(1);
    std::valarray<double> nbr(nbrVector.data(), nbrVector.size());
    std::valarray<double> nbr2(nbrAux.data(), nbrAux.size());
    nbr2 *= -1;
    nbr2 = nbr2+1;
    int nbrsize = nbr.size();
    int nbr2size = nbr2.size();
    nbr *= nbr2;
    xValArray *= nbr;
    //yValArray = xValArray*nbr*nbr2;
    y.assign(std::begin(xValArray), std::end(xValArray));
    return y;
}

//THIS IS THE MAGIC FUNCTION THAT SPREADS THE MAXIMA ACCORDING TO A GAUSSIAN SOMETHING.
//IF SD IS ALWAYS AN INTEGER WE CAN USE INT INSTEAD OF DOUBLE. DONE THIS WAY TO AVOID
//PROBLEMS.
//IT WAS LIATING SO NOW SD IS AN INTEGER :D
std::vector<double> spread(std::vector<double> x, int sd){
    
//    //TEST VECTOR -Dani
//    for(int i = 0; i < x.size(); i++){
//        if(i == 15 || i == 60){
//            x[i] = 3;
//        }else if(i == 120){
//            x[i] = 1;
//        }else{
//            x[i] = 0;
//        }
//    }
    std::vector<double> xLocMaxes = locMax(x);
    int W = 4*sd;
    double gaussIndexs[W+1];  //NOT USING THE PUNK WORKAROUND->VERY PUNK WORKAROUND. LETS SEE IF SD IS ALWAYS AN INT AND WE CAN AVOID THIS MEASURE :D
    std::vector<double> EVector;
    double thisE;
    std::vector<double> EE2;
    std::valarray<double> y(xLocMaxes.data(),xLocMaxes.size());
    y *= 0;
    int j=0;
    
//    std::ofstream logFile;
//    logFile.open("logs/logSpread.txt", std::ios::trunc);
    
    //logFile << "This is EVector: \n"; WORKS FINE -Dani
    for(int i=-W; i<W+1; i++){
        //E=exp(-0.5*[(-W:W)/E].^2);
        //gaussIndexs[i] = pow((i/sd),2);
        //logFile << std::to_string(i) << "     " << std::to_string((double)((double)i/(double)sd)) << "     " ;
        thisE = exp(-0.5*(pow((double)((double)i/(double)sd),2)));
        //logFile << std::to_string(thisE) << std::endl;
        EVector.push_back(thisE);
        //logFile << std::to_string(j) << "     " << std::to_string(EVector[j]) << std::endl;
        j++;
    }
    //logFile << "EVector size is: " << std::to_string(EVector.size()) << std::endl;
    std::valarray<double> E(EVector.data(), EVector.size());
    int lenx = x.size();
    int maxi = lenx + E.size();
    int spos = 1+round((E.size()-1)/2);
    std::vector<double> EE(maxi);
    //logFile << "Start pos: " + std::to_string(spos) + "\n";
    for (int i=0; i<xLocMaxes.size(); i++){
        if (xLocMaxes[i]>0){
            double locMaxi = xLocMaxes[i];
            //NO FUNCIONA, INSERT CANVIA LA MIDA DE EE EN CADA ITERACIÓ -Dani
//            std::fill(std::begin(EE), std::begin(EE)+i, 0);
//            EE.insert(std::begin(EE)+i+1, std::begin(E), std::end(E));
            
            //ACTUALITZAT: PRIMER OMPLIR EE AMB ZEROS, DESPRÉS COPIAR E A PARTIR DE LA MOSTRA i -Dani
            std::fill(EE.begin(),EE.end(),0);
            for(int j = 0; j<E.size(); j++){
                EE[i+j+1] = E[j];
            }
            
            EE[maxi]=0;
            //std::cout << "EE vector size is " + std::to_string(EE.size()) + "\n"; //DEBUG (NOT USED NOW, WORKS FINE) -Dani
            EE2 = std::vector<double>(std::begin(EE)+spos,std::begin(EE)+spos+lenx);
            for(int j=0; j<lenx; j++){
                y[j] = std::fmax(y[j], locMaxi*EE2[j]);
            }
            
            //MORE DEBUG (NOT USED NOW; WORKS FINE) -Dani
//            logFile << "\nMasking max at pos " + std::to_string(i) + " with vector EE2: \n" << std::endl;
//            logFile << "EE2Vector size is: " << std::to_string(EE2.size()) << std::endl;
//            for (int j=0; j<EE2.size(); j++){
//                logFile << std::to_string(EE2[j]) << std::endl;
//            }
        }
    }
    
    std::vector<double> retVector(std::begin(y), std::end(y));
//    logFile << "retVector is: \n" << std::endl;
//    for (int i=0; i<retVector.size(); i++){
//        logFile << std::to_string(retVector[i]) << std::endl;
//    }
//    logFile << "\n\nReturning control to extractMaxes.\n\n";
//    return retVector;
}

//template <typename T>
//std::vector<size_t> sort_indexes(const std::vector<T> &v) {
//
//  // initialize original index locations
//  std::vector<size_t> idx(v.size());
//  for (size_t i = 0; i != idx.size(); ++i) idx[i] = i;
//
//  // sort indexes based on comparing values in v
//  std::sort(idx.begin(), idx.end(),
//       [&v](size_t i1, size_t i2) {return v[i1] < v[i2];});
//
//  return idx;
//}
