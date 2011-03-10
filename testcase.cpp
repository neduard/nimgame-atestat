#include "engine.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;
typedef engine::move_t mv;

void info(engine &x)
{
  if (x.is_ended()) {
    printf("Game has ended: ");
    if (x.won()) printf("Bud won.\n");
    else printf("You won. Congrats!\n");
  } else {
    x.print_piles();
    if (x.winning()) printf("Bud has a winning strategy (you can't beat him).\n");
    else printf("Bud doesn't have a winning strategy (you can defeat him if you're careful)\n");
  }
}

void read_config(short& nr, short *&t, float& mistake, bool &normal)
{
  /** Please make sure you have testcase.txt before running! **/
  /* here's a sample testcase.txt:
    5
    2 3 5 7 9
    0
    0
   */
  ifstream f1("testcase.txt");
  f1 >> nr;
  if (nr)
    for (int i = 0; i != nr; ++i)
      f1 >> t[i];
  else {
    nr = 3;
    t = 0;
  }
  f1 >> mistake >> normal;
  f1.close();
}

int main()
{
  int a, pile;
  float mistake;
  bool normal;
  mv m;
  short nr;
  short test[100], *p = test;
  read_config(nr, p, mistake, normal);
  engine Bud(nr, p, mistake, normal);
  
  while ( !Bud.is_ended() ) {
    printf("Your move is (pile, stones): "); scanf("%d", &pile);
    
    if (pile == -2) {
      info(Bud);
      continue;
    }
    
    else if (pile == -1) {
      printf("Exiting...\n");
      return 0;
      
    } else if (pile == 0) {
      printf("Forcing Bud to make a move:\n");
      Bud.forceNextMove();
      m = Bud.move( (engine::move_t){0 , 0} );
    
    } else if (pile > 0) {
      scanf("%hd", &nr);
      m = Bud.move( (engine::move_t){pile - 1 , nr} );
    }
    
    if ( Bud.is_error() ) printf("Bud signaled an error. You probably made a wrong move.\n");
    else {
      printf("Bud took from %d, %d stones\n", m.pile + 1, m.nrTaken);
      if ( Bud.is_ended() ) info(Bud);
      
    }
    
  }
  return 0;
}