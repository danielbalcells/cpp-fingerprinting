/* 
 * File:   getMatches.cpp
 * Author: daniel
 *
 * Created on 14 de mayo de 2015, 21:10
 */

#include <cstdlib>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <iostream>
#include <fstream>


std::vector<int> getValueFromBitset(std::bitset<32> valueBitset);
std::vector<int> getKeyFromBitset(std::bitset<48> keyBitset);

/*
 * Matches the descriptors stored in sampleFeatures and candidateFeatures, returning an array containing
 *  one matching time offsets vector per possible ID.
 */
std::vector<std::vector<int>> getMatches(std::unordered_multimap<std::bitset<48>,std::bitset<32>> sampleFeatures, std::unordered_multimap<std::bitset<48>,std::bitset<32>> candidateFeatures, int numIDs,int logEnable, std::ofstream& logFile) {
    if(logEnable == 1){logFile << "Running getMatches...\n";}
    std::vector<std::vector<int>> matches(numIDs,std::vector<int>());
    
    //  Actual matching code
    //  Pseudocode for this part:
    //  for each unique key in sampleFeatures
    //      get all sample values with that key
    //      get all candidate values with that key
    //          for each of those sample values
    //              for each of those candidate values
    //                  get offset between starting times and insert into matches, in the row corresponding to the candidate's ID
    
    for(auto samplesIterator = sampleFeatures.begin(); samplesIterator != sampleFeatures.end();){
        //Get key and range of values with that key
        std::bitset<48> currentSampleKey = samplesIterator->first;
        auto currentSampleRange = sampleFeatures.equal_range(currentSampleKey);
        auto currentCandidateRange = candidateFeatures.equal_range(currentSampleKey);
        std::vector<int> currentSampleKeyInts = getKeyFromBitset(currentSampleKey);
        
        if(logEnable == 1){
            logFile << "Analyzing unique key " + std::to_string(currentSampleKeyInts[0]) + ":" + std::to_string(currentSampleKeyInts[1]) + ":" + std::to_string(currentSampleKeyInts[2]) + "\n";
            logFile << std::to_string(sampleFeatures.count(currentSampleKey)) + " elements in sample with this key.\n";
            logFile << "Will now iterate over all sample values with this key.\n";
        }
        
        //Iterate over all values in sampleFeatures with the current key
        for(auto samplesWCKIterator = currentSampleRange.first; samplesWCKIterator != currentSampleRange.second; samplesWCKIterator++){
            //Get current value
            std::bitset<32> currentSampleValueBitset = samplesWCKIterator->second;
            std::vector<int> currentSampleValue = getValueFromBitset(samplesWCKIterator->second);
            int sampleStartTime = currentSampleValue[0];
            
            if(logEnable == 1){
                logFile << "\tAnalyzing sample value " + std::to_string(currentSampleValue[0]) + ":" + std::to_string(currentSampleValue[1]) + "\n";
                logFile << "\t" + std::to_string(candidateFeatures.count(currentSampleKey)) + " elements in candidate with this key.\n";
                if(candidateFeatures.count(currentSampleKey) != 0){
                logFile << "\tWill now iterate over all candidate values with this key.\n";}
            }
            
            //Iterate over all values in candidateFeatures with the current key
            for(auto candidatesWCKIterator = currentCandidateRange.first; candidatesWCKIterator != currentCandidateRange.second; candidatesWCKIterator++){
                std::vector<int> currentCandidateValue = getValueFromBitset(candidatesWCKIterator->second);
                int matchingOffset = currentCandidateValue[0]-sampleStartTime;
                int matchingFilmID = currentCandidateValue[1];
                matches[matchingFilmID].push_back(matchingOffset);
                if(logEnable == 1){
                    logFile << "\t\tInserted matching time offset " + std::to_string(matchingOffset) + " into row with ID " + std::to_string(matchingFilmID) + "\n";
                }
            }
        }
        
        //Move samplesIterator to next unique key
        samplesIterator = currentSampleRange.second;
    }
    
    return matches;
}
