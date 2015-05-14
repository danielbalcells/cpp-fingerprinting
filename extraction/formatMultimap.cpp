/* 
 * File:   formatMultimap.cpp
 * Author: daniel
 *
 * Created on 9 de mayo de 2015, 19:35
 */

#include <cstdlib>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <bitset>


/*
 * 
 */
std::unordered_multimap<std::bitset<48>,std::bitset<32>> formatMultimap(std::vector<std::vector<int>> pointPairs, int filmID, int logEnable, std::ofstream& logFile){
    std::unordered_multimap<std::bitset<48>,std::bitset<32>> descriptorMultimap;
    //auto mapIterator = descriptorMultimap.begin();
    
    if(logEnable == 1){logFile << "Running formatMultimap...\n";}
    
    std::bitset<16> F1bitset, F2bitset, deltaTbitset, T1bitset;
    std::bitset<16> IDbitset(filmID);
    std::pair<std::bitset<48>,std::bitset<32>> currentMapPair;
    std::bitset<48> first;
    std::bitset<32> second;
    std::vector<int> maxPair(4);
    
           
    int i;
    for(i = 0; i < pointPairs.size(); i++){
        //Get values
        maxPair = pointPairs[i];
        
        F1bitset = std::bitset<16>(maxPair[0]);
        F2bitset = std::bitset<16>(maxPair[1]);
        deltaTbitset = std::bitset<16>(maxPair[2]);
        T1bitset = std::bitset<16>(maxPair[3]);
        
        first = std::bitset<48>(F1bitset.to_string('0','1') + F2bitset.to_string('0','1') + deltaTbitset.to_string('0','1'));
        second = std::bitset<32>(T1bitset.to_string('0','1') + IDbitset.to_string('0','1'));
        
        if(logEnable == 1){
            logFile << "F1: " + std::to_string(maxPair[0]) + "\tBin: " + F1bitset.to_string('0','1') + "\n";
            logFile << "F2: " + std::to_string(maxPair[1]) + "\tBin: " + F2bitset.to_string('0','1') + "\n";
            logFile << "DT: " + std::to_string(maxPair[2]) + "\tBin: " + deltaTbitset.to_string('0','1') + "\n";
            logFile << "T1: " + std::to_string(maxPair[3]) + "\tBin: " + T1bitset.to_string('0','1') + "\n";
            logFile << "Key bitset:\t" + first.to_string('0','1') + "\n";
            logFile << "Value bitset:\t" + second.to_string('0','1') + "\n\n";
        }
        
        //Update pair, insert into multimap
        currentMapPair.first = first;
        currentMapPair.second = second;
        
        descriptorMultimap.insert(currentMapPair);
    }
    
    return descriptorMultimap;
}
