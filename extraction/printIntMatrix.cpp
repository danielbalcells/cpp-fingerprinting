/* 
 * File:   plotMatrix.cpp
 * Author: daniel
 *
 * Created on 11 de mayo de 2015, 20:24
 */

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

/*
 * 
 */
void printIntMatrix(std::vector<std::vector<int>>& inputMatrix, std::ofstream& printFile) {

    for (int i = 0; i < inputMatrix.size(); i++ )
        {
           for (int j = 0; j < inputMatrix[i].size(); j++ )
           {
              printFile << inputMatrix[i][j] << '\t';
           }
           printFile << "\n";
        }
}
