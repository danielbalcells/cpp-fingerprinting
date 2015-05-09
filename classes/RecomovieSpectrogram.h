/* 
 * File:   RecomovieSpectrogram.h
 * Author: enric
 *
 * Created on 9 de mayo de 2015, 16:08
 */
#include <aquila/transform/Spectrogram.h>

#ifndef RECOMOVIESPECTROGRAM_H
#define	RECOMOVIESPECTROGRAM_H

class RecomovieSpectrogram: public Aquila::Spectrogram {
public:
    double** toMatrix();
private:

};

#endif	/* RECOMOVIESPECTROGRAM_H */

