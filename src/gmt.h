/*****************************************************************************/
/*                                                                           */
/*   Module:    gmt.h                                                        */
/*                                                                           */
/*   Purpose:   Contains variable declarations for the GMT routines.         */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      No declarations from other modules.                          */
/*                                                                           */
/*   History:   12_Sep_93 NRV   Written.                                     */
/*                                                                           */
/*****************************************************************************/



typedef struct GMT_STRUCT 
{
   int d,h,m,s,ms ;                /* Day, Hour, Minute, and Second from     */
} GMT ;                            /*    the start of the current year       */


#define Set_GMT(G,D,H,M,S,MS)      {G->d=D; G->h=H; G->m=M; G->s=S; G->ms=MS;}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef _NO_PROTO_

double Day_FromGMT  () ;
void   GMT_FromDay  () ;
GMT   *GMT_Incr     () ;
GMT   *GMT_Decr     () ;
char  *Str_FromGMT  () ;
void   GMT_FromStr  () ;
int    GMT_Comp     () ;
double GMT_Secs     () ;
void   GMT_MonthDay () ;

#else

double Day_FromGMT  (unsigned int year, GMT *gmt_in) ;
void   GMT_FromDay  (double days_in, int *year, GMT *gmt_out) ;
GMT   *GMT_Incr     (GMT *gmt_in, int *curr_year, GMT *gmt_incr) ;
GMT   *GMT_Decr     (GMT *gmt_in, int *curr_year, GMT *gmt_decr) ;
char  *Str_FromGMT  (GMT *gmt_in) ;
void   GMT_FromStr  (char text_in[], GMT *gmt_out) ;
int    GMT_Comp     (int year1, GMT *gmt1, int year2, GMT *gmt2) ;
double GMT_Secs     (GMT *gmt) ;
void   GMT_MonthDay (GMT *gmt, int year, int *month, int *day) ;

#endif 
