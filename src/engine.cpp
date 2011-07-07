#include "engine.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <stdio.h>
using namespace std;

engine::move_t::move_t(int a, short b)
{
  pile = a;
  nrTaken = b;
}

engine::engine(vector<short>& p, double mistake,bool normal)
    : gs_misere(!normal), nullMove(0, 0), pile(p), mistakeChance(mistake), initNrStones(0)
{
  gs_gameEnded = gs_win = gs_random = gs_allOne = false;
  gs_force = gs_scalingDifficulty = gs_error = false;
  
  nrPiles = pile.size();
  for (int i = 0; i != nrPiles; ++i) initNrStones+= pile[i];

  nimSum = calculateNimSum();
  detAllOnes();
  detWinning();
}

bool engine::detAllOnes()
{
  if ( !gs_allOne )
    for (int i = 0; i != nrPiles; ++i) 
      if (pile[i] > 1) return false;
  gs_allOne = true;
  return true;
}

bool engine::is_ended() const
{
  return gs_gameEnded;
}

void engine::detEnded()
{
  if ( !gs_gameEnded ) {
    int i;
    for (i = 0; !pile[i] && i != nrPiles; ++i);
    gs_gameEnded = (i == nrPiles);
  }
}

short int engine::calculateNimSum()
{
  short sum = pile[0];
  for (int i = 1; i != nrPiles; sum ^= pile[i++]);
  return sum;
}

engine::move_t engine::move(move_t pm)
{
  if ( gs_gameEnded )           // game already ended
    return nullMove;
  
  if (!pm.nrTaken && !pm.pile) gs_force = true; /// FIXME: this is a workaround

  if ( !gs_force ) { // make player's move (we aren't forced)
    gs_force = false;
    if ( checkMove(pm) )
      return nullMove;
    else {
      makeMove(pm);
      if ( gs_gameEnded ) {  
        gs_win = gs_misere;   // player took last stone
        return nullMove;
      }
    }
  }
  
  move_t aiMove;
  if ( isRandomMove() ) {
    gs_random = true;
    aiMove = findRandomMove();
    printf("rnd mv: (%d, %d)\n", aiMove.pile, aiMove.nrTaken);
    fflush(stdout);
  } 
  else {
    gs_random = false;
    aiMove = findOptMove();
  }
  makeMove(aiMove);
  
  if ( gs_gameEnded ) gs_win = !gs_misere; // we took last stone
  else detWinning();
  
  return aiMove;
}

/// FIXME: rethink error reporting
int engine::checkMove(move_t m)
{
  if (m.pile >= nrPiles || m.pile < 0) ;      // no such pile
  else if (m.nrTaken > pile[m.pile]) ;      // over-taking from a single pile
  else if (!m.nrTaken) ;                     // taking a null value
  else {
    return false;
  }
  return true;
}

void engine::makeMove(engine::move_t m)
{
  nimSum ^= pile[m.pile];
  nimSum ^= (pile[m.pile] -= m.nrTaken);
  if (pile[m.pile] == 1) detAllOnes();
  if (!pile[m.pile]) { detEnded(); detAllOnes(); }
}

engine::move_t engine::findOptMove() const
{
  int i, k, c = 0;
  
  if ( gs_allOne ) {
    for (k = 0; k != nrPiles && !pile[k]; ++k);
    return move_t(k, 1);
  }
  
  if (!nimSum) return findRandomMove();
  
  for (i = 0; i != nrPiles; ++i) if (pile[i] > 1) {++c, k = i;}
  
  if (c == 1) {
    for (i = c = 0; i != nrPiles; ++i) if (pile[i]) ++c;
    if (c % 2 == gs_misere) return move_t(k, pile[k]- 1);
    else return move_t(k, pile[k]);
  }
  
  // now we can proceed w/ the actuall algorithm as described on wikipedia
  int mask = 1 << (sizeof(nimSum) * 8 - 1);
  while( !(nimSum & mask) ) mask >>= 1;
  for (k = 0; k != nrPiles && !(pile[k] & mask); ++k); // find k
  return move_t(k, pile[k] - (nimSum ^ pile[k]));
}

// returns a random integer in the interval [a, b]
// note that is NOT appart of the API
int roll(int a, int b)
{
  return rand() % (b - a + 1) + a;
}

bool randomEvent(double p)
{
    const int rR = 10000;
    return rand() % rR < rR * p;
}

engine::move_t engine::findRandomMove() const
{
  int k;
  while(!pile[k = roll(0, nrPiles - 1)]);
  return move_t(k, roll(1, (pile[k] + 1) / 2));  // take from 1 to floor(pile[k] / 2) stones
}


// not cool function
bool engine::isRandomMove() const
{
  const double DSC = 0.3; // <-what does this fucking do?
  double realMChance; // the real mistake chance
  if (!gs_scalingDifficulty) realMChance = mistakeChance; // compute it normally
  else {
    double currentNrStones = 0;
    for (int i = 0; i != nrPiles; ++i) currentNrStones += pile[i];
    int q = (double)initNrStones * DSC;
    if ( currentNrStones > q) 
      // unfortunately we don't know enough math for a well-working formula. Thus, this will do
      realMChance = ((currentNrStones - q) / ((double)initNrStones - q)) * mistakeChance;
    else realMChance = 0;
    
  }
  
  return randomEvent(realMChance);
    
}

void engine::detWinning()
{
  /*gs_win = gs_force ^
           ( (!nimSum & !gs_allOne) ||
           ((nimSum ^ !gs_misere) && gs_allOne) );*/

    if (gs_allOne)
        gs_win = nimSum ^ !gs_misere;
    else
        gs_win = !nimSum;
}

engine::move_t engine::detOptimumMove()
{
  if (!gs_win) return findOptMove();
  else return findRandomMove();
}

void engine::setDiffType(int t)
{
  if (t) gs_scalingDifficulty = true;
  else gs_scalingDifficulty = false;
}

bool engine::win() const
{
  return gs_win;
}
