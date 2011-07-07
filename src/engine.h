#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
using namespace std;

class engine
{
  
public:
  class move_t { /// do we really need a move_t *CLASS* ?
  public:
    int pile;
    short nrTaken;
    move_t(int = 0, short = 0);
  };
  
  engine(vector<short>&, double=0.0, bool=false);
  move_t move(move_t);
  move_t detOptimumMove();
  bool is_ended() const;
  bool is_error() const;    /// FIXME: rewrite error API
  bool win() const;          // if the ENGINE has a winning strategy / has won
  void setDiffType(int = 0); // set the dificulty type (scaling, or constant)

private:
  bool gs_gameEnded,
       gs_win,
       gs_random,
       gs_allOne,
       gs_misere,
       gs_force,
       gs_scalingDifficulty,
       gs_error;
       
  
  const move_t nullMove; /// THIS SHOULD BE A CONST
  
  short nimSum;
  vector<short> pile;
  int   nsLength; // bitwise length of the nim sum
  int nrPiles;
  double mistakeChance; // 0 = no chance of a mistake, 1 = always make random move
  int initNrStones;   // used in difficulty scaling
  
  void detEnded();
  
  move_t findOptMove() const;
  void makeMove(move_t);
  
  void detWinning();
  move_t findRandomMove() const;
  int checkMove(move_t);
  short calculateNimSum();
  bool detAllOnes();
  bool isRandomMove() const;
};



#endif 
