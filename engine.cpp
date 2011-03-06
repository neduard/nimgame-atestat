#include "engine.h"

int determineBinaryLength(int x)
{
  int ans = 1;
  while (x >>= 1) ans++;
  return ans;
}


engine::engine(const int nr_rows, short int* p, const float mistake)
{
  nrPiles = nr_rows, mistakeChance = mistake;
  
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

bool engine::allOnes()
{
  for (int i = 0; i != nrPiles; ++i) if (pile[i] > 1) return false;
  return true;
}

bool engine::gameEnded()
{
  int i;
  for (i = 0; !pile[i] && i != nrPiles; ++i);
  return i == nrPiles;
}


short int engine::calculateNimSum()
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
  
  // TODO: Implement mistake chance
  move_t aiMove = detOptMove();
  makeMove(aiMove);
  return aiMove;
}

int engine::checkMove(move_t m)
{
  if (m.pile == -1 && !m.nrTaken) return 1; // signal that the ai should do the first move
  
  if (m.pile > nrPiles || m.pile < 0) return 2;      // no such pile
  else if (m.nrTaken > pile[m.pile]) return 3;       // overtaking from a single pile
  else if (!m.nrTaken) return 4;                     // taking a null value
  else return 0;
}

void engine::makeMove(engine::move_t m)
{
  nimSum ^= pile[m.pile];
  nimSum ^= (pile[m.pile] -= m.nrTaken);
  if ( !endGame ) endGame = allOnes();
}

engine::move_t engine::detOptMove()
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
