/* 
 * File:   binToDec.cpp
 * Author: daniel
 *
 * Created on 16 de mayo de 2015, 20:01
 */

#include <cstdlib>
#include <math.h>


/*
 * Returns the integer represented by the input binary string
 */
int binToDec(const char* binaryString,int bitDepth) {
    int dec = 0;
    
    for(int i = 1; i < bitDepth; i++){
        if(binaryString[i] == '1'){
            dec = dec + pow(2,bitDepth-i-1);
        }
    }
    
    if(binaryString[0] == '1'){ dec = dec * (-1);}
    
    return dec;
}