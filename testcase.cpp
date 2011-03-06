#include "engine.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;
typedef engine::move_t mv;

int main()
{
  int a, pile;
  mv m;
  short nr;
  engine Bud(3, 0, 0.8);
  
  while ( !Bud.gameEnded() ) {
    printf("Your move is (pile, stones): "); scanf("%d", &pile);
    
    switch (pile) {
      case -2: printf("Exiting test program...\n"); return 0;
      case -1:
        printf("Forcing Bud to make a move:\n");
        m = Bud.move( (engine::move_t){-1 , 0} );
        printf("Bud took from %d, %d stones\n", m.pile + 1, m.nrTaken);
        break;
      case 0: pile = -2;
      default:
        scanf("%hd", &nr);
        m = Bud.move( (engine::move_t){pile - 1 , nr} );
        if (m.pile == -1) break;
        else if (m.pile == -2) printf("Wrong move. Try again!\n");
        else
          printf("Bud took from %d, %d stones\n", m.pile + 1, m.nrTaken);
    }
  }
  printf("Game ended. Unfortunately I don't know who won... yet!\n");
  return 0;
}