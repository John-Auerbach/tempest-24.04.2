/* STRING SPACE MANAGEMENT ROUTINES.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strsp.h"


/* Adds the string Str to the string space *SS, if it does not exist.
   It returns a pointer to the shared string whose contents are identical
   to Str. NULL is returned in case of memory allocation failure. */

SharedString *SS_Add(SS,Str)
SharedString **SS;
char         *Str;
{
  SharedString *SStr;
  
  /* Try to find Str in current string space. */

  SStr=(*SS);
  while (SStr) {
    if (!strcmp(SStr->Data,Str))
      return SS_Copy(SStr);
    SStr=SStr->Nxt;
  }

  /* Create new string space element. */

  if (!(SStr=(SharedString *)malloc((unsigned int)sizeof(SharedString))))
    goto Error_SS_Add;
  if (!(SStr->Data=malloc((unsigned int)(strlen(Str)+1)))) {
    free(SStr);
    goto Error_SS_Add;
  }
  strcpy(SStr->Data,Str);
  SStr->RefCount=1;

  /* Add to string space. */

  SStr->Nxt=(*SS);
  *SS=SStr;
  return SStr;

 Error_SS_Add:
  fprintf(stderr,"Warning: no more memory for new string.\n");
  return NULL;
}


/* Increases the reference count of the string *SStr. */

SharedString *SS_Copy(SStr)
SharedString *SStr;
{
  SStr->RefCount++;
  return SStr;
}


/* Reduces the reference count of the string *SStr. */

void SS_Remove(SStr)
SharedString *SStr;
{
  SStr->RefCount--;
  return;
}


/* Frees all unreferenced shared strings from the string space *SS.
   This function should be called periodically to make sure that no
   memory is wasted on unused strings. */

void SS_Cleanup(SS)
SharedString **SS;
{
  SharedString *TempSStr;

  while (*SS)
    if (!((*SS)->RefCount)) {
      TempSStr=(*SS);
      *SS=TempSStr->Nxt;
      free(TempSStr);
    } else
      SS=(&((*SS)->Nxt));
  return;
}
