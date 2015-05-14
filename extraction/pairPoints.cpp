/* 
 * File:   pairPoints.cpp
 * Author: daniel
 *
 * Created on 9 de mayo de 2015, 19:24
 */

#include <cstdlib>
#include <vector>
#include <iostream>
#include <fstream>

/*
 * Pairs time-frequency points within a search area
 */
std::vector<std::vector<int>> pairPoints(std::vector<std::vector<int>> maxes, int logEnable, std::ofstream& logFile){
    int maxPairsPerPoint = 3;
    int numPoints = maxes.size();
    int rectangleLeft = 0;
    int rectangleRight = 63;
    int rectangleTop = 31;
    int rectangleBottom = -32;
    //Each pair is F1:F2:T2-T1:T1
    int maxPairs = numPoints*maxPairsPerPoint;
    std::vector<std::vector<int>> pointPairs(maxPairs,std::vector<int>(4));
    
    if(logEnable == 1){
        logFile << "Running pairPoints...\n";
        logFile << "Pairing " + std::to_string(numPoints) + " points.\n";
        logFile << "Max amount of pairs is " + std::to_string(maxPairs) + ".\n";
    }
    
       
    int i,j,currentPointPairs;
    int foundPairs = 0;
    int anchorPointT,anchorPointF,candidateT,candidateF;
    for(i=0;i<numPoints;i++){ //for each point, iterate all others and see if they are in the search zone
        anchorPointT = maxes[i][0];
        anchorPointF = maxes[i][1];
        currentPointPairs = 0;
        
        //if(logEnable == 1){logFile << "i = " + std::to_string(i) + "\n";} //Careful: Extreme verbosity
        //if(logEnable == 1){logFile << "Anchor point: " + std::to_string(anchorPointT) + "-" + std::to_string(anchorPointF) + "\n";}
        
        
        for(j=0;j<numPoints && currentPointPairs < maxPairsPerPoint; j++){
            //if(logEnable == 1){logFile << "j = " + std::to_string(j) + "\n";} //Careful: Extreme verbosity
            
            if(j!=i){
                candidateT = maxes[j][0];
                candidateF = maxes[j][1];
                //if(logEnable == 1){logFile << "Candidate point: " + std::to_string(candidateT) + "-" + std::to_string(candidateF) + "\n";}
                if((candidateT-anchorPointT)<rectangleRight && (candidateT-anchorPointT) > rectangleLeft){
                    if((candidateF-anchorPointF)<rectangleTop && (candidateF-anchorPointF) > rectangleBottom){//Match!
                        pointPairs[foundPairs][0] = anchorPointF; //F1
                        pointPairs[foundPairs][1] = candidateF; //F2
                        pointPairs[foundPairs][2] = candidateT - anchorPointT; //T2-T1
                        pointPairs[foundPairs][3] = anchorPointT; //T1
                        foundPairs++;
                        currentPointPairs++;
                        if(logEnable == 1){
                            logFile << "Match! " + std::to_string(foundPairs) + " pairs, " + std::to_string(currentPointPairs) + "/" + std::to_string(maxPairsPerPoint) + " for this anchor point.\n";
                            logFile << "Pair is " + std::to_string(anchorPointF) +":"+ std::to_string(candidateF);
                            logFile << ":" + std::to_string(candidateT-anchorPointT) + ":" + std::to_string(anchorPointT) + "\n";
                        }
                    }
//                    else{
//                        if(logEnable == 1){logFile << "Candidate discarded: frequency limits exceeded.\n";}
//                    }
                }
//                else{
//                    if(logEnable == 1){logFile << "Candidate discarded: time limits exceeded.\n";}
//                }
            }
//            else{
//                if(logEnable == 1){ logFile << "Candidate discarded: same as anchor point.\n";}
//            }
        }
    }
    std::vector<std::vector<int>> retVector(std::begin(pointPairs),std::begin(pointPairs)+foundPairs);
    return retVector;
}
