/* STRING SPACE MANAGEMENT ROUTINES' TESTER.

   Apostolos Lerios - TOLIS@NOVA. */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strsp.h"


void TestStrsp()
{
  SharedString *SS=NULL;

  SharedString *S1,*S2,*S3,*S4;

  if (!(S1=SS_Add(&SS,"Str1")) || SS!=S1 ||
      S1->Nxt || strcmp(S1->Data,"Str1") || S1->RefCount!=1) {
    printf("Strsp Error (add Str1).\n");
    return;
  }
  if (!(S2=SS_Add(&SS,"Str2")) || SS!=S2 || 
      S1->Nxt || strcmp(S1->Data,"Str1") || S1->RefCount!=1 ||
      S2->Nxt!=S1 || strcmp(S2->Data,"Str2") || S2->RefCount!=1) {
    printf("Strsp Error (add Str2).\n");
    return;
  }
  if (!(S3=SS_Add(&SS,"Str1")) || SS!=S2 || S3!=S1 ||
      S1->Nxt || strcmp(S1->Data,"Str1") || S1->RefCount!=2 ||
      S2->Nxt!=S1 || strcmp(S2->Data,"Str2") || S2->RefCount!=1) {
    printf("Strsp Error (add Str1 again).\n");
    return;
  }

  if (!(S4=SS_Copy(S1)) || S4!=S1 || S1->RefCount!=3) {
    printf("Strsp Error (copy S1).\n");
    return;
  }

  SS_Remove(S1);
  if (S1->RefCount!=2) {
    printf("Strsp Error (remove S1).\n");
    return;
  }
  SS_Remove(S2);
  if (S2->RefCount!=0) {
    printf("Strsp Error (remove S2).\n");
    return;
  }
  SS_Cleanup(&SS);
  if (SS!=S1 || S1->Nxt || strcmp(S1->Data,"Str1") || S1->RefCount!=2) {
    printf("Strsp Error (cleanup 1).\n");
    return;
  }

  SS_Remove(S3);
  if (S3->RefCount!=1) {
    printf("Strsp Error (remove S3).\n");
    return;
  }
  SS_Remove(S4);
  if (S4->RefCount!=0) {
    printf("Strsp Error (remove S4).\n");
    return;
  }

  SS_Cleanup(&SS);
  if (SS) {
    printf("Strsp Error (cleanup 2).\n");
    return;
  }

  printf("Strsp passed.\n");
  return;
}


main()
{
  printf("\nTesting the string space library (libstrsp).\n\n");

  TestStrsp();
}
