#ifndef ENGINE_H
#define ENGINE_H


#include <vector>
using namespace std;

int determineBinaryLength(int);

class engine
{
  
public:
  struct move_t {
    int pile;
    short nrTaken;
  };
  
  engine(int, short*, float);
  move_t move(move_t);
  bool gameEnded();

private:
  bool endGame;
  short *pile, nimSum;
  int   nsLength; /*bitwise length of the nim sum*/
  int nrPiles;
  float mistakeChance; /*0 = no chance of a mistake, 1 = always make random move*/
  
  int checkMove(move_t);
  void makeMove(move_t);
  short calculateNimSum();
  move_t detOptMove();
  bool allOnes();
};



#endif 