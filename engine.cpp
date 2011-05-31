#include "engine.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <stdio.h>
using namespace std;

engine::move_t::move_t(int a, int b)
{
  pile = a;
  nrTaken = b;
}

engine::engine(const int nr_rows, short int *p, const float mistake, const bool normal) : nullMove(0, 0)
{
  gs_gameEnded = gs_win = gs_random = gs_allOne = false;
  gs_misere = gs_force = gs_scalingDifficulty = gs_error = false;
  nrPiles = nr_rows, mistakeChance = mistake;
  initNrStones = 0;
  
  // initialize piles
  pile = new short[nrPiles];
  if (p) for (int i = 0; i != nrPiles; ++i) pile[i] = p[i];
  else {
    pile[0] = 3;
    for (int i = 1; i != nrPiles; ++i) pile[i] = pile[i - 1] + 2;
  }
  for (int i = 1; i != nrPiles; ++i) initNrStones+= pile[i];
    
  if (!normal) gs_misere = true;
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

bool engine::detEnded()
{
  if ( !gs_gameEnded ) {
    int i;
    for (i = 0; !pile[i] && i != nrPiles; ++i);
    return gs_gameEnded = (i == nrPiles);
  }
  else return false;
}

short int engine::calculateNimSum()
{
  short sum = pile[0];
  for (int i = 1; i != nrPiles; sum ^= pile[i++]);
  return sum;
}

engine::move_t engine::move(int pile, int nr)
{
    return move(move_t(pile, nr));
}

engine::move_t engine::move(move_t pm)
{
  if ( gs_gameEnded ) {           // game already ended
    gameStates.set(GS_error);
    gameStates.set(GS_GEE);
    return nullMove;
  }
  
  if ( !gs_force ) { // make player's move (we aren't forced)
    gs_force = false;
    if ( checkMove(pm) )
      return nullMove;
    else {
      makeMove(pm);
      if ( gs_gameEnded ) {
        gameStates[GS_won] = gs_misere;  // player took last stone
        gs_win = gs_misere;
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
  
  if (is_ended()) {
    gameStates[GS_won] = !gs_misere; // we took last stone
    gs_win = !gs_misere;
  } 
  else 
    detWinning();
  
  return aiMove;
}

int engine::checkMove(move_t m)
{
  gameStates.set(GS_error);
  if (m.pile >= nrPiles || m.pile < 0) gameStates.set(GS_IPN);      // no such pile
  else if (m.nrTaken > pile[m.pile])  gameStates.set(GS_INS);      // over-taking from a single pile
  else if (!m.nrTaken) gameStates.set(GS_NST);                     // taking a null value
  else {
    gameStates.reset(GS_error);
    return false;
  }
  return true;
}

void engine::makeMove(engine::move_t m)
{
  nimSum ^= pile[m.pile];
  nimSum ^= (pile[m.pile] -= m.nrTaken);
  detAllOnes();
  detEnded();
}

engine::move_t engine::findOptMove() const
{
  int i, k, c = 0;
  
  if ( gs_allOne ) {
    for (k = 0; k != nrPiles && !pile[k]; ++k);
    return move_t(k, 1);
  }
  
  if (!nimSum) {
    return findRandomMove();
    /*for (k = 0; k != nrPiles && !pile[k]; ++k);
    return (move_t){k, 1};*/
  }
  
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
int engine::roll(int a, int b) const
{
  return rand() % (b - a + 1) + a;
}

engine::move_t engine::findRandomMove() const
{
  int k;
  while(!pile[k = roll(0, nrPiles - 1)]);
  return move_t(k, roll(1, max(pile[k] / 2, 1)));
}


bool engine::isRandomMove() const
{
  const int rR = 1000;
  const double DSC = 0.3;
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
  
  return ((rand() % rR + 1) <= rR * realMChance);
    
}

void engine::detWinning()
{ 
  gameStates[GS_winning] = gs_force ^ 
                           ( (!nimSum & !gs_allOne) || 
                           ((nimSum ^ !gs_misere) && gs_allOne) );
  gs_win = gameStates[GS_winning];
}

void engine::forceNextMove()
{ gs_force = true; }

bool engine::winning() const
{ 
  assert(gs_win == gameStates[GS_winning]);
  return gameStates[GS_winning];
}

bool engine::won() const
{ 
  assert(gs_win == gameStates[GS_won]);
  return gameStates[GS_won];
}

bool engine::is_error() const
{ return gameStates[GS_error]; }

engine::move_t engine::detOptimumMove()
{
  assert(gs_win == gameStates[GS_winning]);
  if (!gameStates[GS_winning])
    return findOptMove();
  else
    return findRandomMove();
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
