/*****************************************************************************/
/*                                                                           */
/*   Module:    types.h                                                      */
/*                                                                           */
/*   Purpose:   Contains declarations for variable types and structures.     */
/*                                                                           */
/*   Inputs:    None.                                                        */
/*                                                                           */
/*   Outputs:   None.                                                        */
/*                                                                           */
/*   Uses:      No other modules' declarations.                              */
/*                                                                           */
/*   History:   01_Feb_94 NRV   Rewritten.                                   */
/*                                                                           */
/*   RCS:       $Id: types.h,v 1.11 1997/08/09 22:03:06 nestorv Exp $                                                         */
/*              $Log: types.h,v $
 * Revision 1.11  1997/08/09  22:03:06  nestorv
 * Added INT_DEFS_OK stuff for machines other than mips or alpha.
 *
 * Revision 1.10  1997/05/09  15:22:11  nestorv
 * Really wanted array_length to be a pointer to int.
 *
 * Revision 1.9  1997/05/09  15:03:21  nestorv
 * Change type of array_length to integer.
 *
 * Revision 1.8  1997/05/09  13:26:06  nestorv
 * Added P_REAL_ARRAY to parameter format list.
 *
 * Revision 1.7  1997/05/09  13:24:45  nestorv
 * Added array length parameter to OUTPUT_VAR structure
 *
 * Revision 1.6  1996/01/30  18:57:16  nestorv
 * Added machine dependent 32-bit integer.
 *
 * Revision 1.5  1995/11/22  00:44:03  nestorv
 * Added GSETime structure.
 *
 * Revision 1.4  1995/10/02  19:50:20  nestorv
 * Removed P_NULL - unneeded.
 *
 * Revision 1.3  1995/10/02  19:40:29  nestorv
 * Added P_NULL type to enumerated list of parameter formats.
 *
 * Revision 1.2  1995/09/28  19:36:44  nestorv
 * Added P_FILE to parameter format type enum list, and added RCS information
 * to header.
 *                                                        */
/*                                                                           */
/*****************************************************************************/


typedef unsigned char   BYTE   ;  /* Unsigned integer 8  bits long           */
typedef unsigned short  WORD   ;  /* Unsigned integer 16 bits long           */
typedef          short  SWORD  ;  /* Signed   integer 16 bits long           */

#ifdef __mips
typedef unsigned long   UINT   ;  /* Unsigned integer 32 bits long           */
typedef          long    INT   ;  /* Signed   integer 32 bits long           */
#define INT_DEFS_OK
#endif

#ifdef __alpha
typedef unsigned int    UINT   ;  /* Unsigned integer 32 bits long           */
typedef          int     INT   ;  /* Signed   integer 32 bits long           */
#define INT_DEFS_OK
#endif

#ifndef INT_DEFS_OK
typedef unsigned long   UINT   ;  /* Unsigned integer 32 bits long           */
typedef          long    INT   ;  /* Signed   integer 32 bits long           */
#define INT_DEFS_OK
#endif

/*****************************************************************************/

#ifndef M_PI
#define M_PI			3.14159265358979323846
#define M_PI_2			1.57079632679489661923
#define M_PI_4			0.78539816339744830962
#endif

#ifndef PI
#define PI			M_PI
#endif

#define D_R_CONST             (PI/180.0)
#define R_D_CONST             (180.0/PI)

#define DEG_TO_RAD(deg)       ((deg)/180.0*PI)
#define RAD_TO_DEG(rad)       ((rad)*180.0/PI)

#define SEC_TO_DAY(sec)       ((sec)/86400.0)
#define MIN_TO_DAY(min)       ((min)/1440.0)
#define DAY_TO_YEAR(day)      ((day)/365.0)
#define DAY_TO_MIN(day)       ((day)*1440.0)
#define DAY_TO_SEC(day)       ((day)*86400.0)
#define MIN_TO_YEAR(min)      ((min)/525600.0)

#define ANGLE_MAX_PI(x)       (asin(sin(x)))
#define ANGLE_MAX_TWOPI(x)    (acos(cos(x)))

#define MAX(a,b)              ((a>b) ? a : b)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

                                   /* Enumerated list of parameter formats   */
enum {P_REAL, P_INT, P_DEG, P_ALT, P_GMT, P_STR, P_BOOL, P_VARS,
      P_TIME, P_FILE, P_GSETIME, P_REAL_ARRAY} ;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef struct
{
   UINT  seconds      ;             /* Time in seconds                       */
   UINT  microseconds ;             /* Time in microseconds                  */
} GSETime ;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

typedef struct PARAMETERS
{
   char       *token            ;  /* Parameter token that is specified      */
   char        param_type       ;  /* Parameter type from list above         */
   void       *param_ptr        ;  /* Pointer to parameter variable          */
   char       *descr            ;  /* Description of parameter (w/ units)    */
} PARAM ;

typedef struct OUTPUT_VARS
{
   char       *token            ;  /* Output variable token                  */
   int         min_tok_len      ;  /* Minimum length needed for the token    */
   char       *p_fmt            ;  /* Printing format in 'printf' format     */
   char       *p_hdr            ;  /* Column header (right justified)        */
   void       *param_ptr        ;  /* Pointer to variable                    */
   double      mult_factor      ;  /* Multiplicative factor for display      */
   char       *descr            ;  /* Description of output variable         */
   char       *plotl_label      ;  /* Label for plotl axis                   */
   char        param_type       ;  /* Parameter type from list above         */
   int        *array_length     ;  /* Number of values in array (if needed)  */
} OUTVAR ;

typedef struct TEMP_MODS
{
   char       *token            ;  /* Abbreviated name for module            */
   char       *descr            ;  /* Brief description of module            */
   char       *docs             ;  /* Pointer to long description of module  */
   void       (*init_fun) ()    ;  /* Initialization function for module     */
   void       (*sim_fun ) ()    ;  /* Function to be called every delta-t    */
   PARAM      *module_params    ;  /* Input parameters to module             */
   int         num_params       ;  /* Number of input parameters to module   */
   OUTVAR     *module_outputs   ;  /* Simulation outputs available fr module */
   int         num_outputs      ;  /* Number of outputs from module          */
   int         compute_module   ;  /* Boolean whether or not to compute      */
} MODULES ;

