/* 
 * File:   featureMatching.cpp
 * Author: daniel
 *
 * Created on 14 de mayo de 2015, 17:03
 */

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <unordered_map>
#include <utility>

std::vector<std::vector<int>> getMatches(std::unordered_multimap<std::bitset<48>,std::bitset<32>> sampleFeatures, std::unordered_multimap<std::bitset<48>,std::bitset<32>> candidateFeatures, int numIDs,int logEnable, std::ofstream& logFile);
std::vector<std::vector<int>> getModes(std::vector<std::vector<int>> matches, int numIDs, int logEnable, std::ofstream& logFile);
std::vector<float> getScores(std::vector<std::vector<int>> modes, int numIDs, int logEnable, std::ofstream& logFile);

void printIntMatrix(std::vector<std::vector<int>>& inputMatrix, std::ofstream& printFile);
void printFloatVector(std::vector<float>& inputVector, std::ofstream& printFile);
/*
 * Returns the score resulting from matching sampleFeatures with candidateFeatures. 
 */
std::vector<float> featureMatching(std::unordered_multimap<std::bitset<48>,std::bitset<32>> sampleFeatures, std::unordered_multimap<std::bitset<48>,std::bitset<32>> candidateFeatures, int numIDs, int logEnable, std::ofstream& logFile){
    
    //Get an array of matching offsets for each ID
    std::vector<std::vector<int>> matches = getMatches(sampleFeatures, candidateFeatures, numIDs, logEnable, logFile);
    if(logEnable == 1){
        logFile << "Obtained To:ID pairs for matching fingerprints.\n";
        
        //Plot pairs to file
        std::string matchingOffsetsName = "logs/matchingOffsets.txt";
        std::ofstream matchingOffsetsFile;
        matchingOffsetsFile.open(matchingOffsetsName, std::ios::trunc);
        logFile << "Printing matches to file " + matchingOffsetsName + "...";
        printIntMatrix(matches,matchingOffsetsFile);
        logFile << " Done.\n\n";
    }
    
    
    //Get a pair for each ID containing the most repeated and second most repeated time offsets
    std::vector<std::vector<int>> modes = getModes(matches, numIDs, logEnable, logFile);
    if(logEnable == 1){
        logFile << "Obtained first and second most repeated offsets.\n";
        
        //Plot modes to file
        std::string modesName = "logs/modes.txt";
        std::ofstream modesFile;
        modesFile.open(modesName, std::ios::trunc);
        logFile << "Printing modes to file " + modesName + "...";
        printIntMatrix(modes,modesFile);
        logFile << " Done.\n\n";
    }
    
    //Get scores for each ID
    std::vector<float> scores = getScores(modes, numIDs, logEnable, logFile);
    if(logEnable == 1){
        logFile << "Obtained scores.\n";
        
        //Plot scores to file
        std::string scoresName = "logs/scores.txt";
        std::ofstream scoresFile;
        scoresFile.open(scoresName, std::ios::trunc);
        logFile << "Printing scores to file " + scoresName + "...";
        printFloatVector(scores,scoresFile);
        logFile << " Done.\n\n";
    }
    
    return scores;
}
