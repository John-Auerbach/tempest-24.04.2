/* STRING SPACE MANAGEMENT ROUTINES' DEVELOPMENT PLATFORM.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>

#include "strsp.h"


void ShowSS(SS)
SharedString *SS;
{
  while (SS) {
    printf("%d; %d; %s\n",SS,SS->RefCount,SS->Data);
    SS=SS->Nxt;
  }
  puts("");
  return;
}


main()
{
  SharedString *SS=NULL;

  SharedString *S1,*S2,*S3,*S4;

  ShowSS(SS);
  S1=SS_Add(&SS,"Agadoo!");
  ShowSS(SS);
  S2=SS_Add(&SS,"Agadoo doo doo!");
  ShowSS(SS);
  S3=SS_Add(&SS,"Agadoo!");
  ShowSS(SS);

  S4=SS_Copy(S1);
  ShowSS(SS);

  SS_Remove(S1);
  ShowSS(SS);
  SS_Remove(S2);
  ShowSS(SS);
  SS_Remove(S3);
  ShowSS(SS);
  SS_Remove(S4);
  ShowSS(SS);

  puts("End");
  SS_Cleanup(&SS);
  ShowSS(SS);
}
