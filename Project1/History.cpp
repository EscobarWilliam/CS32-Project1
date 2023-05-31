//
//  History.cpp
//  Project_1
//
//  Created by William  Escobar on 4/5/23.
//

#include "History.h"
#include <iostream>

History::History(int nRows, int nCols)
{
    //because when constructed no plays have been
    //made so there are no spots in the grid that
    //have a poisoned bunny yet,
    for (int r = 1; r <= nRows; r++)
        for (int c = 1; c <= nCols; c++)
            historyGrid[r-1][c-1] = '.';//i.e set all values to be zero
    
    cols = nCols;
    rows = nRows;
    //intialize values to zero and oconvert when we display()
}


bool History::record(int r, int c)
{
    //we need to return false if the r and c value are out of bounds
    
    char gridValue =  historyGrid[r-1][c-1];
    switch(gridValue)
    {
        case '.':  historyGrid[r-1][c-1] = 'A'; break;
        case 'Z':  break;
        default:   historyGrid[r-1][c-1]++; break;  // 'B' through 'Y'
    }
    
    return true;
}

void History::display() const
{
    clearScreen();
    for (int r = 1; r <= rows; r++)
    {
        for (int c = 1; c <= cols; c++)
            std::cout << historyGrid[r-1][c-1];
        std::cout << std::endl;
    }
    std::cout << std::endl;

}
