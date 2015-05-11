/* 
 * File:   pairPoints.cpp
 * Author: daniel
 *
 * Created on 9 de mayo de 2015, 19:24
 */

#include <cstdlib>
#include <vector>


/*
 * Pairs time-frequency points within a search area
 */
std::vector<std::vector<int>> pairPoints(std::vector<std::vector<int>> maxes){
    int maxPairsPerPoint = 3;
    int numPoints = maxes.size();
    int rectangleLeft = 0;
    int rectangleRight = 63;
    int rectangleTop = 31;
    int rectangleBottom = -32;
    
    //Each pair is F1:F2:T2-T1:T1
    std::vector<std::vector<int>> pointPairs(numPoints*maxPairsPerPoint,std::vector<int>(4));
    
    int i,j,currentPointPairs;
    int numPairs = 0;
    int anchorPointT,anchorPointF,candidateT,candidateF;
    for(i=0;i<numPoints;i++){ //for each point, iterate all others and see if they are in the search zone
        
        anchorPointT = maxes[i][1];
        anchorPointF = maxes[i][2];
        
        for(j=0;j<numPoints && currentPointPairs <= maxPairsPerPoint && j != i; j++){
            candidateT = maxes[j][1];
            candidateF = maxes[j][2];
            if((candidateT-anchorPointT)<rectangleRight && (candidateT-anchorPointT) > rectangleLeft){
                if((candidateF-anchorPointF)<rectangleTop && (candidateF-anchorPointF) > rectangleBottom){//Match!
                    pointPairs[numPoints][1] = anchorPointF; //F1
                    pointPairs[numPoints][2] = candidateF; //F2
                    pointPairs[numPoints][3] = candidateT - anchorPointT; //T2-T1
                    pointPairs[numPoints][4] = anchorPointT; //T1
                    numPairs++;
                    currentPointPairs++;
                }
            }
        }
    }
    return pointPairs;
}
