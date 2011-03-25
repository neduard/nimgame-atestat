#ifndef ENGINE_H
#define ENGINE_H

#include <bitset>
#include <iostream>
using namespace std;

class engine
{
  
public:
  class move_t { /// do we really need a move_t *CLASS* ?
  public:
    int pile;
    short nrTaken;
    move_t(int = 0, int = 0);
  };
  
  engine(int, short*, float, bool=false);
  move_t move(move_t);
  move_t detOptimumMove();
  bool is_ended() const;
  bool is_error() const;
  bool winning() const; // deprecated
  bool won() const;     // deprecated
  bool win() const;     // use this instead!
  void forceNextMove();
  void setDiffType(int = 0); // set the dificulty type (scaling, or constant)
  
  void print_piles()
  {
    for (int i = 0; i != nrPiles; ++i)
      cout << pile[i] << " ";
    cout << endl;
  };
  
private:
  enum GS {
    GS_winning,
    GS_won,
    GS_error,
    GS_IPN,
    GS_INS,
    GS_NST,
    GS_GEE
  };
  
  bool gs_gameEnded,
       gs_win,
       gs_random,
       gs_allOne,
       gs_misere,
       gs_force,
       gs_scalingDifficulty,
       gs_error;
       
  
  const move_t nullMove; /// THIS SHOULD BE A CONST
  
  bitset<16> gameStates;

  short *pile, nimSum;
  int   nsLength; // bitwise length of the nim sum
  int nrPiles;
  float mistakeChance; // 0 = no chance of a mistake, 1 = always make random move
  int initNrStones;   // used in difficulty scaling
  
  bool detEnded();
  
  move_t findOptMove() const;
  void makeMove(move_t);
  
  void detWinning();
  move_t findRandomMove() const;
  int checkMove(move_t);
  short calculateNimSum();
  bool detAllOnes();
  bool isRandomMove() const;
  int roll(int, int) const;
  
};



#endif 
