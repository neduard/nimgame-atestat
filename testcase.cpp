#include "engine.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;
typedef engine::move_t mv;

void info(engine &x)
{
  printf("Info section (more are planned):\n");
  if (x.is_ended()) {
    printf("Game has ended: ");
    if (x.won()) printf("Bud has won.\n");
    else printf("You have won. Congrats!\n");
  } else {
    if (x.winning()) printf("Bud has a winning strategy (you can't beat him).\n");
    else printf("Bud doesn't have a winning strategy (you can defeat him if you're careful)\n");
  }
}

int main()
{
  int a, pile;
  mv m;
  short nr;
  engine Bud(3, 0, 0);
  
  while ( 1 ) {
    printf("Your move is (pile, stones): "); scanf("%d", &pile);
    
    if (pile == -2) info(Bud);
    
    else if (pile == -1) {
      printf("Exiting...\n");
      return 0;
      
    } else if (pile == 0) {
      printf("Forcing Bud to make a move:\n");
      Bud.forceNextMove();
      m = Bud.move( (engine::move_t){0 , 0} );
      printf("Bud took from %d, %d stones\n", m.pile + 1, m.nrTaken);
    
    } else if (pile > 0) {
      scanf("%hd", &nr);
      m = Bud.move( (engine::move_t){pile - 1 , nr} );
      if ( Bud.is_error() ) printf("Bud signaled an error. You probably made a wrong move.\n");
      else if ( Bud.is_ended() ) info(Bud);
      else printf("Bud took from %d, %d stones\n", m.pile + 1, m.nrTaken);
    }
  }
  printf("Game ended. Unfortunately I don't know who won... yet!\n");
  return 0;
}