//
//  Arena.h
//  Project_1
//
//  Created by William  Escobar on 4/5/23.
//

#ifndef Arena_h
#define Arena_h
//#include "History.h"
#include "globals.h"
#include <iostream>
#include "History.h"

class Player;
class Rabbit;
//class History;

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     rabbitCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfRabbitsAt(int r, int c) const;
    void    display(std::string msg) const;
    History& history();//this function will return the address to the private m_history variable

      // Mutators
    void setCellStatus(int r, int c, int status);
    bool addRabbit(int r, int c);
    bool addPlayer(int r, int c);
    void moveRabbits();

  private:
    int     m_grid[MAXROWS][MAXCOLS];//2D array of integers
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Rabbit* m_rabbits[MAXRABBITS];//an array of rabbit objects
    int     m_nRabbits;
    int     m_turns;
    //History& m_history; //we create a reference to a history object
    
    History m_history; //private history class created evertime an arena object is constructed
    
      // Helper functions
    void checkPos(int r, int c, std::string functionName) const;
    bool isPosInBounds(int r, int c) const;
};
#endif /* Arena_h */
