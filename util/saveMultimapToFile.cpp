/* 
 * File:   saveMultimapToFile.cpp
 * Author: daniel
 *
 * Created on 17 de mayo de 2015, 13:37
 */

#include <cstdlib>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>


/*
 * 
 */
void saveMultimapToFile(std::unordered_multimap<std::bitset<48>,std::bitset<32>> map, std::string fileName, std::ios::openmode oMode) {

    std::ofstream file;
    file.open(fileName, oMode);
    std::bitset<48> currentKey;
    std::bitset<32> currentValue;
    
    for(auto mapIterator = map.begin(); mapIterator != map.end(); mapIterator++){
        currentKey = mapIterator->first;
        currentValue = mapIterator->second;
        file << currentKey.to_string('0','1') + "\t" + currentValue.to_string('0','1') + "\n";
    }
    
    file.close();
    
}
