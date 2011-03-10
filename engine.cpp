#include "engine.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace std;

engine::engine(const int nr_rows, short int* p, const float mistake, const bool normal) : nullMove((move_t){0, 0})
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
  
  if (!normal) gameStates.set(GS_misere);
  nimSum = calculateNimSum();
  detAllOnes();
  detWinning();
}

bool engine::detAllOnes()
{
  if (!gameStates[GS_allOne])
    for (int i = 0; i != nrPiles; ++i) 
      if (pile[i] > 1) return false;
  gameStates.set(GS_allOne);
  return true;
}

bool engine::is_ended() const
{
  return gameStates[GS_gameEnded];
}

bool engine::detEnded()
{
  if ( !gameStates[GS_gameEnded] ) {
    int i;
    for (i = 0; !pile[i] && i != nrPiles; ++i);
    return gameStates[GS_gameEnded] = (i == nrPiles);
  }
  else return false;
}

short int engine::calculateNimSum()
{
  short sum = pile[0];
  for (int i = 1; i != nrPiles; sum ^= pile[i++]);
  return sum;
}

engine::move_t engine::move(move_t pm)
{
  if (gameStates[GS_gameEnded]) {           // game already ended
    gameStates.set(GS_error);
    gameStates.set(GS_GEE);
    return nullMove;
  }
  
  if ( !gameStates[GS_force] ) { // make player's move (we aren't forced)
    if ( checkMove(pm) )
      return nullMove;
    else {
      makeMove(pm);
      if ( gameStates[GS_gameEnded] ) {
        gameStates[GS_won] = gameStates[GS_misere];  // player took last stone
        return nullMove;
      }
    }
  }
  
  gameStates.reset(GS_force);
  
  move_t aiMove;
  if ( isRandomMove() ) {
    gameStates.set(GS_random);
    cout << "random!";
    aiMove = findRandomMove();
  }
  else {
    gameStates.reset(GS_random);
    aiMove = findOptMove();
  }
  makeMove(aiMove);
  
  if (is_ended()) gameStates[GS_won] = !gameStates[GS_misere]; // we took last stone
  else detWinning();
  
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
  
  if (gameStates[GS_allOne]) {
    for (k = 0; k != nrPiles && !pile[k]; ++k);
    return (move_t){k, 1};
  }
  
  if (!nimSum) {
    return findRandomMove();
    /*for (k = 0; k != nrPiles && !pile[k]; ++k);
    return (move_t){k, 1};*/
  }
  
  for (i = 0; i != nrPiles; ++i) if (pile[i] > 1) {++c, k = i;}
  
  if (c == 1) {
    for (i = c = 0; i != nrPiles; ++i) if (pile[i]) ++c;
    if (c % 2 == gameStates[GS_misere]) return (move_t){k, pile[k]- 1};
    else return (move_t){k, pile[k]};
  }
  
  // now we can proceed w/ the actuall algorithm as described on wikipedia
  int mask = 1 << (sizeof(nimSum) * 8 - 1);
  while( !(nimSum & mask) ) mask >>= 1;
  for (k = 0; k != nrPiles && !(pile[k] & mask); ++k); // find k
  return (move_t){k, pile[k] - (nimSum ^ pile[k])};
}

// returns a random integer in the interval [a, b]
int engine::rool(int a, int b) const
{
  return rand() % (b - a + 1) + a;
}

engine::move_t engine::findRandomMove() const
{
  int k;
  while(!pile[k = rool(0, nrPiles - 1)]);
  return (move_t){k, rool(1, pile[k])};
}


bool engine::isRandomMove() const
{
  const int rR = 1000;
  return ((rand() % rR + 1) <= rR * mistakeChance);
}

void engine::detWinning()
{ 
  gameStates[GS_winning] = gameStates[GS_force] ^ 
                           ( (!nimSum & !gameStates[GS_allOne]) || 
                             ((nimSum ^ !gameStates[GS_misere]) & gameStates[GS_allOne]) ); 
  
}

void engine::forceNextMove()
{ gameStates.set(GS_force); }

bool engine::winning() const
{ return gameStates[GS_winning]; }

bool engine::won() const
{ return gameStates[GS_won]; }

bool engine::is_error() const
{ return gameStates[GS_error]; }


