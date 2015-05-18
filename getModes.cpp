/* 
 * File:   getModes.cpp
 * Author: daniel
 *
 * Created on 16 de mayo de 2015, 23:01
 */

#include <cstdlib>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>
#include <map>


/*
 * 
 */
std::vector<std::vector<int>> getModes(std::vector<std::vector<int>> matches, int numIDs, int logEnable, std::ofstream& logFile) {
    if(logEnable==1){logFile << "Running getModes...\n";}
    
    std::vector<std::vector<int>> modes(numIDs, std::vector<int>(2));
    std::vector<int> currentIDmatches;
    std::pair<int,int> currentModeCounts, currentModeValues;
    std::map<int,int> counts;
    int i = 0; //Punk .|..|
    int currentValue, currentCount;
        
    for(auto idIterator = matches.begin(); idIterator != matches.end(); idIterator++, i++){
        if(logEnable == 1){logFile << "Analyzing row " + std::to_string(i) + "\n";}
        if(!idIterator->empty()){
            currentModeCounts.first = 0;
            currentModeCounts.second = 0;
            currentModeValues.first = 0;
            currentModeValues.second = 0;
            currentValue = 0;
            currentCount = 0;
            for(auto rowIterator = idIterator->begin(); rowIterator != idIterator->end(); rowIterator++){
                if(logEnable==1){logFile << "\tFound value " + std::to_string(*rowIterator) + "\n";}
                if(counts.find(*rowIterator) == counts.end()){   //If value is not in map, insert it
                    if(logEnable==1){logFile << "\t\tValue not in map, inserting with count=1.\n";}
                    counts.insert(std::pair<int,int>(*rowIterator,1));
                    currentValue = *rowIterator;
                    currentCount = 1;
                }
                else{                                           //If value is in map, update count
                    currentValue = counts.find(*rowIterator)->first;
                    currentCount = counts.find(*rowIterator)->second + 1;
                    counts.erase(counts.find(*rowIterator));
                    counts.insert(std::pair<int,int>(*rowIterator,currentCount));
                    if(logEnable==1){logFile << "\t\tValue in map with count=" + std::to_string(currentCount-1)+", increasing.\n";}
                }
                //Update first and second mode values and counts
                if(currentCount > currentModeCounts.first){ //If current count exceeds previous first mode
                    if(currentValue == currentModeValues.first){ //If current value is same as value with first mode
                        currentModeCounts.first++;
                    }
                    else if(currentValue == currentModeValues.second){ //If current value is same as value with second mode
                        currentModeCounts.second = currentModeCounts.first;
                        currentModeValues.second = currentModeCounts.first; //Move previous first mode to second mode
                        currentModeValues.first = currentValue;
                        currentModeCounts.first = currentCount;
                    }
                    else if(currentModeCounts.first == 0){
                        currentModeCounts.first = currentCount;
                        currentModeValues.first = currentValue;
                    }
                }
                else if(currentCount > currentModeCounts.second){
                    currentModeCounts.second = currentCount;
                    currentModeValues.second = currentValue;
                }
                if(logEnable==1){
                    logFile << "\t\tNew modes:\t" + std::to_string(currentModeValues.first) + " (" + std::to_string(currentModeCounts.first)+")\t";
                    logFile << std::to_string(currentModeValues.second) + " (" + std::to_string(currentModeCounts.second)+")\n";
                }
            }
            if(counts.size() > 1){
                modes[i][0] = currentModeCounts.first;
                modes[i][1] = currentModeCounts.second;
            }
        }
        else{
            if(logEnable==1){logFile << "\tRow is empty!\n";}
        }
    }
    
    return modes;
}
