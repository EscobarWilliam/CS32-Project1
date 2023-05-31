//
//  Game.h
//  Project_1
//
//  Created by William  Escobar on 4/5/23.
//



//#include gaurds to make sure that the
//Game class code isn't included twice
#ifndef Game_h
#define Game_h
#include <iostream>

class Arena; //becasue Game class only
            //has an Arena pointer, we
            //can just write this instead
            //of including the header file

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nRabbits);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    std::string takePlayerTurn();
};

#endif /* Game_h */
 
