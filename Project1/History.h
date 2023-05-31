//
//  History.h
//  Project_1
//
//  Created by William  Escobar on 4/5/23.
//

#ifndef History_h
#define History_h
#include "globals.h"
class History
{
    public:
    //constructor//
    History(int nRows, int nCols);

    //member functions
    bool record(int r, int c);//this functio will record at which positions a bunny was poisoned
    void display() const;
    
    private:
    char historyGrid[MAXROWS][MAXCOLS];
    //the grid whill have a list of characters depending on how many times a bunny started their turn poisoned
    int rows;
    int cols;
};

#endif /* History_h */
