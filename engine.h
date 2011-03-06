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
  bool gameEnded() const;

private:
  bool endGame;
  short *pile, nimSum;
  int   nsLength; /*bitwise length of the nim sum*/
  int nrPiles;
  float mistakeChance; /*0 = no chance of a mistake, 1 = always make random move*/
  
  move_t detOptMove() const;
  void makeMove(move_t);
  
  move_t detRandomMove() const;
  int checkMove(move_t) const;
  short calculateNimSum() const;
  bool allOnes() const;
  bool isRandomMove() const;
  int rool(int, int) const;
};



#endif 