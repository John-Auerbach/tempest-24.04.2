/* PRIVATE HEADER FILE OF STRING SPACE MANAGEMENT ROUTINES.

   Apostolos Lerios - TOLIS@NOVA. */


/* STRING SPACE ELEMENT. */

typedef struct _SharedString {
  char         *Data;          /* Actual string. */
  unsigned int RefCount;       /* Reference count. */

  struct _SharedString *Nxt;
} SharedString;


/* FUNCTION DECLARATIONS. */

#ifdef _NO_PROTO

/* strsp.c */

SharedString *SS_Add();
SharedString *SS_Copy();
void SS_Remove();
void SS_Cleanup();

#else

/* strsp.c */

SharedString *SS_Add(SharedString **,char *);
SharedString *SS_Copy(SharedString *);
void SS_Remove(SharedString *);
void SS_Cleanup(SharedString **);

#endif
