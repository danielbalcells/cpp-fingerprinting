/* 
 * File:   getKeyFromBitset.cpp
 * Author: daniel
 *
 * Created on 16 de mayo de 2015, 19:30
 */

#include <cstdlib>
#include <bitset>
#include <vector>
#include <string>
#include <iostream>

int binToDec(const char* binaryString,int bitDepth);

/*
 * Returns the three ints contained in a 48-bit Key bitset
 */
std::vector<int> getKeyFromBitset(std::bitset<48> keyBitset) {
    std::vector<int> key(3);
    std::string bits = keyBitset.to_string('0','1');
    std::string f1string(bits.begin(),bits.begin()+16);
    std::string f2string(bits.begin()+16,bits.begin()+32);
    std::string deltaTstring(bits.begin()+32,bits.end());
        
    key[0] = binToDec(f1string.c_str(),16);
    key[1] = binToDec(f2string.c_str(),16);
    key[2] = binToDec(deltaTstring.c_str(),16);
    
    //Deboogie-woogie
//    std::cout << "f1: " + f1string + "\t" + std::to_string(key[0]) + "\n";
//    std::cout << "f2: " + f2string + "\t" + std::to_string(key[1]) + "\n";
//    std::cout << "f2: " + deltaTstring + "\t" + std::to_string(key[2]) + "\n";
    
    return key;
}

/*
 * Returns the two ints contained in a 32-bit Value bitset
 */
std::vector<int> getValueFromBitset(std::bitset<32> valueBitset) {
    std::vector<int> value(2);
    std::string bits = valueBitset.to_string('0','1');
    std::string t1string(bits.begin(),bits.begin()+16);
    std::string IDstring(bits.begin()+16,bits.end());
    
    value[0] = binToDec(t1string.c_str(),16);
    value[1] = binToDec(IDstring.c_str(),16);
    //Deboogie-woogie
//    std::cout << "f1: " + t1string + "\t" + std::to_string(value[0]) + "\n";
//    std::cout << "f2: " + IDstring + "\t" + std::to_string(value[1]) + "\n";
    
    return value;
}