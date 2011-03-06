#include "engine.h"
#include <iostream>
#include <stdio.h>

using namespace std;
typedef engine::move_t mv;

int main()
{
  int a, pile;
  mv m;
  short nr;
  engine Bud(3, 0, 0);
  
  while (!Bud.gameEnded()) {
    printf("Your move is (stones, pile): "); scanf("%hd %d", &nr, &pile);
    if (pile == -2) break;
    m = Bud.move( (engine::move_t){pile - 1 , nr} );
    printf("Bud took %d stones from pile nr. %d\n", m.nrTaken, m.pile + 1);
  }
  return 0;
}