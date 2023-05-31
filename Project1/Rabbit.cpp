//
//  Rabbit.cpp
//  Project_1
//
//  Created by William  Escobar on 4/5/23.
//

#include "Rabbit.h"
#include "History.h"
#include "Arena.h"//because we are accessing member variables of the Arena pointers
///////////////////////////////////////////////////////////////////////////
//  Rabbit implementation
///////////////////////////////////////////////////////////////////////////

Rabbit::Rabbit(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        std::cout << "***** A rabbit must be created in some Arena!" << std::endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        std::cout << "***** Rabbit created with invalid coordinates (" << r << ","
             << c << ")!" << std::endl;
        exit(1);
    }
    m_arena = ap;
    //will hold the initial position the rabbit will start
    m_row = r;
    m_col = c;
    m_health = INITIAL_RABBIT_HEALTH;
    m_idleTurnsRemaining = 0;
}

int Rabbit::row() const
{
    return m_row;
}

int Rabbit::col() const
{
    return m_col;
}

bool Rabbit::isDead() const
{
    return m_health == 0;
}

void Rabbit::move()
{
    if (m_idleTurnsRemaining > 0)//this keeps track of the turn after a poisoned rabbit
    {
        //because the rabbit cant move after its been poisoned
        //we record the position that its stuck at by calling
        //the history object
        History& history = m_arena->history();
        history.record(m_row, m_col);
        m_idleTurnsRemaining--;
        return;
    }
//    if(m_health == 1)
//    {
//        //if the rabbit starting its turn poisoned but alive we need to record that position
//        History& history = m_arena->history();
//        history.record(m_row, m_col);
//    }

      // Attempt to move in a random direction; if can't move, we don't move
    if (attemptMove(*m_arena, randInt(0, NUMDIRS-1), m_row, m_col))
    {
        
        if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        {
            //set status to empty since the poison was eaten and reduce the bunnies health
            m_arena->setCellStatus(m_row, m_col, EMPTY);
            m_health--;
        }
    }

    if (m_health < INITIAL_RABBIT_HEALTH)
        m_idleTurnsRemaining = POISONED_IDLE_TIME;
}
