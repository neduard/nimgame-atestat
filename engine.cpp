#include "engine.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

engine::engine(const int nr_rows, short int* p, const float mistake)
{
  nrPiles = nr_rows, mistakeChance = mistake;
  //srand(time());  
  
  // initialize piles
  pile = new short[nrPiles];
  if (p) for (int i = 0; i != nrPiles; ++i) pile[i] = p[i];
  else {
    pile[0] = 3;
    for (int i = 1; i != nrPiles; ++i) pile[i] = pile[i - 1] + 2;
  }
  
  nimSum = calculateNimSum();
  endGame = allOnes();
}

bool engine::allOnes() const
{
  for (int i = 0; i != nrPiles; ++i) if (pile[i] > 1) return false;
  return true;
}

bool engine::gameEnded() const
{
  int i;
  for (i = 0; !pile[i] && i != nrPiles; ++i);
  return i == nrPiles;
}


short int engine::calculateNimSum() const
{
  short sum = pile[0];
  for (int i = 1; i != nrPiles; sum ^= pile[i++]);
  return sum;
}

engine::move_t engine::move(move_t pm)
{
  int chm = checkMove(pm);
  if ( chm > 1 ) return (move_t){-2,0};
  else if ( !chm ) makeMove(pm);
  
  if ( gameEnded() ) return (move_t){-1, 0};
  
  move_t aiMove;
  if ( isRandomMove() ) aiMove = detRandomMove();
  else aiMove = detOptMove();
  makeMove(aiMove);
  return aiMove;
}

int engine::checkMove(move_t m) const
{
  if (m.pile == -1 && !m.nrTaken) return 1; // signal that the ai should do the first move
  
  if (m.pile > nrPiles || m.pile < 0) return 2;      // no such pile
  else if (m.nrTaken > pile[m.pile]) return 3;       // over-taking from a single pile
  else if (!m.nrTaken) return 4;                     // taking a null value
  else return 0;
}

void engine::makeMove(engine::move_t m)
{
  nimSum ^= pile[m.pile];
  nimSum ^= (pile[m.pile] -= m.nrTaken);
  if ( !endGame ) endGame = allOnes();
}

engine::move_t engine::detOptMove() const
{
  int i, k, c = 0;
  
  if (endGame) {
    for (k = 0; k != nrPiles && !pile[k]; ++k);
    return (move_t){k, 1};
  }
  
  if (!nimSum) {
    for (k = 0; k != nrPiles && !pile[k]; ++k);
    return (move_t){k, 1};
  }
  
  for (i = 0; i != nrPiles; ++i) if (pile[i] > 1) {++c, k = i;}
  
  if (c == 1) {
    for (i = c = 0; i != nrPiles; ++i) if (pile[i]) ++c;
    if (c % 2) return (move_t){k, pile[k]- 1};
    else return (move_t){k, pile[k]};
  }
  
  // now we can proceed w/ the actuall algorithm as described on wikipedia
  int mask = 1 << (sizeof(nimSum) * 8 - 1);
  while( !(nimSum & mask) ) mask >>= 1;
  for (k = 0; k != nrPiles && !(pile[k] & mask); ++k); // find k
  return (move_t){k, pile[k] - (nimSum ^ pile[k])};
}

int engine::rool(int a, int b) const
{
  return rand() % (b - a + 1) + a;
}


engine::move_t engine::detRandomMove() const
{
  cout << "random!" << endl;
  int k;
  while(!pile[k = rool(0, nrPiles)]);
  return (move_t){k, rool(1, pile[k])};
}


bool engine::isRandomMove() const
{
  const int rR = 1000;
  return ((rand() % rR + 1) <= rR * mistakeChance);
}
