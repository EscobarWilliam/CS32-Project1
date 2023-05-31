//
//  Game.cpp
//  Project_1
//
//  Created by William  Escobar on 4/5/23.
//

#include "Game.h"
#include <iostream>
#include "globals.h"
#include "Arena.h"
#include "Player.h"
///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRabbits)
{
    if (nRabbits < 0)//lower bound verify
    {
        std::cout << "***** Cannot create Game with negative number of rabbits!" << std::endl;
        exit(1);
    }
    if (nRabbits > MAXRABBITS)//upperbound verify
    {
        std::cout << "***** Trying to create Game with " << nRabbits
             << " rabbits; only " << MAXRABBITS << " are allowed!" << std::endl;
        exit(1);
    }
    //keep track of how many open positions are left in
    //the grid which are not occuppied by the player or
    //the rabbits if less than zero, than creating an
    //arena is not possible
    int nEmpty = rows * cols - nRabbits - 1;  // 1 for Player. This variable keeps track of how many empty spots are on our grid
    if (nEmpty < 0)
    {
        std::cout << "***** Game created with a " << rows << " by "
             << cols << " arena, which is too small to hold a player and "
             << nRabbits << " rabbits!" << std::endl;
        exit(1);
    }

    //if you made it this far all verifications have been passed
    
      // Create the arena
    m_arena = new Arena(rows, cols);
    //this generates a pointer to an Arena oject. Because we
    //call the constructor, we need to #include "Arena"
    //to include the Arena class so program knows what to do

      // Add player
    // here we generate player position and then
    //we check to make sure that position isnt
    //occupied by other objects since we initally
    //want the player to not get hurt by a bunny
    int rPlayer;
    int cPlayer;
    
    do//the next bit of code generates random position for the player until it finds a position that is empty
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);//we call
    //the member function to create a plyaer
    

      // Populate with rabbits
    while (nRabbits > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;//breaks current loop and goes to next iteration
        m_arena->addRabbit(r, c);
        nRabbits--;
    }
}

Game::~Game()
{
    delete m_arena;
}

std::string Game::takePlayerTurn()
{
    for (;;)
    {
        std::cout << "Your move (n/e/s/w/c/h or nothing): ";
        std::string playerMove;
        getline(std::cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)//no character was entered
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonedCarrot();
        }
        else if (playerMove.size() == 1)//some character was entered
        {
            if (tolower(playerMove[0]) == 'c')//we convert to lowercase and check if the option was to drop a poisoned carrot
                return player->dropPoisonedCarrot();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
            else if (playerMove[0] == 'h')
            {
                History& temp = m_arena->history();
                temp.display();//call to function to display the history grid
                
                
               return "!"; // The ! will act as a flag
            
            }
                
        }
        
        //otherwise player entered more than one character
        std::cout << "Player move must be nothing, or 1 character n/e/s/w/c/h." << std::endl;
    }
}

void Game::play()
{
    m_arena->display("");//first show player position and rabbits
    Player* player = m_arena->player();//create a pointer to the player object thats nested in the arena object. This way we can access player info directly
    if (player == nullptr)
        return;//meaning no player was created and we can end the game
    while ( ! player->isDead()  &&  m_arena->rabbitCount() > 0)
    {
        std::string msg = takePlayerTurn();
        
        //now we want to display the arena after the player moved
        m_arena->display(msg);
        if (player->isDead())
            break;
        else if(msg == "!")
            continue; //skip this iteration since looking at history doesn't count as a turn
        m_arena->moveRabbits();
        m_arena->display(msg);
    }
    if (player->isDead())
        std::cout << "You lose." << std::endl;
    else
        std::cout << "You win." << std::endl;
}

