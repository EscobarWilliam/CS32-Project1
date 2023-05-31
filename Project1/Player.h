//
//  Player.h
//  Project_1
//
//  Created by William  Escobar on 4/5/23.
//

#ifndef Player_h
#define Player_h
#include <string>

class Arena;


class Player
{
  public:
      // Constructor
    Player(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    std::string dropPoisonedCarrot();
    std::string move(int dir);
    void   setDead();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

#endif /* Player_h */
