/* 
 * File:   loadMultimapFromFile.cpp
 * Author: daniel
 *
 * Created on 17 de mayo de 2015, 13:48
 */

#include <cstdlib>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <utility>



/*
 * 
 */
std::unordered_multimap<std::bitset<48>,std::bitset<32>> loadMultimapFromFile(std::string fileName) {
    std::unordered_multimap<std::bitset<48>,std::bitset<32>> map;
    std::ifstream file;
    file.open(fileName,std::ifstream::in);
    char currentLine[82];
    std::string lineString;
    std::bitset<48> keyBitset;
    std::bitset<32> valueBitset;
    //std::pair<std::bitset<48>,std::bitset<32>> currentPair;
    
    while(file.getline(currentLine,82)){
        lineString = std::string(currentLine);;
        //std::cout << std::to_string(lineString.find('\t'))+"\n";
        std::string lineKey(lineString.begin(),lineString.begin()+lineString.find('\t'));
        std::string lineValue(lineString.begin()+lineString.find('\t')+1, lineString.end());
        
        keyBitset = std::bitset<48>(lineKey);
        valueBitset = std::bitset<32>(lineValue);
        
        //currentPair = std::pair<std::bitset<48>,std::bitset<32>>(keyBitset,valueBitset);
        map.insert(std::pair<std::bitset<48>,std::bitset<32>>(keyBitset,valueBitset));
        
        //std::cout << keyBitset.to_string('0','1') + "\t" + valueBitset.to_string('0','1') + "\n";
    }
    
    return map;
}
