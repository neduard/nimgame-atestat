#ifndef ENGINE_H
#define ENGINE_H

#include <bitset>
#include <iostream>
using namespace std;

int determineBinaryLength(int);

class engine
{
  
public:
  struct move_t {
    int pile;
    short nrTaken;
  };
  
  engine(int, short*, float, bool=false);
  move_t move(move_t);
  move_t detOptimumMove();
  bool is_ended() const;
  bool is_error() const;
  bool winning() const;
  bool won() const;
  void forceNextMove();
  void setDiffType(int=0);
  
  void print_piles()
  {
    for (int i = 0; i != nrPiles; ++i)
      cout << pile[i] << " ";
    cout << endl;
  };
  
private:
  enum GS {
    GS_gameEnded,
    GS_winning,
    GS_won,
    GS_random,
    GS_allOne,
    GS_misere,
    GS_force,
    GS_scalingDiff,
    GS_error,
    GS_IPN,
    GS_INS,
    GS_NST,
    GS_GEE
  };
  
  const move_t nullMove;
  
  bitset<16> gameStates;
  bool endGame;
  short *pile, nimSum;
  int   nsLength; /*bitwise length of the nim sum*/
  int nrPiles;
  float mistakeChance; /*0 = no chance of a mistake, 1 = always make random move*/
  int initialStones;
  
  bool detEnded();
  
  move_t findOptMove() const;
  void makeMove(move_t);
  
  void detWinning();
  move_t findRandomMove() const;
  int checkMove(move_t);
  short calculateNimSum();
  bool detAllOnes();
  bool isRandomMove() const;
  int rool(int, int) const;
  
};



#endif 