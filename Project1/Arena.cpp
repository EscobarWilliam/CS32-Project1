//
//  Arena.cpp
//  Project_1
//
//  Created by William  Escobar on 4/5/23.
//

#include "Arena.h"
#include "History.h"
#include "Player.h"//we might not have to include this header
#include "Rabbit.h"//we might not have to inlcude this header
//#include <iostream>


///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols) : m_history(nRows, nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        std::cout << "***** Arena created with invalid size " << nRows << " by "
             << nCols << "!" << std::endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRabbits = 0;
    m_turns = 0;
    //m_history = new History(nRows, nCols); //
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
    
    
}

Arena::~Arena()
{
    for (int k = 0; k < m_nRabbits; k++)
        delete m_rabbits[k];
    delete m_player;
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::rabbitCount() const
{
    return m_nRabbits;
}
//return the address to the history object inside arena
History& Arena::history()
{
    
    return m_history;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c, "Arena::getCellStatus");
    return m_grid[r-1][c-1];
}

int Arena::numberOfRabbitsAt(int r, int c) const
{
    int count = 0;
    for (int k = 0; k < m_nRabbits; k++)
    {
        Rabbit* rp = m_rabbits[k];
        if (rp->row() == r  &&  rp->col() == c)
            count++;
    }
    return count;
}

void Arena::display(std::string msg) const
{
    
    if(msg == "!")//for when we display the history grid
    {
        std::cout << "Press enter to continue.";
        std::cin.ignore(10000, '\n');//pause the game until player is ready

    }
    
    //create a grid that can be displayed to screen
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
      // Fill displayGrid with dots (empty) and stars (poisoned carrots)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');//if cell status is EMPTY place a dot else use a star

        // Indicate each rabbit's position
    for (int k = 0; k < m_nRabbits; k++)
    {
        const Rabbit* rp = m_rabbits[k]; //temp variable to hold the address of each rabbit object to make it easier to test and access member functions
        
        char& gridChar = displayGrid[rp->row()-1][rp->col()-1];
        switch (gridChar)
        {
          case '.':  gridChar = 'R'; break; //if it was previously empty now it will have a rabbit
          case 'R':  gridChar = '2'; break;//before it had a rabbit, now it has 2 rabbits
          case '9':  break;
          default:   gridChar++; break;  // '2' through '8'
        }
    }

      // Indicate the player position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');

      // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            std::cout << displayGrid[r-1][c-1];
        std::cout << std::endl;
    }
    std::cout << std::endl;

      // Write message, rabbit, and player info
    if (msg != "" && msg != "!" )//This handles the initial startup of the game where no rabbits killed and no turns taken
        std::cout << msg << std::endl;
    std::cout << "There are " << rabbitCount() << " rabbits remaining." << std::endl;
    if (m_player == nullptr)//no player was created
        std::cout << "There is no player!" << std::endl;
    else if (m_player->isDead())
        std::cout << "The player is dead." << std::endl;
    std::cout << m_turns << " turns have been taken." << std::endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c, "Arena::setCellStatus");
    m_grid[r-1][c-1] = status;
}

bool Arena::addRabbit(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a rabbit on a spot with a poisoned carrot
    if (getCellStatus(r, c) != EMPTY)
        return false;

      // Don't add a rabbit on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
        // Don't add a rabbit if exceeding max allowed # of rabbits
    if (m_nRabbits == MAXRABBITS)
        return false;
    
    //Then add a new rabbit object to the end of the array of rabbits. m_nRabbits is a counter that keeps track of last position in array
    m_rabbits[m_nRabbits] = new Rabbit(this, r, c);
    m_nRabbits++;
    return true;
    //nothing in this code that updates the CellStatus
}

bool Arena::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;

      // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

      // Don't add a player on a spot with a rabbit
    if (numberOfRabbitsAt(r, c) > 0)
        return false;
        
        //allocate memory for a Player object
    m_player = new Player(this, r, c);
    return true;
    //Nothing in this code that updates the Cell Status
}

void Arena::moveRabbits()
{
      // Move all rabbits
    for (int k = m_nRabbits-1; k >= 0; k--)
    {
        Rabbit* rp = m_rabbits[k];
        //we need to include code here that calls the
        //record history function to keep track of
        //rabbits who start out poisoned
        
        rp->move();

        if (m_player != nullptr  &&
                rp->row() == m_player->row()  &&  rp->col() == m_player->col())
            m_player->setDead();

        if (rp->isDead())
        {
            delete rp;

              // The order of Rabbit pointers in the m_rabbits array is
              // irrelevant, so it's easiest to move the last pointer to
              // replace the one pointing to the now-deleted rabbit.  Since
              // we are traversing the array from last to first, we know this
              // last pointer does not point to a dead rabbit.

            m_rabbits[k] = m_rabbits[m_nRabbits-1];
            m_nRabbits--;
        }
    }

      // Another turn has been taken
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c, std::string functionName) const //this function strictly determines whether or not the position we are looking in is within bounds if not program terminates
{
    if (!isPosInBounds(r, c))
    {
        std::cout << "***** " << "Invalid arena position (" << r << ","
             << c << ") in call to " << functionName << std::endl;
        exit(1);
    }
}
